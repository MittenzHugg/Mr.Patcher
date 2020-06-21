/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_rom.h
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

/************************************************
* BK ROM 
* Description:
*     Represents original BK ROM, and functions
* for building a ROM using the BK engine. 
*/
#pragma once

#include <list>
#include <vector>
#include <array>
#include "n64_rom.h"
#include "bk_enums.h"
#include "bk_file.h"
#include "bk_asm_file.h"
#include "bk_asset.h"
#include "ALBankFile.h"



class bk_rom :
	public n64_rom
{
public:
	bk_rom(char* filename);
	bk_rom(n64_rom&& src);
	~bk_rom();

	//function forwarding for visibility
	inline u64 getHash() { return n64_rom::getHash(); };

	void export_rom(const std::string& dir, bool split_c = false);
	const n64_span* build_mod(
		const std::vector<const bk_asm_file*>& modified_asm, 
		const std::vector<const bk_asset*>& modified_assets
		// TODO , std::vector<const alBank*> sfx,
		// TODO , std::vector<const alBank*> music_sounds
	);
	std::vector<const bk_asset*> assets(void); //returns all assets
	std::array<const bk_asm_file*, 0x11> asm_files(void) { return _asm_files; };
	std::vector<const bk_model*> models(void); //returns all model assets
	bk_model* model(u32 indx);

private:
	std::array<const bk_asm_file*, 0x11> _asm_files = std::array<const bk_asm_file*, 0x11>();
	//std::array<bk_soundfont*, 2> _soundfonts = std::array<bk_asm_file*, 2>();
	n64_span _unknown_rom_section;
	n64_span _core_1_crc_sec;

	std::vector<const bk_asset*> _assets;
	n64_span _asset_span;

	n64_span	_soundfont_1_bank_span;
	n64_span    _soundfont_1_wave_span;
	ALBankFile* _soundfont_1 = nullptr;

	n64_span	_soundfont_2_bank_span;
	n64_span    _soundfont_2_wave_span;
	ALBankFile* _soundfont_2 = nullptr;

	u32 _get_from_instructions(const n64_file& section, const u32 upper, u32 lower) const;
	void _init(void);
};

