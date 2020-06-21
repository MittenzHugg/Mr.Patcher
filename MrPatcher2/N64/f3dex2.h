/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   f3dex2.h
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
#include <vector>
#include <QtOpenGL>
#include "n64_span.h"
#include "simpTypes.h"

struct f3dex_vtx {
	s16 x;
	s16 y;
	s16 z;
	u16 flag;
	s16 u;
	s16 v;
	u8 r;
	u8 g;
	u8 b;
	u8 a;
};


class f3dex2
{
public:
	static std::vector<f3dex_vtx>* gen_vbo(const n64_span& src, u32 vtx_cnt);
	static std::vector<GLuint> gen_display_list(const n64_span& src);
	//static gen_display_list(const n64_span* dl_src, u32 vtx_cnt);
};

