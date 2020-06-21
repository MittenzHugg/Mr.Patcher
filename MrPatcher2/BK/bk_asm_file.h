/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_asm_file.h
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
#include "n64_file.h"
#include "simpTypes.h"
#include "libdeflate.h"


enum bk_asm_f_t {
	boot_asm, core_1_asm, core_2_asm,
	mm_asm,	ttc_asm, cc_asm, bgs_asm, fp_asm, 
	lair_asm, gv_asm, ccw_asm, rbb_asm, mmm_asm, 
	sm_asm, fight_asm, cs_asm,
	exp_asm,
};

class bk_asm_file :
	public n64_file
{
public:
	
	const u32 ram_offset;

	bk_asm_file(bk_asm_f_t asm_id, const n64_span& rom_span, u32 ram_offset, bool compressed = true);
	bk_asm_file(bk_asm_f_t asm_id, u8* rom_ptr, u32 rom_offset, u32 size, u32 ram_offset, bool compressed = true);

	using n64_file::decompress;
	using n64_file::compress;

	const n64_span& code(void) const{ 
		if (_decomp_buffer == nullptr) {
			_decomp_method(); } 
		return _code;
	}
	const n64_span& data(void) const {
		if (_decomp_buffer == nullptr) {
			_decomp_method();
		}
		return _data;
	}
	const bk_asm_f_t id(void) const { return _asm_id; }
	const u32* const checksum(void) const{
		_checksum();
		return _crc;
	}

protected:
	const bk_asm_f_t _asm_id;
	static libdeflate_compressor* comper;
	static libdeflate_decompressor* decomper;

	mutable n64_span _code;
	mutable n64_span _data;
	mutable u32 _crc[2] = { 0,0 };

	void _comp_method(void) const;
	void _decomp_method(void) const;
	void _checksum(void) const;



};

