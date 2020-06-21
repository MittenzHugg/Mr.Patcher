/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   dk64_asset.h
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
#include "n64_file.h"
#include "libdeflate.h"
#include <vector>

class dk64_asset :
	public n64_file
{
public:
	dk64_asset() = default;
	//TODO dk64_asset(dk64_asset&& src); //move constructor
	//TODO dk64_asset(const dk64_asset& src); //copy constructor
	//TODO dk64_asset(u32 indx, u32 offset, u16 compressed, u16 type, const n64_span& data);

	//TODO dk64_asset& operator = (const dk64_asset& other) {}; //assignment operator

private:
	static libdeflate_compressor* comper;
	static libdeflate_decompressor* decomper;

	void _comp_method() const override;
	void _decomp_method() const override;
};

class dk64_asset_section {
public: //helper functions for parsing/building entire asset section
	//TODO static std::vector<const dk64_asset*> parse(const n64_span&);
	//TODO static const n64_span& build(std::vector<const dk64_asset*>& assets);
};