#include "memory.h"
#include "mbc.h"

#include "../graphics.h"
#include "../interrupts.h"
#include "../bits.h"
#include "../sound.h"
#include "../sprite_priorities.h"

#include "../../non_core/joypad.h"
#include "../../non_core/logger.h"
#include <stdio.h>
#include <string.h>

  
// 0xFF00 - 0xFFFF
uint8_t dmg_io_mem[256]= {
		0xCF, 0x00, 0x7E, 0xFF, 0xD3, 0x00, 0x00, 0xF8,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE1,
		0x80, 0xBF, 0xF3, 0xFF, 0xBF, 0xFF, 0x3F, 0x00,
		0xFF, 0xBF, 0x7F, 0xFF, 0x9F, 0xFF, 0xBF, 0xFF,
		0xFF, 0x00, 0x00, 0xBF, 0x77, 0xF3, 0xF1, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x71, 0x72, 0xD5, 0x91, 0x58, 0xBB, 0x2A, 0xFA,
		0xCF, 0x3C, 0x54, 0x75, 0x48, 0xCF, 0x8F, 0xD9,
		0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFC,
		0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x2B, 0x0B, 0x64, 0x2F, 0xAF, 0x15, 0x60, 0x6D,
		0x61, 0x4E, 0xAC, 0x45, 0x0F, 0xDA, 0x92, 0xF3,
		0x83, 0x38, 0xE4, 0x4E, 0xA7, 0x6C, 0x38, 0x58,
		0xBE, 0xEA, 0xE5, 0x81, 0xB4, 0xCB, 0xBF, 0x7B,
		0x59, 0xAD, 0x50, 0x13, 0x5E, 0xF6, 0xB3, 0xC1,
		0xDC, 0xDF, 0x9E, 0x68, 0xD7, 0x59, 0x26, 0xF3,
		0x62, 0x54, 0xF8, 0x36, 0xB7, 0x78, 0x6A, 0x22,
		0xA7, 0xDD, 0x88, 0x15, 0xCA, 0x96, 0x39, 0xD3,
		0xE6, 0x55, 0x6E, 0xEA, 0x90, 0x76, 0xB8, 0xFF,
		0x50, 0xCD, 0xB5, 0x1B, 0x1F, 0xA5, 0x4D, 0x2E,
		0xB4, 0x09, 0x47, 0x8A, 0xC4, 0x5A, 0x8C, 0x4E,
		0xE7, 0x29, 0x50, 0x88, 0xA8, 0x66, 0x85, 0x4B,
		0xAA, 0x38, 0xE7, 0x6B, 0x45, 0x3E, 0x30, 0x37,
		0xBA, 0xC5, 0x31, 0xF2, 0x71, 0xB4, 0xCF, 0x29,
		0xBC, 0x7F, 0x7E, 0xD0, 0xC7, 0xC3, 0xBD, 0xCF,
		0x59, 0xEA, 0x39, 0x01, 0x2E, 0x00, 0x69, 0x00
};

           
    
// OAM Ram 0xFE00 - 0xFE9F
uint8_t dmg_oam_mem[0xFF] = {
    0xBB, 0xD8, 0xC4, 0x04, 0xCD, 0xAC, 0xA1, 0xC7,
    0x7D, 0x85, 0x15, 0xF0, 0xAD, 0x19, 0x11, 0x6A,
    0xBA, 0xC7, 0x76, 0xF8, 0x5C, 0xA0, 0x67, 0x0A,
    0x7B, 0x75, 0x56, 0x3B, 0x65, 0x5C, 0x4D, 0xA3,
    0x00, 0x05, 0xD7, 0xC9, 0x1B, 0xCA, 0x11, 0x6D,
    0x38, 0xE7, 0x13, 0x2A, 0xB1, 0x10, 0x72, 0x4D,
    0xA7, 0x47, 0x13, 0x89, 0x7C, 0x62, 0x5F, 0x90,
    0x64, 0x2E, 0xD3, 0xEF, 0xAB, 0x01, 0x15, 0x85,
    0xE8, 0x2A, 0x6E, 0x4A, 0x1F, 0xBE, 0x49, 0xB1,
    0xE6, 0x0F, 0x93, 0xE2, 0xB6, 0x87, 0x5D, 0x35,
    0xD8, 0xD4, 0x4A, 0x45, 0xCA, 0xB3, 0x33, 0x74,
    0x18, 0xC1, 0x16, 0xFB, 0x8F, 0xA4, 0x8E, 0x70,
    0xCD, 0xB4, 0x4A, 0xDC, 0xE6, 0x34, 0x32, 0x41,
    0xF9, 0x84, 0x6A, 0x99, 0xEC, 0x92, 0xF1, 0x8B,
    0x5D, 0xA5, 0x09, 0xCF, 0x3A, 0x93, 0xBC, 0xE0,
    0x15, 0x19, 0xE4, 0xB6, 0x9A, 0x04, 0x3B, 0xC1,
    0x96, 0xB7, 0x56, 0x85, 0x6A, 0xAA, 0x1E, 0x2A,
    0x80, 0xEE, 0xE7, 0x46, 0x76, 0x8B, 0x0D, 0xBA,
    0x24, 0x40, 0x42, 0x05, 0x0E, 0x04, 0x20, 0xA6,
    0x5E, 0xC1, 0x97, 0x7E, 0x44, 0x05, 0x01, 0xA9
};


