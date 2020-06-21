/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*	MrP_project.h
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
#include <string>
#include "MrP_Model.h"


class MrP_project
{
public:
	struct Context {
		u32 gshrk : 1;
		u32 assem : 1;
		u32 scene : 1;
		u32 model : 1;
		u32 music : 1;
		u32 dialg : 1;
	};

	static MrP_project* new_proj(std::string proj_file);

	virtual void save(const std::string& proj_file) = 0;
	virtual void load(const std::string& romFilePath) = 0;
	virtual const Context getContext(void) = 0;


	//interface for editors
	virtual std::vector<const MrP_Model*> models() { return std::vector<const MrP_Model*>(); };


	//virtual void export(const std::string& exportPath) = 0;

	inline void setInFilePath(std::string in_rom_path) { _in_filename = in_rom_path; };
	inline void setOutFilePath(std::string out_rom_path) { _out_filename = out_rom_path; };

protected:
	std::string _in_filename;
	std::string _out_filename;
};