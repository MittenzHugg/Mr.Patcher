/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   MrP_project.cpp
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

#include "MrP_project.h"
#include "BK_project.h"

MrP_project* MrP_project::new_proj(std::string romFilePath)
{
	n64_rom* new_rom = new n64_rom(romFilePath.c_str());
	//new n64_rom easiest way to correct endianess and obtain file hash.

	MrP_project* new_proj = nullptr;
	switch (new_rom->getHash()) {
	case BK_USA_HASH: [[fallthrough]];
	case BK_PAL_HASH: [[fallthrough]];
	case BK_JP_HASH:  [[fallthrough]];
	case BK_USA_REV1_HASH:
		new_proj = new BK_project( std::move(*new_rom) ); //move constructor
		break;
	default:
		throw "Unrecognized ROM";
		break;
	}
	if (new_proj != nullptr)
		new_proj->setInFilePath(romFilePath);
	return new_proj;
}