/*  Gameboy bootstrap ROM for startup.
 *  Modified from the original so that the CPU doesn't
 *  hang if the ROM checksum checksum is incorrect */
static uint8_t const dmg_boot_rom[0x100] = {
  0x31, 0xfe, 0xff, 0xaf, 0x21, 0xff, 0x9f, 0x32, 0xcb, 0x7c, 0x20, 0xfb,
  0x21, 0x26, 0xff, 0x0e, 0x11, 0x3e, 0x80, 0x32, 0xe2, 0x0c, 0x3e, 0xf3,
  0xe2, 0x32, 0x3e, 0x77, 0x77, 0x3e, 0xfc, 0xe0, 0x47, 0x11, 0x04, 0x01,
  0x21, 0x10, 0x80, 0x1a, 0xcd, 0x95, 0x00, 0xcd, 0x96, 0x00, 0x13, 0x7b,
  0xfe, 0x34, 0x20, 0xf3, 0x11, 0xd8, 0x00, 0x06, 0x08, 0x1a, 0x13, 0x22,
  0x23, 0x05, 0x20, 0xf9, 0x3e, 0x19, 0xea, 0x10, 0x99, 0x21, 0x2f, 0x99,
  0x0e, 0x0c, 0x3d, 0x28, 0x08, 0x32, 0x0d, 0x20, 0xf9, 0x2e, 0x0f, 0x18,
  0xf3, 0x67, 0x3e, 0x64, 0x57, 0xe0, 0x42, 0x3e, 0x91, 0xe0, 0x40, 0x04,
  0x1e, 0x02, 0x0e, 0x0c, 0xf0, 0x44, 0xfe, 0x90, 0x20, 0xfa, 0x0d, 0x20,
  0xf7, 0x1d, 0x20, 0xf2, 0x0e, 0x13, 0x24, 0x7c, 0x1e, 0x83, 0xfe, 0x62,
  0x28, 0x06, 0x1e, 0xc1, 0xfe, 0x64, 0x20, 0x06, 0x7b, 0xe2, 0x0c, 0x3e,
  0x87, 0xe2, 0xf0, 0x42, 0x90, 0xe0, 0x42, 0x15, 0x20, 0xd2, 0x05, 0x20,
  0x4f, 0x16, 0x20, 0x18, 0xcb, 0x4f, 0x06, 0x04, 0xc5, 0xcb, 0x11, 0x17,
  0xc1, 0xcb, 0x11, 0x17, 0x05, 0x20, 0xf5, 0x22, 0x23, 0x22, 0x23, 0xc9,
  0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b, 0x03, 0x73, 0x00, 0x83,
  0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f, 0x88, 0x89, 0x00, 0x0e,
  0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99, 0xbb, 0xbb, 0x67, 0x63,
  0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f, 0xbb, 0xb9, 0x33, 0x3e,
  0x3c, 0x42, 0xb9, 0xa5, 0xb9, 0xa5, 0x42, 0x3c, 0x21, 0x04, 0x01, 0x11,
  0xa8, 0x00, 0x1a, 0x13, 0xbe, 0x20, 0xfe, 0x23, 0x7d, 0xfe, 0x34, 0x20,
  0xf5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 0xfb, 0x86, 0x00, 0x00,
  0x3e, 0x01, 0xe0, 0x50
};


