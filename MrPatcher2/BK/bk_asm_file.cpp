/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_asm_file.cpp
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

#include <cstring>
#include "bk_asm_file.h"
#include "libdeflate.h"
#include <stdio.h>
#include <direct.h>



libdeflate_compressor* bk_asm_file::comper = libdeflate_alloc_compressor(12); 			//TODO: relax compression?
libdeflate_decompressor* bk_asm_file::decomper = libdeflate_alloc_decompressor();


bk_asm_file::bk_asm_file(bk_asm_f_t asm_id, const n64_span& rom_span, u32 ram_offset, bool compressed)
	:n64_file(rom_span, compressed)
	, ram_offset{ ram_offset & 0x7FFFFFFF }
	, _asm_id{ asm_id }
{
	if (!compressed) {
		_code = *_decomp_buffer;
		_data = *_decomp_buffer;
	}
}

bk_asm_file::bk_asm_file(bk_asm_f_t asm_id, u8* rom_ptr, u32 rom_offset, u32 size, u32 ram_offset, bool compressed)
	: n64_file(rom_ptr, rom_offset, size, compressed)
	, ram_offset{ ram_offset & 0x7FFFFFFF }
	, _asm_id{asm_id}
{}



void bk_asm_file::_comp_method(void) const{
	u8* tmpBuffer = (u8*)calloc(_decomp_buffer->size() + 0xC, sizeof(u8));
	if (tmpBuffer == nullptr)
		throw "Bad memory allocation";

	n64_span tmpCodeSpan = n64_span(tmpBuffer, tmpBuffer + _decomp_buffer->size() + 0x0C);

	//CODE
	tmpCodeSpan.set<u16>(0x1172);
	u32 outsize = libdeflate_deflate_compress(comper, _code.begin(), _code.size(), tmpCodeSpan.begin() + 0x06, tmpCodeSpan.size() - 0x06);
	if (outsize == 0)
		throw "Unable to compress code buffer.";
	tmpCodeSpan.set<u32>(outsize, 0x02);

	size_t code_comp_size = outsize + 0x06;
	if (u32 padding = (code_comp_size % 8)) {
		code_comp_size += 8 - padding;
	}

	//DATA
	n64_span tmpDataSpan = n64_span(tmpBuffer + code_comp_size, tmpCodeSpan.size() - code_comp_size);
	tmpDataSpan.set<u16>(0x1172);
	outsize = libdeflate_deflate_compress(comper, _data.begin(), _data.size(), tmpDataSpan.begin() + 0x06, tmpDataSpan.size() - 0x06);
	if (outsize == 0)
		throw "Unable to compress data buffer.";
	tmpDataSpan.set<u32>(outsize, 0x02);
	size_t data_comp_size = outsize + 0x06;
	
	tmpBuffer = (u8*)realloc(tmpBuffer,code_comp_size + data_comp_size);
	if(tmpBuffer == nullptr)
		throw "Bad memory reallocation";

	_comp_buffer = new n64_span(tmpBuffer, (size_t)code_comp_size + data_comp_size);
	_comp_own = true;

	return;
}

void bk_asm_file::_decomp_method(void) const {
	uint8_t* tmpBuffer = (uint8_t*)malloc(0x400000);
	if (tmpBuffer == nullptr) 
		throw "Bad memory allocation.";
	size_t tmpCompSize;
	size_t codeSize = 0;
	size_t dataSize = 0;
	size_t codeCompSize;
	size_t dataCompSize;

	codeCompSize = _comp_buffer->size();

	//decompress Code
	enum libdeflate_result decompResult = libdeflate_deflate_decompress_ex( decomper, _comp_buffer->begin() + 6, _comp_buffer->size() - 6
		                                                                  , tmpBuffer, 0x400000, &codeCompSize, &codeSize);
	//decompress data
	codeCompSize += 6;
	decompResult = libdeflate_deflate_decompress_ex( decomper, _comp_buffer->begin() + codeCompSize + 6, _comp_buffer->size() - codeCompSize - 6
												   , tmpBuffer + codeSize, 0x400000 - codeSize, &dataCompSize, &dataSize);
	tmpBuffer = (uint8_t *)realloc(tmpBuffer, codeSize + dataSize);
	if (tmpBuffer == nullptr) 
		throw "Bad memory reallocation.";
	_decomp_buffer = new n64_span(tmpBuffer, codeSize + dataSize);
	_decomp_own = true;

	_code = _decomp_buffer->slice(0, codeSize);
	_data = _decomp_buffer->slice(codeSize, dataSize);
	return;
}

void bk_asm_file::_checksum(void) const {
	s32 val = 0;
	s32 crc = 0xFFFFFFFF;
	for (u32 i = 0; i < decompress().size(); i++) {
		s32 byte = (s32)(decompress()[i]);
		val += byte;
		crc ^= (byte << (val & 0x17));
	}
	return;
}