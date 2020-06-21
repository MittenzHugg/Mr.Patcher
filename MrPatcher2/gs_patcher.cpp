/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   gs_patcher.cpp
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

#include "gs_patcher.h"
#include <list>
#include <algorithm>


std::list<bk_asm_file*>& gs_patcher::patch(bk_rom* rom, patchObj* patch_list)
{
	std::list<patchObj*>& p_list = patch_list->toList();
	//std::list<bk_asm_file*>& static_asm = rom->static_asm();

	//validate patch files as gs_code

	//gen mem masks foreach patch (shows what bytes are changed in RAM 8MB)
	//gen static_asm mask
	//gen level_asm mask
	//overlay mask = static_asm_mask U level_asm_mask U expansion pak
	//foreach patch 
		//throw is outide overlay_mask
	//acc_patch = fold patches
		//throw if acc_mask = acc_mask ⋂ cur_mask

	//duplicate asm_files
	//apply patches to asm_files
	//return asm_files
	return std::list<bk_asm_file*>();
}
