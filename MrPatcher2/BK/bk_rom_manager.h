/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_rom_manager.h
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
#include "bk_rom.h"
#include "n64_rom.h"
#include "bk_file.h"
#include "patchObj.h"
#include <QCombobox>

#define BK_MNGR_PATCH_IN_DYNAMIC_REGION (0x01<<0)
#define BK_MNGR_PATCH_IN_BOOTLOADER (0x01<<1)
#define BK_MNGR_PATCH_IN_UNKNOWN_REGION (0x01<<2)
#define BK_MNGR_ASM_PATCH (0x01<<3)
#define BK_MNGR_C_PATCH (0x01<<4)

typedef enum bk_asm_file_e {
	bootloader,
	basicLibs,
	engine,
	sm,
	mm,
	ttc,
	cc,
	bgs,
	fp,
	gv,
	mmm,
	rbb,
	ccw,
	lair,
	cs,
	fight,
	expansion_pak,
} bk_asm_file_t;

class bk_rom_manager
{
public:

	n64_rom* bk_Rom;

	bk_rom_manager(n64_rom* inROM);
	~bk_rom_manager();

	//bk_file* getFile(uint32_t fileIndex);

	uint8_t* mod_rom_buffer;

	void export_rom(char* dir);

	void set_asm_combo_options(QComboBox* cbox);
	void applyPatches(patchObj* patch_list);
	FILE* packROM(void);
	//QList<QString> comboList;
	

private:
	bk_file* asm_code[0x11] = { nullptr };
	bk_file* asm_vars[0x11] = { nullptr };

	uint8_t* dyn_file_table;
	uint32_t dyn_file_cnt;
	uint8_t* dyn_file_base;


	//functions
	uint32_t getAddressFromLI(n64_file* section, uint32_t upper, uint32_t lower);

};


