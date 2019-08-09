#include "ppu.h"

void PPU::Init()
{
	//at power up 
	//ppucntl = 0000 0000
	//ppumask = 0000 0000
	//ppustatus = +x+x xxxx
	//oamaddr = $00
	//$2005/$2006 latch is cleared
	//ppu scroll = $0000
	//ppuaddr = $0000
	//ppudata = $00
	//odd frame = 0
	//OAM, palette, nt ram, chr ram = unspecifed
	//where ? is unknown, x is irrelevant, + is often set, U = unchanged
	//writes to PPUCTRL, PPUMASK, PPUSCROLL, PPUADDR are ignored for the first 29 658 cycles
	this->PPUCTRL = this->PPUMASK = 0;
	this->dtaAddress = this->tmpAddress = this->cycle = 0;
}

void PPU::load_graphics(std::vector<BYTE> graphics)
{
	//only loads 1 bank of graphics. Only works with mapper 0 for now.
	for (int i = 0; i < graphics.size(); i++)
		this->memory[i] = graphics[i];
}

void PPU::fetchNametableByte() {
	BYTE addr = 0x2000 | (this->dtaAddress & 0x0FFF); //mask off y fine
	this->_nametable = this->memory[addr];
}

void PPU::fetchPatternByte(BYTE x_fine) {
	//as each pattern occupies 16 bytes multiply by 16. Add with y fine
	BYTE addr = (this->_nametable * 16) + ((this->dtaAddress >> 12) & 0x7);
	addr |= _bgPage << 12; //find if pattern is in the high or low of page

	this->bgColor = (this->memory[addr] >> (7 ^ x_fine)) & 1; //bit 0 of palette
	this->bgColor |= ((this->memory[addr + 8] >> (7 ^ x_fine)) & 1) << 1;; //bit 1 of palette

	this->_bgOpaque = this->bgColor; //used to calculate final pixel with sprite pixel
}

void PPU::fetchAttributeByte() {
	//fetch attribute and calculate top 2 bits of palette
	BYTE addr = 0x23C0 | (this->dtaAddress & 0x0C00) | ((this->dtaAddress >> 4) & 0x38) | ((this->dtaAddress >> 2) & 0x07);
	BYTE attr = this->memory[addr];

	int sft = ((this->dtaAddress >> 4) & 4) | (this->dtaAddress & 2);
	//set upper 2 bits of color
	this->bgColor |= ((attr >> sft) & 0x3) << 2;
}

void PPU::windowInit() {
	gui.createWindow();
}

void PPU::sendRender() {
	gui.render();
}