/* Store first 255 bytes of cartridge so it can
   be restored after boot room has finished */
static uint8_t cartridge_start[0x100];


int load_rom(char const *filename, unsigned char const *file_data, size_t size) {

    /* Check the file length given to us is long enough
     * to obtain what the size of the file should be */   
    if (size < CARTRIDGE_ROM_SIZE + 1) {
        log_message(LOG_ERROR, "Error: Cartridge size is too small (%lu bytes)\n",size);
        return 0;
    }

    // Obtain rom size in KB
    size_t rom_size = id_to_rom_size(file_data[CARTRIDGE_ROM_SIZE]) * 1024;

    // Data read in doesn't match header information
    if (size != rom_size) {
        log_message(LOG_ERROR, "Error: Cartridge header info on its size (%lu bytes) \
            doesn't match file size (%lu bytes)\n",rom_size, size);
        return 0;
    }

    // Dump ROM into memory banks
    unsigned rom_bank_count = rom_size / 0x4000;
      
    for (unsigned n = 0; n < rom_bank_count; n++) {
        memcpy(ROM_banks[n], file_data + (0x4000 * n), 0x4000);
    }
    
    /* Copy first 100 bytes of cartridge before
     * it is overwritten by the boot rom so we can restore
     *  it later */
    memcpy(cartridge_start, ROM_banks[0],  0x100);
    memcpy(ROM_banks[0], dmg_boot_rom, 0x100);
	
	/*Copy original GB starting IO and OAM mem values into
	  IO and OAM mem */
	memcpy(io_mem, dmg_io_mem, 0x100);
	memcpy(oam_mem, dmg_oam_mem, 0x100);

    // Setup the memory bank controller 
    if(!setup_MBC(file_data[CARTRIDGE_TYPE], 
        (id_to_ram_save_size(file_data[CARTRIDGE_RAM_SIZE]) * 0x400) / 0x2000, 
        filename)) {
        return 0;
    }
    
    return 1;
} 



/* Restore first 255 bytes of memory
 * with first 255 bytes of the cartridge */
void unload_boot_rom() {
    memcpy(ROM_banks[0], cartridge_start, 0x100);
}




/* Transfer 160 bytes to sprite memory starting from
 * address XX00 */
void dma_transfer(uint8_t val) {

    uint16_t source_addr = val << 8;
    for (int i = 0; i < 0xA0; i++) {
        oam_mem[i] = get_mem(source_addr + i);
    }
}


/* Keypad is written to, update register with state */
void joypad_write(uint8_t joypad_state) {
    
    joypad_state |= 0xF; // unset all keys

    // Check directional keys
    if (joypad_state & BIT_5) {
        joypad_state &= ~(down_pressed() << 3);
        joypad_state &= ~(up_pressed() << 2);
        joypad_state &= ~(left_pressed() << 1);
        joypad_state &= ~(right_pressed());
    } 
    // Check other 4 keys
    else if (joypad_state & BIT_4) {
        joypad_state &= ~(start_pressed() << 3);
        joypad_state &= ~(select_pressed() << 2);
        joypad_state &= ~(b_pressed() << 1);
        joypad_state &= ~(a_pressed());
    }
    
    /* Raise joypad interrupt if a key
     * was pressed */
    if (!(joypad_state & 0xF)) {
        raise_interrupt(JOYPAD_INT);
    }
    io_mem[GLOBAL_TO_IO_ADDR(P1_REG)] = joypad_state;
}




/* Directly inject a value into IO memory without performing
 * any checks or operations on the data. Should be used by
 * controllers that have direct access to modifying this memory
 * and not the CPU. */
void io_write_override(uint8_t addr, uint8_t val) {
   io_mem[addr] = val;
}

