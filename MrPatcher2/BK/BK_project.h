/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   BK_Project.h
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
#include "MrP_project.h"
#include "bk_rom.h"
#include <string>

class BK_project :
	public MrP_project
{
public:
	BK_project(bk_rom* BK) : _BK{BK} {};
	BK_project(n64_rom&& BK) :_BK{ new bk_rom(std::move(BK)) } {}

	//MrP_Project interface
	void save(const std::string& proj_file) override;
	void load(const std::string& proj_file);
	inline const MrP_project::Context getContext(void) override { return _bkContext; };


	std::vector<const MrP_Model*> models();
	//void export(const std::string& exportPath) override { _BK->export_rom(exportPath); return; };


private:
	bk_rom* _BK;

	//FILE
	const MrP_project::Context _bkContext = {
		.gshrk = 1,
		.assem = 0,
		.scene = 0,
		.model = 1,
		.music = 0,
		.dialg = 0,
	};
};