void PPU::Clock_Tick()
{

	//Read Address Bus for changes from the CPU

	switch (ppuMode) {
		/*
		PPU renders 262 scanlines per frame. Each scanline lasts for 341 PPU clock cycles (113.667 cpu clock cycles) with each cycle producing 1 pixel.
		Pre-render scanline (-1-261)
			dummy scanline, sole purpose is to fill the shift registers with the data for the first 2 tiles of the next scanline. No pixels rendered but PPU still makes the same memory accessed it would usually
			Varies in length, depending on if odd or even frame. Odd frame - cycle at the end is skipped (dont by jumping from 339,261 to 0,0 replacing the idle tick at the beginning of the first visible scnaline
			with the last tick of the last dummy nametable fetch. Even frame - last cycle occurs normally. Done to compensate for shortcomcing with the PPU phsycal output.
			During pixels 280 through 304 the vertical scroll bits are reloaded if rendering is enabled
		*/
		/*
			In pre render, set nmi occuring, sprite overflow and sprite zero hit to false. This happens on cycle 1
			If rendering is enabled (show sprites and show background are not false)
				if cycle > 0 and < 256 render pixels
				if cycle = 257 evaluate sprites
			if cycle >= 321 and <= 336
				load latched with fetching nametable bytes and tile bytes based on cycle % 8
			Between cycles 257-320 the OAMADDR is set to 0
			If rendering is enabled copy horizontal pozition from t to v?
			If rendering is enabled copy vertical position repeatedly on cycles 280-304 (scanline 261)
		*/
	case 1:
		//pre render
		//fill registers for initial visible scanlines
		//fetch nametable, attribute table, tile bitmap low and high
		//place into latches/registers, check for sprite zero hit, etc.
		//read memory accesses into latches, when in visible scanline these latches are moved into registers, these are then multiplexed before being written to screen.
		//currently no mirroring implemented below.
		//I don't think I need to actually have seperate variable fields for the nametables and the like.
		//this->spriteLatch1 = this->memory[0x2000];
		//this->spriteAttrib1 = this->memory[0x23C0];
		//increment VRAM address
		//fetch low order byte of an 8x1 pixel of pattern table $0000 - 0FF7 or $1000-1FF7
		//fetch high order from 8 bytes higher
		//turn attribute data and pattern data into palette indices, combine them with data from sprite data using priority
		if (this->cycle == 1) {
			this->vblank = 0;
			this->spriteCollision = false;
		}
		if (this->cycle == 258 && this->showBackground && this->showSprites) {
			//show horizontal pixels
			//this sets bits related to hotizontal position
			this->dtaAddress &= ~0x41F;
			this->dtaAddress |= this->tmpAddress & 0x41F;
		}
		if (this->cycle > 280 && this->cycle < 341 && this->rendering) {
			//refresh vertical pixels
			this->dtaAddress &= ~0x7BE0;
			this->dtaAddress |= this->tmpAddress & 0x7BE0;
		}
		if (this->cycle >= 340 - (this->oddFrame && this->showBackground && this->showSprites)) {
			//if rendering is emabled then every even frame is one cycle shorter
			this->ppuMode = 2;
			this->cycle = this->scanline = 0;
		}
		break;
	case 2:
		//visible
		if (this->cycle > 0 && this->cycle <= 256) {
			//start fetching nametable bytes and the like
			if (this->showBackground) {
				int xPos = this->cycle - 1;
				int yPos = this->scanline - 1;

				//copies from simpleNES, need to figure out what it does.
				//xScroll currently never set and this will not work.
				BYTE x_fine = (this->xScroll = xPos) % 8;

				if (this->showBackground || xPos >= 8) {
					//fetch nametable
					fetchNametableByte();

					//fetch pattern
					fetchPatternByte(x_fine);

					//fetch attribute and calculate higher 2 bits of palette
					fetchAttributeByte();

					if (x_fine == 7) {
						//if coarse X == 31
						if ((this->dtaAddress & 0x001F) == 31) {
							this->dtaAddress &= 0x001F; //coarse X = 0
							this->dtaAddress ^= 0x0400; //switch horizontal nametable
						}
						else
							this->dtaAddress += 1; //increment coarse X
					}
				}

				if (this->showSprites && (this->showEdgeSprites || xPos >= 8)) {
					for (int i : this->_scanlineSprites) {
						BYTE spr_x = this->internalmemory[i * 4 + 3];

						if (0 > xPos - spr_x || xPos - spr_x >= 8)
							continue;

						BYTE spr_y = this->internalmemory[i * 4 + 0] + 1;
						BYTE tile = this->internalmemory[i * 4 + 1];
						BYTE attribute = this->internalmemory[i * 4 + 2];

						int size = this->largeSprites ? 16 : 8;

						int x_shift = (xPos - spr_x) % 8, y_offset = (yPos - spr_y) % size;

						//if not flipping horizontally
						if ((attribute & 0x40) == 0) {
							x_shift ^= 7;
						}
						//if not flipping vertically
						if ((attribute & 0x80) != 0) {
							y_offset ^= (size - 1);
						}

						BYTE addr = 0;

						if (!this->largeSprites) {
							addr = tile * 16 + y_offset;
							if (_sprPage == 1)
								addr += 0x1000;
						}
						//8x16 sprites
						else {
							//bit 3 is one if it is the bottom tile of the sprite
							y_offset = (y_offset & 7) | ((y_offset & 8) << 1);
							addr = (tile >> 1) * 32 + y_offset;
							addr |= (tile & 1) << 12; //Bank 0x1000 if bit is high
						}

						this->sprColor |= (this->memory[addr] >> x_shift) & 1; //bit 0 of palette entry
						this->sprColor |= ((this->memory[addr + 8]) >> x_shift) & 1;

						if (!(this->_spriteOpaque = this->sprColor)) {
							this->sprColor = 0;
							continue;
						}

						this->sprColor |= 0x10; //select sprite palette
						this->sprColor |= (attribute & 0x3) << 2; //bits 2 and 3

						this->_spriteForeground = !(attribute & 0x20);

						//sprite 0 hit detection
						if (!this->spriteCollision && this->showBackground && i == 0 && this->_spriteOpaque && this->_bgOpaque) {
							this->spriteCollision = 1;
						}

						break; //now done the highest priority sprite
					}

					BYTE paletteAddr = this->bgColor;

					if ((!this->_bgOpaque && this->_spriteOpaque) || (this->_bgOpaque && this->_spriteOpaque && this->_spriteForeground))
						paletteAddr = this->sprColor;
					else if (!this->_bgOpaque && !this->_spriteOpaque)
						paletteAddr = 0;

					//set xPos, yPos = palette[paletteAddr]
					//I believe this should work?
					pixels[scanline * 256 + xPos] = palette[paletteAddr];
				}
				else if (this->cycle == 257 && this->showBackground) {
					this->dtaAddress &= ~0x41F;
					this->dtaAddress |= this->tmpAddress & 0x41F;
				}

				if (this->cycle >= 340) {
					//Find and index sprites for the next scanline
					int range = 8;
					if (this->largeSprites)
						range = 16;

					int j = 0;

					for (int i = this->OAMADDR / 4; i < 64; i++) {
						int diff = (this->scanline - this->internalmemory[i * 4]);
						if (0 <= diff && diff < range) {
							this->_scanlineSprites.push_back(i);
							++j;
							if (j >= 8)
								break;
						}
					}
				}

				++this->scanline;
				this->cycle = 0;
			}

			if (this->scanline >= 240)
				this->ppuMode = 3;
		}
		break;
	case 3:
		//post render
		if (this->cycle >= 340) {
			++this->scanline;
			this->cycle = 0;
			this->ppuMode = 4;

			/*for (int x = 0; x < this->_pictureBuffer.size(); ++x) {
				for (int y = 0; y < this->pictureBuffer[0].size(); ++y) {
					screen[x][y] = color at [x][y]
				}
			}
			*/

			/*
			for (int x = 0; x < 256; x++) {
				for (int y = 0; y < 240; y++) {
					//screen at x, y = color in buffer at x, y
				}
			}
			*/

			gui.update(pixels);
		}
		break;
	case 4:
		//vblank
		if (this->cycle == 1 && this->scanline == 241) {
			this->vblank = true;
			//if (this->nmiOccurred)
				//call nmi
		}

		if (this->cycle >= 340) {
			++this->scanline;
			this->cycle = 0;
			}

		if (this->scanline >= 261) {
			this->ppuMode = 1;
			this->scanline = 0;
			this->oddFrame = !this->oddFrame;
		}
		
		break;
	default:
		break;
		cycle++;
	}

	/*
	Rendering:
		Background:
		VRAM address, temp VRAM, fine X scroll, first/second write toggle. This controls addresses that the PPU reads during background rendering
		2 16-bit registers - contain the bitmap data for 2 tiles. Every 8 cycles the bitmap data for the nex time is loaded into upper 8 bits of shift register. Pixel to render fetched from lower 8 bits
		2 8-bit shift registers -contain the palette attributes for th elower 8 pixels of the above. Fed by a latch which contains the palette attribute for the next tile. Every 8 cycles loaded with next tile
	Sprites:
		Primary OAM (holds 64 sprites for next frame)
		Secondary OAM (holds 8 sprites for current scan line)
		8 pairs of 8-bit shift registers. Contain the bitmap data for up to 8 sprites to be rendered on current scan line. Unushed are loaded with all-transparent
		8 latches - contain attribute data for up to 8 sprites
		8 counter - contain X pos for up to 8 sprites
	Every cycle a bit if fetched from the 4 background shift registers to create pixel on screen. Bit depends on the fine X scroll set by $2005. Then registers shifted once to next pixel
	Every 8 cycles new data is loaded into these registers
	Every cycle the 8 x-position counters for the sprites are decremented by one. If counter is still nonzero nothing else happens. If counter is zero the sprite becomes active' and the respective
		pair of shift registers fo the sprite is shifted once every cycle. This output accompanies the data in the sprite's latch, to form a pixel. Currenc pixel for each active sprite is checked
		from highest to lowest priority and the first non-transparent pixel moves on to a multiplexor where it joins the BG pixel.
		-If sprite has foreground priority or the BG pixel is zero, the sprite pixel i soutput
		-If sprite has background priority and the BG pixel is nonzero the BG pixel is outout (even though the sprite is behind the background it was still the highest priorty sprite to be non transparent
			and thus the only sprite to be looked at. Therefore the BG pixel is output even if another foreground priority sprite is present. This is where the sprite priority quirk comes from.)
		Sprite priority quirk - each sprite has 2 values that affect priority - index in OAM (from top down) and priority bit (attribute 2 bit 6, 0 is front 1 is back)
			Putting a back-priority sprite at a lower OAM index than a front sprite can cover up the front sprite and let the background show through.
			What happens 1) - During sprite evaluation for each scanline (cycles 65-240) the PPU searches the 8 frontmost sprites on the line. The during sprite pattern fetch (cycles 257-320) these get drawn front 
			(lower index) to back (higher index) into a buffer, taking only the first opagque pixel that matches each X coord. Priority does not affect ordering in this buffer but is saved withe ach pixel
			2) Background gets drawn to a seperate buffer
			3) for each pixel in the background buffer, the corresponding sprite pixel replaces it only if th esprite pixel is opaque and the front priority or if the background is transparent
			-The buffers don't actually exist as full-scanline buffers inside the PPU but instead as a set of coutners and shift registers. The above is implemented one pixel at a time.
	*/

	/*
	Line-by-line timnig:
		PPU renders 262 scanlines per frame. Each scanline lasts for 341 PPU clock cycles (113.667 cpu clock cycles) with each cycle producing 1 pixel.
		Pre-render scanline (-1-261)
			dummy scanline, sole purpose is to fill the shift registers with the data for the first 2 tiles of th enext scanline. No pixels rendered but PPU still makes the same memory accessed it would usually
			Varies in length, depending on if odd or even frame. Odd frame - cycle at the end is skipped (dont by jumping from 339,261 to 0,0 replacing the idle tick at the beginning of the first visible scnaline
			with the last tick of the last dummy nametable fetch. Even frame - last cycle occurs normally. Done to compensate for shortcomcing with the PPU phsycal output.
			During pixels 280 through 304 the vertical scroll bits are reloaded if rendering is enabled
		Visible Scnalines (0-239)
			These are the visible scanlines, which contain the graphics to be displayed. Includes rendering of both the background and the sprites. During these the PPU is fetching data and 
			the program should NOT access PPU, unless rendering is turned off.
			Cycle 0 -idle cycle, value on the PPU address bus during this cycle appears to be the same CHR address that is later used to fetch the low background tile byte starting at dot 5
			Cycles 1-256 - data for each tile is fetched during this phase. Each memory access takes 2 PPU cycles to complete and 4 must be performed per tile:
				1. Nametable Byte
				2. Attreibute table byte
				3. Tile bitmap low
				4. Tile bitmap high (+8 bytes from tile bitmap low)
				Data fetched from these is placed into internal lathes and then fed to appropiate shift registers when it's time to do so (every 8 cycles).
				Becuase the PPU can only fetch an attribute byte every 8 cycles, each sequential string of 8 pixels if forced to have the same palette attribute
				Sprite zero hits act as if the image starts at cycle 2(which is the same that the shifters shift for the first time) so the sprite zero flag will be raised at this point at the earliest.
				Actual pixel output is delayed further due to internal render pipelining and the first pixel output during cycle 4
				The shifteres are reloaded dureing ticks 9, 17, 25... 257
				Note: at the beginning of each line the adata for the first two tiles is arleady loaded into the shift registeres and ready to be renderes so the first tile that gets fetched is tile 3
				While all this is going on the sprite evaluation for the next scanline is taking place as a seperate process
			Cycles 257-320 - Tile data for the sprites on the next scanline are fetched here. Again, each access takes 2 cycles to complete and 4 are performed
				1. Garbage nametable byte
				2. Garbage nametable byte
				3. Tile bitmap low
				4. Tile bitmap high
				Garbage fetches occur so that the same circuitry that performs BG tile fetches can be used for sprites.
				If there are less than 6 sprites on the next scanline, then dummy fetches to tile $FF occur for left-over sprites becuase of the dummy sprite data in the secondary OAM
					This data is then discarded and the sprites are loaded with a trnansparent bitmap instead
				In addition, the X positions and attributes for each sprite are loaded from the secondary OAM into their respective counter/latches. The happens during the second garbage nametable fetch
					with the attribute loaded during first tick and the X coord during the second
			Cycles 321 - 336 - This is where the first two tiles for the next scanline are fetched and loaded into the shift registers. Again 2 per access and 4 are performed
				1. Nametable byte
				2. Attribute table byte
				3. Bitmap low
				4. bitmap high
			Cycles 337 - 240
				Two bytes are fetched but purpose is unknown. 2 cycles each
				1. Nametable Byte
				2. Nametable Byte
				Both of the bytes fetched are the same nametable byte that will be fetched atr the beginning of the next scanline (tile 3). One mapper known to use this string to clcok a scanline counter
		Post-render scnaline (240)
			PPU just idles during this scanline. VBlank set after this scanline
		Vertical Blanking Lines (241-260)
			VBlank flag of the PPU is set at tick 1(the second tick) of scanline 241, where the VBlank NMI also occures. The PPU makes no memory accesses during these lines so memory can be accessed.
	*/

	/*
	PPU bus contents:
		During frame rendering, provided enabled, the value of the bus is as indicated above. During VBlank and when rendering disabled the value on the bus is the value of the v register
		To save pins, the PPU multiplexes the lower eight vram address pins, leading to access taking 2 cycles
		During first cycle the entire VRAM address is output on the PPU address pins and the lower 8 bits stored in an external octal latch by asserting ALE line
		Second cycle the PPU only outputs the upper 6 bits of the address, with the octal latch providing te lower 8 bits (VRAM addressed 14 bits) The value is read or wrriten to lower 8 pints
		As example - the PPU VRAM address pins will have the valuie $2001 followed by $20AB for a read from VRAM address $2001 that returns value $AB
	*/

	if (this->oddFrame)
		this->oddFrame = false;
	else
		this->oddFrame = true;
}

