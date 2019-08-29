#include "bk_file.h"
#include <cstdlib>
#include <cstring>
#include "libdeflate.h"

bk_file::bk_file(void) {

}

bk_file::bk_file(uint32_t offset, uint32_t size, uint8_t* rom_ptr, bk_file_type_t t, bool comp, uint32_t ram_off) {
	rom_buffer = rom_ptr + offset;
	rom_offset = offset;
	comp_size = size;
	type = t;
	compressed = comp;

	ram_offset = ram_off;
	if (ram_off >= 0x80000000) {
		ram_offset -= 0x80000000;
	}
	uncomp_buffer = nullptr;
	uncomp_size = 0;

	modified = false;
	mod_buffer = nullptr;
	mod_ROMOffset = 0;
	mod_comp_size = 0;
}

bk_file::~bk_file() {

}

uint8_t* bk_file::decomp(void) {
	if (uncomp_buffer == nullptr) {
		uint8_t* tmpBuffer = nullptr;
		if (compressed) {
			uint8_t* tmpBuffer = (uint8_t*)malloc(0x400000);
			if (tmpBuffer == nullptr) return nullptr;
			//setup decompressor
			struct libdeflate_decompressor* decomper = libdeflate_alloc_decompressor();

			//decompress
			enum libdeflate_result decompResult = libdeflate_deflate_decompress_ex(decomper,
				rom_buffer + 0x06,
				comp_size,
				tmpBuffer,
				0x400000,
				&(comp_size),
				&(uncomp_size)
			);
			comp_size += 0x06;
			uncomp_buffer = (uint8_t*)malloc(uncomp_size);
			if (uncomp_buffer == nullptr) return nullptr;
			memcpy(uncomp_buffer, tmpBuffer, uncomp_size);
			free(tmpBuffer);
			libdeflate_free_decompressor(decomper);
		}
		else {
			//non-compressed file
			uncomp_size = comp_size;
			uncomp_buffer = (uint8_t*)malloc(uncomp_size);
			if (uncomp_buffer == nullptr) return nullptr;
			memcpy(uncomp_buffer, rom_buffer, uncomp_size);
		}
	}
	return uncomp_buffer;
}

uint8_t* bk_file::comp(bool pad) {
	if (uncomp_buffer != nullptr) {
		if (compressed) {
			//TODO: relax compression?
			struct libdeflate_compressor* comper = libdeflate_alloc_compressor(12);
			uint8_t* tmp_buffer = (uint8_t*)malloc(sizeof(uint8_t) * comp_size);
			mod_comp_size = libdeflate_deflate_compress(comper, uncomp_buffer, uncomp_size, tmp_buffer, comp_size);
			if (mod_comp_size == 0) {
				return nullptr;
			};

			//add bk_header
			mod_comp_size += 0x06;
			uint32_t padding = (mod_comp_size % 8);
			if (pad) {
				if (padding != 0) {
					mod_comp_size += 8 - padding;
				}
			}
			if (mod_buffer != nullptr) {
				free(mod_buffer); 
				mod_buffer = nullptr;
			}
			mod_buffer = (uint8_t*)calloc(mod_comp_size, sizeof(uint8_t));
			if (mod_buffer == nullptr) return nullptr;
			memcpy(mod_buffer + 0x06, tmp_buffer, mod_comp_size - 0x06);
			free(tmp_buffer);
			mod_buffer[0] = 0x11;
			mod_buffer[1] = 0x72;
			for (int i = 0; i < 4; i++) {
				if (!pad) {
					mod_buffer[2 + i] = (uint8_t)(((mod_comp_size - 0x06) >> ((3 - i) * 8)) & 0xFF);
				}
				else {
					mod_buffer[2 + i] = (uint8_t)(((mod_comp_size - padding - 0x06) >> ((3 - i) * 8)) & 0xFF);
				}
			}
			return mod_buffer;
		}
		else {
			mod_comp_size = uncomp_size;
			mod_buffer = (uint8_t*)malloc(mod_comp_size);
			if (mod_buffer == nullptr) return nullptr;
			memcpy(mod_buffer, uncomp_buffer, mod_comp_size);
			return mod_buffer;
		}
	}
	else { return nullptr; }
}

void bk_file::clear_buffers(void) {
	if (uncomp_buffer != nullptr) {
		free(uncomp_buffer);
		uncomp_buffer = nullptr;
	}
	if (mod_buffer != nullptr) {
		free(mod_buffer);
		mod_buffer = nullptr;
	}

}