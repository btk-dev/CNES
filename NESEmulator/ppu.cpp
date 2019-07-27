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
	this->PC = this->cycle = 0;
}

void PPU::load_graphics(std::vector<BYTE> graphics)
{
	//only loads 1 bank of graphics. Only works with mapper 0 for now.
	for (int i = 0; i < graphics.size(); i++)
		this->memory[i] = graphics[i];
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
	case 1:
		//pre render
		//fill registers for initial visible scanlines
		//fetch nametable, attribute table, tile bitmap low and high
		//place into latches/registers, check for sprite zero hit, etc.
		//read memory accesses into latches, when in visible scanline these latches are moved into registers, these are then multiplexed before being written to screen.
		//currently no mirroring implemented below.
		//I don't think I need to actually have seperate variable fields for the nametables and the like.
		this->spriteLatch1 = this->memory[0x2000];
		this->spriteAttrib1 = this->memory[0x23C0];
		//increment VRAM address
		//fetch low order byte of an 8x1 pixel of pattern table $0000 - 0FF7 or $1000-1FF7
		//fetch high order from 8 bytes higher
		//turn attribute data and pattern data into palette indices, combine them with data from sprite data using priority
		if (this->cycle == 1) {
			this->vblank = 0;
			this->spriteCollision = false;
		}
		if (this->cycle > 280 && this->cycle < 341 && this->rendering) {
			//refresh vertical pixels
		}
		break;
	case 2:
		//visible
		break;
	case 3:
		//post render
		break;
	case 4:
		//vblank
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
