/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   BK_project.cpp
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



#include "BK_project.h"
#include <iostream>
#include <fstream>
#include <filesystem>


void BK_project::save(const std::string& proj_file)
{
	std::ofstream ofile;
	ofile.open(proj_file, std::ios::out | std::ios::binary);
	
	u32 iput_size = (8 + _in_filename.length()) + (8 + sizeof(u64));
	u32 oput_size = 8 + _out_filename.length();

	//TODO assemble mods for size
	u32 mod_size = (8 + sizeof(u32));
	u32 mrp_size = (8 + iput_size) + (8 + oput_size) + (8 + mod_size); //+(8 + mods_size;)
	u32 cnk_size;

	ofile.write("Mr.P", 4);
	ofile.write(reinterpret_cast<const char*>(&mrp_size), sizeof(mrp_size));

	ofile.write("IPUT", 4);
	ofile.write(reinterpret_cast<const char*>(&iput_size), sizeof(iput_size));
	ofile.write("ipth", 4);
	cnk_size = _in_filename.length();
	ofile.write(reinterpret_cast<const char*>(&cnk_size), sizeof(cnk_size));
	ofile.write(_in_filename.c_str(), _in_filename.length());
	ofile.write("hash", 4);
	cnk_size = sizeof(u64);
	ofile.write(reinterpret_cast<const char*>(&cnk_size), sizeof(cnk_size));
	u64 bk_hash = 0xEFBEADDE;
	//u64 bk_hash = _BK->getHash();
	ofile.write(reinterpret_cast<const char*>(&bk_hash), sizeof(bk_hash));

	ofile.write("OPUT", 4);
	ofile.write(reinterpret_cast<const char*>(&iput_size), sizeof(iput_size));
	ofile.write("opth", 4);
	cnk_size = _out_filename.length();
	ofile.write(reinterpret_cast<const char*>(&cnk_size), sizeof(cnk_size));
	ofile.write(_out_filename.c_str(), _out_filename.length());

	ofile.write("MODS", 4);
	ofile.write(reinterpret_cast<const char*>(&mod_size), sizeof(mod_size));
	ofile.write("mcnt", 4);
	u32 mod_count = 0;
	ofile.write(reinterpret_cast<const char*>(&mod_count), sizeof(mod_count));
	//create unique tmp directory
	//save all patches and modified assets in directory with unique names
	//write project header file
		//open file
		/*
		Mr.P {
			IPUT{
				ipth: <string>
				hash:   <u64>
			}
			OPUT{
				opth: <string>
			}
			FILES{
				mcnt: <u32>
				{mcnk: {name:<string> type: id: size:<u32> data:{u8}}}
			}
		}*/
		//create zip file with .MrP extension


	ofile.close();
	
}

void BK_project::load(const std::string& proj_file)
{
	//look for files
	return;
}

std::vector<const MrP_Model*> BK_project::models()
{
	auto bk_models = _BK->models();
	std::vector<const MrP_Model*> rom_models(bk_models.size());
	rom_models.resize(bk_models.size());
	std::copy(bk_models.begin(), bk_models.end(), rom_models.begin());
	return rom_models;
}

