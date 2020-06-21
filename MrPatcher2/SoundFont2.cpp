/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   SoundFont2.cpp
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
#include "SoundFont2.h"
#include <fstream>

void SoundFont2::FromN64(const std::string oFilename, const ALBankFile& n64Bank)
{
	std::ofstream ofile;
	ofile.open(oFilename, std::ios::binary | std::ios::out);
	//generate LISTS
	ofile.write("RIFF", 4);
	//TODO get RIFF size
	u32 riff_size = 0x042069;
	ofile.write(reinterpret_cast<const char*>(&riff_size), sizeof(u32));
	ofile.write("sfbk", 4);
	
	
	//write <INFO-list>
	ofile.write("LIST", 4);
	//u32 riff_size = 0x042069;
	ofile.write(reinterpret_cast<const char*>(&riff_size), sizeof(u32));
	ofile.close();
}


const u16		  SF2_INFO::ifil_data[2] = { 2,1 };
const std::string SF2_INFO::isng_data = "EMU8000";
const std::string SF2_INFO::INAM_data = "Unnamed";

size_t SF2_INFO::size(void)
{
	size_t out_size = 2 * sizeof(u16);
	out_size += isng_data.size() + isng_data.size()%16;
	out_size += INAM_data.size() + isng_data.size()%16;
	return out_size;
}

void SF2_INFO::write(std::ofstream& ofile)
{
	ofile.write("LIST", 4);
	u32 list_size = this->size();
	ofile.write(reinterpret_cast<const char*>(&list_size), sizeof(u32));
	ofile.write("INFO", 4);
	ofile.write("ifil", 4);   //<ifil - ck>; Refers to the version of the Sound Font RIFF file
}
