/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   n64_rom.cpp
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
#include "n64_rom.h"
#include "N64\md5.h"
#include <stdio.h>
#include <cstring>
#include <string>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include "bk_rom.h"



n64_rom::n64_rom(const char* fileName)
{
	FILE* ROMFile;
	//QErrorMessage errMsg();
	int err = 0;
	if (err = fopen_s(&ROMFile, fileName, "rb")) {
		//errMsg->showMessage(tr("ERROR: Can not open %s").arg(fileName));
		return;
	}

	//load ROM into memory
	fseek(ROMFile, 0, SEEK_END);
	size_t size = ftell(ROMFile);
	this->_buffer = (uint8_t*)malloc(size);
	if (this->_buffer == nullptr) {
		//TODO: Error Msg
		return;
	}
	rewind(ROMFile);
	fread(this->_buffer, sizeof(uint8_t), size, ROMFile);
	buffer = n64_span(_buffer, size);

	//correct endianess
	u32 ROMSignature = buffer.get<u32>();
	switch (ROMSignature)
	{
	case N64_SIG:
		for (int i = 0; i < size; i += 4) {
			uint8_t tmp;
			tmp = this->_buffer[i];
			this->_buffer[i] = this->_buffer[i + 3];
			this->_buffer[i + 3] = tmp;
			tmp = this->_buffer[i + 1];
			this->_buffer[i + 1] = this->_buffer[i + 2];
			this->_buffer[i + 2] = tmp;
		}
		break;

	case V64_SIG:
		for (int i = 0; i < size; i += 2) {
			uint8_t tmp;
			tmp = this->_buffer[i];
			this->_buffer[i] = this->_buffer[i + 1];
			this->_buffer[i + 1] = tmp;
		}
		break;

	case Z64_SIG:
		//nothing to do
		break;
	default:
		delete this;
		throw "Unknown File Format";
		break;
	}


	//perform md5 hash to verify the ROM
	md5_state_t ROM_md5_state;
	md5_byte_t ROM_md5_digest[16];
	md5_init(&ROM_md5_state);
	md5_append(&ROM_md5_state, (const md5_byte_t*)this->_buffer, size);
	md5_finish(&ROM_md5_state, ROM_md5_digest);
	_hash = n64_be<u64>::get(n64_span(ROM_md5_digest,(size_t) 16 ),0);
	

	const std::map<u64, supported_games_t> hash_ID_map = {
		{BK_USA_HASH, BANJOKAZOOIE_NTSC},
		{BK_PAL_HASH, BANJOKAZOOIE_PAL},
		{BK_JP_HASH, BANJOKAZOOIE_JP},
		{BK_USA_REV1_HASH, BANJOKAZOOIE_NTSC_REV1},
		{DK64_USA_HASH, DONKEYKONG64_USA}
	};

	switch (_hash)
	{
	case BK_USA_HASH:
		this->gameID = BANJOKAZOOIE_NTSC;
		break;
	case BK_PAL_HASH:
		this->gameID = BANJOKAZOOIE_PAL;
		break;
	case BK_JP_HASH:
		this->gameID = BANJOKAZOOIE_JP;
		break;
	case BK_USA_REV1_HASH:
		this->gameID = BANJOKAZOOIE_NTSC_REV1;
		break;
	case DK64_USA_HASH:
		this->gameID = DONKEYKONG64_USA;
		break;
	default:
		this->gameID = UNKNOWN_GAME;
		break;
	}
	return;
}

n64_rom::n64_rom(n64_rom&& src)
	: gameID{src.gameID}
	, buffer{src.buffer}
	, _buffer{src._buffer}
	, _hash{src._hash}
{
	src._buffer = nullptr;
}

n64_rom::~n64_rom()
{
	if (_buffer != nullptr) {
		free(_buffer);
		_buffer = nullptr;
	}
}
