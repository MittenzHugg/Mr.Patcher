/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_rom.cpp
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
*/

#include "bk_rom.h"
#include "bk_enums.h"
#include <algorithm>
#include <numeric>
#include <direct.h>
#include <execution>
#include <SoundFont2.h>



static const uint32_t level_asm_upper[0x0D] = {
		0x187A,//mm
		0x1872,//ttc
		0x180A,//cc
		0x1882,//bgs
		0x1892,//fp
		0x1902,//lair
		0x181A,//gv
		0x18EA,//ccw
		0x188A,//rbb
		0x1812,//mmm
		0x18F2,//sm
		0x190A,//fight
		0x18FA,//cs
};

bk_rom::bk_rom(char* filename)
	:n64_rom(filename)
{	
	if (gameID != supported_games_t::BANJOKAZOOIE_NTSC
		&& gameID != supported_games_t::BANJOKAZOOIE_PAL
		&& gameID != supported_games_t::BANJOKAZOOIE_JP
		&& gameID != supported_games_t::BANJOKAZOOIE_NTSC_REV1
		) {
		delete this;
		throw "Not a recognized version of Banjo-Kazooie";
		return;
	}

	_init();

}

bk_rom::bk_rom(n64_rom&& src)
	:n64_rom(std::move(src))
{
	if (gameID != supported_games_t::BANJOKAZOOIE_NTSC
		&& gameID != supported_games_t::BANJOKAZOOIE_PAL
		&& gameID != supported_games_t::BANJOKAZOOIE_JP
		&& gameID != supported_games_t::BANJOKAZOOIE_NTSC_REV1
		) {
		delete this;
		throw "Not a recognized version of Banjo-Kazooie";
		return;
	}

	_init();
}

bk_rom::~bk_rom(void) {
	std::destroy(_asm_files.begin(), _asm_files.end());
	std::destroy(_assets.begin(), _assets.end());
}

void bk_rom::export_rom(const std::string& dir, bool split_c)
{
	return;
}

const n64_span* bk_rom::build_mod(const std::vector<const bk_asm_file*>& modified_asm, const std::vector<const bk_asset*>& modified_assets)
{
	//original asm
	std::vector<const bk_asm_file*> asm_set;
	asm_set.reserve(11);

	//modified asm
	if (modified_asm.empty()) {
		//no mod files
		asm_set.assign(_asm_files.begin(), _asm_files.end()); //already ordered no need to sort
	}
	else {
		//copy mod asm to sorted set
		std::vector<const bk_asm_file*> mod_asm_set(modified_asm);
		std::sort(std::execution::par_unseq, mod_asm_set.begin(), mod_asm_set.end(),
			[](const bk_asm_file* lhs, const bk_asm_file* rhs) { return (lhs->id() < rhs->id()); }
		);

		//create set union prioritizing mod files
		std::set_union(mod_asm_set.begin(), mod_asm_set.end(),
			_asm_files.begin(), _asm_files.end(),
			std::back_inserter(asm_set),
			[](const bk_asm_file* lhs, const bk_asm_file* rhs) { return (lhs->id() == rhs->id()); }
		);
	}


	//CREATE ASSET SECTION
	std::vector<const bk_asset*> asset_set;
	asset_set.reserve(assets().size());

	if (modified_assets.empty()) {
		asset_set = std::vector<const bk_asset*>(assets());
	}
	else {
		//copy mod asm to sorted set
		std::vector<const bk_asset*> mod_asset_set(modified_assets);
		std::sort(std::execution::par_unseq,
			mod_asset_set.begin(), mod_asset_set.end(),
			[](const bk_asset* lhs, const bk_asset* rhs) { return (lhs->index < rhs->index); }
		);

		//sort OG assets
		std::vector<const bk_asset*>& og_asset_set(assets()); //do this to ensure asset section is parsed
		std::sort(std::execution::par_unseq,
			og_asset_set.begin(), og_asset_set.end(),
			[](const bk_asset* lhs, const bk_asset* rhs) { return (lhs->index < rhs->index); }
		);

		//union of assets
		std::set_union(mod_asset_set.begin(), mod_asset_set.end(),
			og_asset_set.begin(), og_asset_set.end(),
			std::back_inserter(asset_set),
			[](const bk_asset* lhs, const bk_asset* rhs) { return (lhs->index == rhs->index); }
		);

	}

	const n64_span& new_asset_sec = bk_asset_section::build(asset_set);

	//TODO CREATE ALBANKs
	//TODO sort sounds
	//TODO create sound unions
	//TODO generate alBank/wave files

	//TODO calculate section rom offsets 
	//bootloader = 0x400
	u32 asset_offset = 0x5E90;

	u32 sound1_bank_offset = asset_offset + new_asset_sec.size();
	u32 sound1_wave_offset = sound1_bank_offset = _soundfont_1_bank_span.size();
	//TODO write sound1 offsets to static code instructions

	u32 sound2_bank_offset = sound1_wave_offset + _soundfont_1_wave_span.size();
	u32 sound2_wave_offset = sound2_bank_offset = _soundfont_2_bank_span.size();
	//TODO write sound2 offsets to static code instructions

	const u32* core_2_crc = asm_set.at(bk_asm_f_t::core_2_asm)->checksum();
	//TODO write asm[core2] checksums to asm[core1]
	const u32* core_1_crc = asm_set.at(bk_asm_f_t::core_1_asm)->checksum();
	//TODO write asm[core1] checksums to rom
	u32 asm_core_1_offset = sound2_wave_offset + _soundfont_2_wave_span.size();
	
	//compress all asm_files
	std::vector<n64_span> comp_asm;
	comp_asm.reserve(asm_set.size());
	std::transform(std::execution::par_unseq,
		asm_set.begin(), asm_set.end(), comp_asm.begin(),
		[](const bk_asm_file* x)->n64_span { return x->compress(); }
	);

	//calculate all offsets
	//write offsets to bootloader

	//u32 asm_core_2_offset = asm_core_1_offset + comp_asm.at(bk_asm_f_t::core_1_asm).size();
	//TODO write asm[core2] checksums to asm[core1]
	//TODO write asm[core1] checksums to rom
	//TODO compress level core asm files + calculate offsets

	//write asm offsets to bootloader


	//TODO if expansion pak code
		//TODO Write asm[core1] boot to expansion pak
		//TODO update bootloader for extra boot stage

	//TODO update ROM header
	//TODO allocate MrP_bk_rom header
	//TODO write all sections to ROM
	//TODO update N64 CRCs
	

	return nullptr;
}

