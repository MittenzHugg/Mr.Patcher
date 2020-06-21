/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   dk64_rom.cpp
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
#include "dk64_rom.h"

dk64_rom::dk64_rom(char* filename)
	:n64_rom(filename)
{
	if (gameID != supported_games_t::DONKEYKONG64_USA
		//TODO other versions
		) {
		delete this;
		throw "Not a recognized version of Donkey Kong 64";
		return;
	}

	_init();
}

dk64_rom::dk64_rom(n64_rom&& src)
	:n64_rom(std::move(src))
{
	if (gameID != supported_games_t::DONKEYKONG64_USA
		//TODO other versions
		) {
		delete this;
		throw "Not a recognized version of Donkey Kong 64";
		return;
	}

	_init();
}

dk64_rom::~dk64_rom()
{
}

void dk64_rom::_init(void)
{
	//77C - CRC Check 1
	//	788 - CRC Check 2

	//	1000->E8AC - Low - Level / N64 OS Functions(decompressed)
	_bootcode_span = buffer.slice(0x1000, 0xE8AC - 0x1000);

	//	EA88->F908 - Expansion Pak Pic
	_exp_pak_pic_span = buffer.slice(0xEA88, 0xF908 - 0xEA88);


	///*	-Assets and Assembly -
	//	113F0 - Globals Assembly Code
	_global_asm_span = buffer.slice(0x113F0, 0xC29D4 - 0x113F0);

	//	C29D4 - Globals Assets


	//	CBE70 - Assembly Code(Affects OKAY!, Main Menu(DONKEY KONG!), and shops)
	//	D4554 - Asset File(Affects OKAY!, Main Menu(DONKEY KONG!), and shops)
	//	D4B00 - Assembly Code(Multiplayer Mode)
	//	D69F8 - Asset File(Multiplayer Mode)
	//	D6B00 - Assembly Code(Minecart Rides)
	//	D98A0 - Asset File(MineCart Rides)
	//	D9A40 - Assembly Code(Rambi / Enguarde arena / Minigames)
	//	DF346 - Asset File(Rambi / Enguarde arena / Minigames)
	//	DF600 - Assembly Code(Slide / Car / Boat Race)
	//	E649A - Asset File(Slide / Car / Boat Race)
	//	E6780 - Assembly Code(Training Barrels, Tag Barrels, DK RAP)
	//	E9D17 - Asset File(Training Barrels, Tag Barrels, DK RAP)
	//	EA0B0 - Assembly Code(Boss)
	//	F388F - Asset File(Boss)
	//	F41A0 - Assembly Code(DK ARCADE Minigame)
	//	FB42C - Asset File(DK ARCADE Minigame)
	//	FD2F0 - Assembly Code(JETPAC Minigame)
	//	1010FD - Asset File(JETPAC Minigame)*/
	///*-Main Pointer Table -
	//	Initial Address : 0x101C50*/
	///*Textures for Test Map(how is size determined from ROM ? )
	//	119363C
	//	1193A6E - Test Wall Pallete
	//	1193AA4 - Test Map Floor*/
}