void PPU::spriteEval() {

}

bool PPU::Reset()
{
	//at reset
	//ppurctrl = 0000 0000
	//ppumask = 0000 0000
	//ppu status = U??x xxxx
	//oamaddr is unchanged
	//$2005/$2006 latch is cleared
	//ppuscroll = $0000
	//ppuaddr is uncahnged
	//ppudata = $00
	//odd frame = no
	//oam is unspecified 
	//palette, nt ram, chr ram = unchanged

	PPUCTRL = 0;
	PPUMASK = 0;
	PPUSTATUS = PPUSTATUS & 0x80;
	PPUSCROLL = 0;
	oddFrame = false;

	return true;
}

void PPU::readRegisters(BYTE reg)
{
	BYTE data = _mainbus.read(reg);
	//set PPU register equal to value read for that register from the main bus
	switch (reg) {
	case 0x2002:
		//PPU status
		this->PPUSTATUS = data;
		break;
	case 0x2004:
		//OAM data
		this->OAMDATA = data;
		break;
	case 0x2007:
		//ppu data
		this->PPUDATA = data;
		break;
	case 0x4014:
		//oam dma
		this->OAMDMA = data;
		break;
	default:
		break;
	}
}

void PPU::writeRegisters(BYTE reg, BYTE data)
{
	//write PPU register to the main bus for the cpu to read
	_mainbus.write(reg, data);
}