std::vector<const bk_asset*> bk_rom::assets(void)
{
	if (_assets.empty()) 
		_assets = bk_asset_section::parse(_asset_span);
	return _assets;
}

std::vector<const bk_model*> bk_rom::models(void)
{
	std::vector<const bk_asset*>& ass = assets();
	auto end_it = std::remove_if(ass.begin(), ass.end(),
		[](const bk_asset* x) {return ((x->index < 0x2CA) || (x->index > 0x71B))
		|| ((x->_type_num != 4) && (x->_type_num != 0));}
	);
	ass.resize(std::distance(ass.begin(), end_it));
	
	std::vector<const bk_model*> ret_val(ass.size());

	std::transform(ass.begin(), ass.end(), ret_val.begin(),
		[](const bk_asset* x) {
			return new bk_model(std::move(bk_asset(*x)));
		}
	);
	return ret_val;
}

bk_model* bk_rom::model(u32 indx)
{
	std::vector<const bk_asset*>& ass = assets();
	bk_asset& retval = bk_asset(**std::find_if(ass.begin(), ass.end(),
		[&](const bk_asset* x) {return x->index == indx; }));
	return new bk_model(std::move(retval));
}

u32 bk_rom::_get_from_instructions(const n64_file& section, const u32 upper, const u32 lower) const {
	u32 tmp = (u32)section.decompress().get<u16>(upper);
	u32 tmp2 = (u32)section.decompress().get<u16>(lower);
	if (tmp2 > 0x7FFF) tmp--; //n64 signed value bug
	tmp = tmp << 0x10;
	return (uint32_t)((int32_t)tmp + tmp2);
}

