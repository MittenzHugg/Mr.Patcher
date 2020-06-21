/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_file.h
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

#pragma once
#include <cstdint>
#include "simpTypes.h"
#include "n64_file.h"
#include "libdeflate.h"

typedef enum bk_file_types {
	ASM,
	ASM_VAR,
	MODEL,
	SPRITE,
	TEXT,
	LEVEL,
	LEVEL_MODEL,
	MUSIC,
	UNKNOWN,
} bk_file_type_t;

class bk_file: public n64_file {
public:
	uint32_t rom_offset;
	size_t comp_size;
	uint8_t* rom_buffer = nullptr;
	bool compressed;
	uint32_t ram_offset;
	size_t uncomp_size;
	uint8_t* uncomp_buffer = nullptr;
	bool modified;
	uint32_t mod_ROMOffset;
	size_t mod_comp_size;
	bk_file_type_t type;
	uint8_t* mod_buffer = nullptr;

	bk_file();
	bk_file(uint32_t offset, uint32_t size, uint8_t* rom_ptr, bk_file_type_t t, bool comp, uint32_t ram_off = 0);
	bk_file(const n64_span& rom_span, uint32_t ram_off, bool comp);
	~bk_file();
	
	uint8_t* decomp(void);
	uint8_t* comp(bool pad = false);
	void clear_buffers(void);
	void get_checksums(uint32_t* crc1, uint32_t* crc2);

protected:
	//TODO: relax compression?
	static libdeflate_compressor* comper;
	static libdeflate_decompressor* decomper;

	void _comp_method();
	void _decomp_method();
	void _checksum();
};
