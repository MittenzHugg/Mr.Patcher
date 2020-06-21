/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   SoundFont2.h
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
#include <string>
#include <ALBankFile.h>
#include <fstream>

class SF2_ck {
public:
	//SF2_ck(const std::string& ID, const std::iterator<>std::iterator_);
	//SF2_ck(const std::string& ID, const std::string& DATA);
	void write(std::ofstream& ofile);
	size_t size();
	
private:
	std::string ckID;
	u32 ckSize;
	std::vector<u8> ckDATA;
};

class SF2_INFO
{
public:
	void write(std::ofstream& ofile);
	size_t size();

private:
	static const u16 ifil_data[2];
	static const std::string isng_data; //<isng - ck>; Refers to the target Sound Engine
	static const std::string INAM_data; //<INAM - ck>; Refers to the Sound Font Bank Name

	//  [<irom - ck>] ; Refers to the Sound ROM Name
	//	[<iver - ck>]; Refers to the Sound ROM Version
	//	[<ICRD - ck>]; Refers to the Date of Creation of the Bank
	//	[<IENG - ck>]; Sound Designersand Engineers for the Bank
	//	[<IPRD - ck>]; Product for which the Bank was intended
	//	[<ICOP - ck>]; Contains any Copyright message
	//	[<ICMT - ck>]; Contains any Comments on the Bank
	//	[<ISFT - ck>]; The SoundFont tools used to createand alter the bank
};

class SoundFont2
{
public:
	static void FromN64(const std::string oFilename, const ALBankFile& n64Bank);

private:
};