void bk_rom::_init(void)
{
#pragma region SETUP_ASM
	//bootloader
	//const bk_asm_file* bootloader = new bk_asm_file(bk_asm_f_t::boot_asm, buffer.slice(0x1000,0x4BE0), 0x400, false);
	_asm_files[bk_asm_f_t::boot_asm] = new bk_asm_file(bk_asm_f_t::boot_asm, buffer.slice(0x1000, 0x4BE0), 0x400, false);
	const bk_asm_file& bootloader = *_asm_files[bk_asm_f_t::boot_asm];

	//C Libraries
	u32		core1_rom_offset = _get_from_instructions(bootloader, 0x7A, 0x82);
	u32		core1_ram_offset = _get_from_instructions(bootloader, 0x5A, 0x66);
	u32		core1_size = _get_from_instructions(bootloader, 0x7E, 0x86) - core1_rom_offset;
	n64_span tmp = buffer.slice(core1_rom_offset, core1_size);
	size_t tmpS = tmp.size();
	_asm_files[bk_asm_f_t::core_1_asm] = new bk_asm_file(bk_asm_f_t::core_1_asm, tmp, core1_ram_offset);
	const bk_asm_file& core_1 = *_asm_files[bk_asm_f_t::core_1_asm];

	//Game Engine
	u32		core2_rom_offset = _get_from_instructions(bootloader, 0x17FA, 0x1822);
	u32		core2_ram_offset = _get_from_instructions(core_1, 0x0E, 0x1A);
	u32		core2_size = _get_from_instructions(bootloader, 0x17FE, 0x1826) - core2_rom_offset;
	bk_asm_file* core_2 = new bk_asm_file(bk_asm_f_t::core_2_asm, buffer.slice(core2_rom_offset, core2_size), core2_ram_offset);
	_asm_files[bk_asm_f_t::core_2_asm] = core_2;

	//Level ASM
	u32 level_asm_ram_off;
	switch (gameID) {
	case supported_games_t::BANJOKAZOOIE_NTSC:
		level_asm_ram_off = 0x803863F0;
		break;
	case supported_games_t::BANJOKAZOOIE_PAL:
	case supported_games_t::BANJOKAZOOIE_JP:
	case supported_games_t::BANJOKAZOOIE_NTSC_REV1:
	default:
		throw "Level ASM RAM Offset Unspecified for Version";
		break;
	}

	std::vector<u8> iLevel(bk_asm_f_t::cs_asm - bk_asm_f_t::mm_asm + 1);
	std::iota(iLevel.begin(), iLevel.end(), (u8)bk_asm_f_t::mm_asm);
	std::transform(std::execution::par_unseq,
		iLevel.begin(), iLevel.end(), _asm_files.begin() + bk_asm_f_t::mm_asm,
		[&](u8 i) -> bk_asm_file* {
			u32 upper = level_asm_upper[i - bk_asm_f_t::mm_asm];
			u32 lower = upper + 0x28;
			u32 level_asm_rom_off = _get_from_instructions(bootloader, upper, lower);
			u32 level_asm_size = _get_from_instructions(bootloader, upper + 4, lower + 4);

			level_asm_size -= level_asm_rom_off;
			return new bk_asm_file((bk_asm_f_t)i, buffer.slice(level_asm_rom_off, level_asm_size), level_asm_ram_off);
		}
	);
#pragma endregion SETUP_ASM

#pragma region SETUP_ASSETS
	//ASSETS
	u32 asset_cnt = buffer.get<u32>(0x5E90);
	u32 asset_end = buffer.get<u32>(0x5E90 + 8 * (asset_cnt));
	_asset_span = buffer.slice(0x5E90, asset_end + 8 * (asset_cnt) + 8);
#pragma endregion SETUP_ASSETS

#pragma region SETUP_SOUNDFONTS
	//SOUNDFONTS
	//Soundfont - table	
	//TODO specify as asm2 offset
	u32 sound1_bank_offset = 0xd846c0;
	u32 sound1_wave_offset = 0xd954b0;
	u32 sound2_bank_offset = 0xea3eb0;
	u32 sound2_wave_offset = 0xeade60;

	switch (gameID) {
	case supported_games_t::BANJOKAZOOIE_NTSC:
		//TODO specify as asm2 offset
		//u32 sound1_bank_offset = _get_from_instructions(core_2, 0xAE2DE, 0xAE2E2);
		//u32 sound1_wave_offset = _get_from_instructions(core_2, , );
		//u32 sound2_wave_offset = _get_from_instructions(core_2, , );
		//u32 sound2_wave_offset = _get_from_instructions(core_2, , );
		sound1_bank_offset = 0xd846c0;
		sound1_wave_offset = 0xd954b0;
		sound2_bank_offset = 0xea3eb0;
		sound2_wave_offset = 0xeade60;
		break;
	case supported_games_t::BANJOKAZOOIE_PAL:
	case supported_games_t::BANJOKAZOOIE_JP:
	case supported_games_t::BANJOKAZOOIE_NTSC_REV1:
	default:
		//TODO version support
		throw "Soundfont RAM Offset Unspecified for Version";
		break;
	}
	
	_soundfont_1_bank_span = buffer.slice(sound1_bank_offset, sound1_wave_offset - sound1_bank_offset);
	_soundfont_1_wave_span = buffer.slice(sound1_wave_offset, sound2_bank_offset - sound1_wave_offset);
	
	_soundfont_2_bank_span = buffer.slice(sound2_bank_offset, sound2_wave_offset - sound2_bank_offset),
	_soundfont_2_wave_span = buffer.slice(sound2_wave_offset - core1_rom_offset - sound2_wave_offset);
#pragma endregion SETUP_SOUNDFONTS

#pragma region SETUP_MISC
	//UNKNOWN SECTION - no clue what this is...
	_unknown_rom_section = buffer.slice(0x5BE0, (size_t) 0x5E70 - 0x5BE0);

	//CODE CRCs
	_core_1_crc_sec = buffer.slice(0x5E70, (size_t) 0x20);
#pragma endregion SETUP_MISC
}