/* Write to IO memory given address 0 - 0xFF */
void io_write_mem(uint8_t addr, uint8_t val) {


	io_mem[addr] = val;
	uint16_t global_addr = addr + 0xFF00;
	if (global_addr >= 0xFF10 && global_addr <= 0xFF3F) {
		write_apu(global_addr, val);
		return;
	}
	switch (global_addr) {

	case P1_REG: joypad_write(val); break;

		/*  Attempting to set DIV reg resets it to 0 */
	case DIV_REG: io_mem[addr] = 0; break; //io_mem[addr] = 0; break;
		/*  Attempting to set LY reg resets it to 0  */
	case LY_REG: io_mem[addr] = 0; break;
		/*  Perform direct memory transfer  */
	case DMA_REG: dma_transfer(val); break;
	}
}

/* Write to OAM given OAM address 0x0 - 0xA0
* Does nothing if address > 0xA0 */
void oam_set_mem(uint8_t addr, uint8_t val) {

	// Check not unusable RAM (i.e. not 0xFEA0 - 0xFEFF)
	if (addr < 0xA0) {
		oam_mem[addr] = val;
		/* If Object X position is written to, reorganise
		* sprite priorities for rendering */
		if ((addr - 1) % 4 == 0) {
			update_sprite_prios(addr / 4, val);
		}
	}
}


/* Read from OAM given OAM address 0 - A0
* Returns 0x0 if addres > 0xA0 */
uint8_t oam_get_mem(uint8_t addr) {
	//Check not unusable RAM (i.e. not 0xFEA0 - 0xFEFF)
	return oam_mem[addr];
}


/*  Write an 8 bit value to the given 16 bit address */
void set_mem(uint16_t const addr, uint8_t const val) {

	//Check if memory bank controller chip is being accessed 
	if (addr < 0x8000 || ((uint16_t)(addr - 0xA000) < 0x2000)) {
		write_MBC(addr, val);
		return;
	}
	// Write to "ordinary" memory (0x8000 - 0xFDFF)
	if (addr < 0xFE00) {
		mem[addr] = val;
		//  Check if mirrored memory being written to

		// 0xE000 - 0xFDFF, repeat to 0xC000 - 0xDDFF
		if ((uint16_t)(addr - ECHO_RAM_START) < 0x1DFF) {
			mem[addr - 0x2000] = val;
			// 0xC000 - 0xDDFF, repeat to 0xE000 - 0xFDFF
		}
		else if ((uint16_t)(addr - ECHO_RAM_START - 0x2000) < 0x1DFF) {
			mem[addr + 0x2000] = val;
		}
		return;
	}
	// Write to Object Attribute Table (0xFE00 - 0xFEFF)
	if ((uint16_t)(addr - 0xFE00) < 0x100) {
		oam_set_mem(addr - 0xFE00, val);
		return;
	}
	/*  IO being written to */
	if (addr >= 0xFF00) {
		// Boot ROM finished, reload start of cartridge
		if (addr == 0xFF50 && val == 1) {
			unload_boot_rom();
		}
		io_write_mem(addr - 0xFF00, val);
	}
}


// Read contents from given 16 bit memory address
uint8_t get_mem(uint16_t const addr) {

	// Check if reading from Memory Bank Controller
	if (addr < 0x8000 || ((uint16_t)(addr - 0xA000) < 0x2000)) {
		return read_MBC(addr);
	}
	// Read from "ordinary" memory (0x8000 - 0xFDFF)
	if (addr < 0xFE00) {
		return mem[addr];
	}
	// Read from Object Attribute Table (0xFE00 - 0xFEFF) 
	if ((uint16_t)(addr - 0xFE00) < 0x100) {
		return oam_get_mem(addr - 0xFE00);;
	}
	// Read from IO mem
	if (addr >= 0xFF10 && addr <= 0xFF3F) {
		return read_apu(addr);
	}
	return io_mem[addr - 0xFF00];

}



/* Write 16bit value starting at the given memory address 
 * into memory.  Written in little-endian byte order */
void set_mem_16(uint16_t loc, uint16_t val) {
    set_mem(loc + 1, val >> 8);
    set_mem(loc, val & 0xFF);
}


/* Read contents of 2 memory locations starting at the
 * given address. Returned as little-endian byte order 16 bit value */
uint16_t get_mem_16(uint16_t loc) {
    return (get_mem(loc + 1) << 8) |
            get_mem(loc);
}

