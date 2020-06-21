/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   gs_parser.h
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

typedef struct gs_byte gs_byte_t;
struct gs_byte {
	uint32_t addr;
	uint8_t val;
	gs_byte_t * next;
};

struct gs_c {
	bool  len;
	u32 addr;
	u16 val;
};
class gs_parser
{
public:
	gs_parser(char* filename);
	~gs_parser();

	int get_byte(uint32_t* ram_addr, uint8_t* value);

private:
	gs_byte_t* byte_list;
};

