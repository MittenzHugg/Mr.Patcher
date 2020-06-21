/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_asset.h
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
#include "simpTypes.h"
#include "bk_file.h"
#include <vector>


enum bk_asset_type_e {
	BK_EMPTY,
	BK_ANIMATION,
	BK_3D_MODEL,
	BK_2D_MODEL,
	BK_LEVEL_SETUP,
	BK_SKY,
	BK_TRANSITION,
	BK_SPRITE,
	BK_BUTTON_FILE,
	BK_TEXT,
	BK_LEVEL_MODEL,
	BK_MIDI,
};


class bk_asset :
	public n64_file
{
public:
	u32 index;

	bk_asset() = default;
	bk_asset(bk_asset&& src); //move constructor
	bk_asset(const bk_asset& src); //copy constructor
	bk_asset(u32 indx, const n64_span& tbl_entry, const n64_span& data);
	bk_asset(u32 indx, u32 offset, u16 compressed, u16 type, const n64_span& data);

	//base class functions
	using n64_file::decompress;
	using n64_file::compress;

	inline bool compressed() { return _compressed; }
	inline bk_asset_type_e type() { return _type; }
	inline u16 type_num() { return _type_num; }
	mutable u32 _offset;
	bool _compressed = false;
	u16 _type_num = 4;

private:
	static libdeflate_compressor* comper;
	static libdeflate_decompressor* decomper;
	bk_asset_type_e _type = BK_EMPTY;

	void _comp_method() const override;
	void _decomp_method() const override;
};

class bk_asset_section {
public:
	static std::vector<const bk_asset*> parse(const n64_span&);
	static const n64_span& build(std::vector<const bk_asset*>& assets);
};