void PPU::writePPUCTL(BYTE data) {
	//$2000. Write only
	/*
	7  bit  0
	---- ----
	VPHB SINN
	|||| ||||
	|||| ||++- Base nametable address
	|||| ||    (0 = $2000; 1 = $2400; 2 = $2800; 3 = $2C00)
	|||| |+--- VRAM address increment per CPU read/write of PPUDATA
	|||| |     (0: add 1, going across; 1: add 32, going down)
	|||| +---- Sprite pattern table address for 8x8 sprites
	||||       (0: $0000; 1: $1000; ignored in 8x16 mode)
	|||+------ Background pattern table address (0: $0000; 1: $1000)
	||+------- Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
	|+-------- PPU master/slave select
	|          (0: read backdrop from EXT pins; 1: output color on EXT pins)
	+--------- Generate an NMI at the start of the
           vertical blanking interval (0: off; 1: on)
		   */
	this->_bgPage = (data >> 4) & 0x1;
	this->_bgPage = this->_bgPage == 0 ? 0 : 1;
	this->_sprPage = (data >> 5) & 0x1;
	this->nmiOccurred = (data >> 7) & 0x1;
	this->largeSprites = (data >> 6) & 0x1;
	if ((data >> 5) & 0x1)
		this->dtaAddressInc = 0x20;
	else {
		this->dtaAddressInc = 1;
	}
	this->tmpAddress &= ~0xC00;
	this->tmpAddress |= (data & 0x3) << 10;
}

