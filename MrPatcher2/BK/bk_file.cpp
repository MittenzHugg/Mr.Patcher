/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_file.cpp
*
*   This file is part of Mr.Patcher.
*
*   Mr.Patcher is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   Mr.Patcher is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Mr.Patcher.  If not, see <https://www.gnu.org/licenses/>.
*************************************************************************/

#include "bk_file.h"
#include <cstdlib>
#include <cstring>
#include "libdeflate.h"


libdeflate_compressor* bk_file::comper = libdeflate_alloc_compressor(12); 			//TODO: relax compression?
libdeflate_decompressor* bk_file::decomper = libdeflate_alloc_decompressor();

bk_file::bk_file(void) {
}

bk_file::bk_file(const n64_span& rom_span, uint32_t ram_off, bool comp)
	: n64_file(rom_span, comp)
{
	rom_buffer = rom_span.begin();
	rom_offset = 0;
	comp_size = rom_span.size();
	type = bk_file_types::ASM;
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

bk_file::bk_file(uint32_t offset, uint32_t size, uint8_t* rom_ptr, bk_file_type_t t, bool comp, uint32_t ram_off)
	: n64_file((u8*) rom_ptr, offset, size, comp) 
{
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
	clear_buffers();
}

uint8_t* bk_file::decomp(void) {
	return (uint8_t*)(decompress().begin());
}

void bk_file::get_checksums(uint32_t * crc1, uint32_t * crc2) {
	const n64_span d = decompress();
	if (d.begin() != nullptr) {
		int32_t val = 0;
		int32_t crc = 0xFFFFFFFF;
		for (uint32_t i = 0; i < d.size(); i++) {
			int32_t byte = (int32_t)d[i];
			val = val + byte;
			crc = crc ^ (byte << (val & 0x17));
		}
		*crc1 = (uint32_t)val;
		*crc2 = (uint32_t)crc;
	}
	return;
}

uint8_t* bk_file::comp(bool pad) {
	if (uncomp_buffer != nullptr) {
		if (compressed) {
			return mod_buffer;
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

void bk_file::_comp_method() {
	uint8_t* tmp_buffer = (uint8_t*)malloc(sizeof(uint8_t) * comp_size);
	mod_comp_size = libdeflate_deflate_compress(comper, _decomp_buffer->begin(), _decomp_buffer->size(), tmp_buffer, comp_size);
	if (mod_comp_size == 0) return;

	//add bk_header
	mod_comp_size += 0x06;
	uint32_t padding = (mod_comp_size % 8);
	if (mod_buffer != nullptr) {
		free(mod_buffer);
		mod_buffer = nullptr;
	}
	mod_buffer = (uint8_t*)calloc(mod_comp_size, sizeof(uint8_t));
	if (mod_buffer == nullptr) return;
	memcpy(mod_buffer + 0x06, tmp_buffer, mod_comp_size - 0x06);
	free(tmp_buffer);
	_comp_own = true;
	_comp_buffer = new n64_span((u8*)mod_buffer, (size_t)mod_comp_size);
	_comp_buffer->set((u16)0x1172);
	_comp_buffer->set((u32) mod_comp_size - 0x06, 0x02);
	return;
}

void bk_file::_decomp_method() {
	uint8_t* tmpBuffer = (uint8_t*)malloc(0x400000);
	if (tmpBuffer == nullptr) return;
	size_t tmpCompSize;
	size_t tmpDecompSize = 0;

	//decompress
	enum libdeflate_result decompResult = libdeflate_deflate_decompress_ex( decomper, _comp_buffer->begin(), _comp_buffer->size(), tmpBuffer, 0x400000, &tmpCompSize,	&tmpDecompSize);
	tmpCompSize += 0x06;
	u8* tmpBuffer2 = (u8*)malloc(tmpDecompSize);
	if (tmpBuffer2 == nullptr) return;
	_decomp_buffer = new n64_span(tmpBuffer2, tmpDecompSize);
	_decomp_own = true;
	memcpy(_decomp_buffer->begin(), tmpBuffer, tmpDecompSize);
	free(tmpBuffer);
	uncomp_buffer = (uint8_t*)_decomp_buffer->begin();
	return;
}

void bk_file::_checksum() {
	return;
}