void PPU::writePPUMASK() {
	/*
	$2001. Write only
	7  bit  0
	---- ----
	BGRs bMmG
	|||| ||||
	|||| |||+- Greyscale (0: normal color, 1: produce a greyscale display)
	|||| ||+-- 1: Show background in leftmost 8 pixels of screen, 0: Hide
	|||| |+--- 1: Show sprites in leftmost 8 pixels of screen, 0: Hide
	|||| +---- 1: Show background
	|||+------ 1: Show sprites
	||+------- Emphasize red
	|+-------- Emphasize green
	+--------- Emphasize blue
	*/
	this->PPUMASK = (this->showSprites << 4) & (this->showBackground << 3) & (this->showEdgeSprites << 2) & (this->showEdgeBackground << 1) & (this->greyscale << 0);
}

BYTE PPU::readPPUSTATUS() {
	/*
	$2002. Read only.
	7  bit  0
	---- ----
	VSO. ....
	|||| ||||
	|||+-++++- Least significant bits previously written into a PPU register
	|||        (due to register not being updated for this address)
	||+------- Sprite overflow. The intent was for this flag to be set
	||         whenever more than eight sprites appear on a scanline, but a
	||         hardware bug causes the actual behavior to be more complicated
	||         and generate false positives as well as false negatives; see
	||         PPU sprite evaluation. This flag is set during sprite
	||         evaluation and cleared at dot 1 (the second dot) of the
	||         pre-render line.
	|+-------- Sprite 0 Hit.  Set when a nonzero pixel of sprite 0 overlaps
	|          a nonzero background pixel; cleared at dot 1 of the pre-render
	|          line.  Used for raster timing.
	+--------- Vertical blank has started (0: not in vblank; 1: in vblank).
		   Set at dot 1 of line 241 (the line *after* the post-render
		   line); cleared after reading $2002 and at dot 1 of the
		   pre-render line.
	*/
	BYTE status;
	status = (spriteCollision << 6) & (vblank << 7);
	this->vblank = false;
	this->firstWrite = true;
	return status;
}

void PPU::writeOAMADDR(BYTE data) {
	/*
	$2003.
	Write the address of OAM to access here
	*/
	this->OAMADDR = data;
}

void PPU::writeOAMDATA(BYTE data) {
	/*
	$2004
	Write OAM Data here
	*/
	this->internalmemory[this->OAMADDR] = data;
}

BYTE PPU::readOAMDATA() {
	/*
	Read OAM Data
	*/
	return this->OAMDATA;
}

void PPU::writePPUSCROLL(BYTE data) {
	/*
	$2005
	Write the Scroll
	*/
	this->PPUSCROLL = data;
}

void PPU::writePPUADDR(BYTE data) {
	/*
	$2006
	Because the CPU and the PPU are on separate buses, neither has direct access to the other's memory. 
	The CPU writes to VRAM through a pair of registers on the PPU. First it loads an address into PPUADDR, and then it writes repeatedly to PPUDATA to fill VRAM.
	*/
	this->PPUADDR = data;
}

void PPU::writePPUDATA(BYTE data) {
	/*
	$2007
	VRAM read/write data register. After access, the video memory address will increment by an amount determined by bit 2 of $2000
	*/
	this->PPUDATA = data;
	this->dtaAddress += (this->PPUCTRL & 0x02);
	//this->dtaAddress += this->dtaAddressInc;
}

BYTE PPU::readPPUDATA() {
	BYTE data;
	data = PPUDATA;
	this->dtaAddress += (this->PPUCTRL & 0x02);
	return data;
}

void PPU::readOAMDMA() {
	/*
	$4014. Writes done on CPU side.
	This port is located on the CPU. Writing $XX will upload 256 bytes of data from CPU page $XX00-$XXFF to the internal PPU OAM. 
	This page is typically located in internal RAM, commonly $0200-$02FF, but cartridge RAM or ROM can be used as well.
	*/
	readRegisters(4014);
}
