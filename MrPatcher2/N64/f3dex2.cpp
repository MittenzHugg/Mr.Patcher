/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   f3dex2.cpp
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

#include "f3dex2.h"
#include <numeric>
#include <algorithm>

static enum f3d_cmds {
	G_VTX = 0x04,
	G_DL = 0x06,
	G_CLEARGEOMETRYMODE = 0xB6,
	G_SETGEOMETRYMODE = 0xB7,
	G_ENDDL = 0xB8,
	G_SetOtherMode_L = 0xB9,
	G_SetOtherMode_H = 0xBA,
	G_TEXTURE = 0xBB,
	G_RDPPIPESYNC = 0xE7,
	G_LOADTLUT = 0xF0,
	G_SETTILESIZE = 0xF2,
	G_LOADBLOCK = 0xF3,
	G_SETTILE = 0xF5,
	G_SETCOMBINE = 0xFC,
	G_SETTIMG = 0xFD,
};

static enum rsp_geo_mode {
	G_ZBUFFER		= 0x00000001,
	G_SHADE			= 0x00000004,
	G_SHADING_SMOOTH = 0x00000200,
	G_CULL_FRONT	= 0x00001000,
	G_CULL_BACK		= 0x00002000,
	G_FOG			= 0x00010000,
	G_LIGHTING		= 0x00020000,
	G_TEXTURE_GEN	= 0x00040000,
	G_TEXTURE_GEN_LINEAR = 0x00080000,
	G_CLIPPING		= 0x00800000,
};

std::vector<f3dex_vtx>* f3dex2::gen_vbo(const n64_span& src, u32 vtx_cnt)
{
	std::vector<u32> indx(vtx_cnt);
	std::iota(indx.begin(), indx.end(), 0);

	std::vector<f3dex_vtx>* retval = new std::vector<f3dex_vtx>(vtx_cnt);

	std::transform(indx.begin(), indx.end(), retval->begin(),
		[&](u32 i)->f3dex_vtx {
			u32 offest = i * 16; 
			return {
				src.seq_get<s16>(offest),
				src.seq_get<s16>(offest),
				src.seq_get<s16>(offest),
				src.seq_get<u16>(offest),
				src.seq_get<s16>(offest),
				src.seq_get<s16>(offest),
				src.seq_get<u8>(offest),
				src.seq_get<u8>(offest),
				src.seq_get<u8>(offest),
				src.seq_get<u8>(offest),
			};
		}
	);

	return retval;
}

std::vector<GLuint> f3dex2::gen_display_list(const n64_span& src)
{
	GLuint returnList = glGenLists(1);
	QGLFunctions* f = QGLContext::currentContext()->functions();

	glNewList(returnList, GL_COMPILE);
	u8 cmd;
	bool end_list = false;
	for (u32 offset = 0; offset < src.size() && !end_list; offset += 8) {
		cmd = src[offset];
		switch (cmd)
		{
		case f3d_cmds::G_VTX:{ //TODO
			u8 vtx_indx = src[offset + 1];
			u8 vtx_cnt = src[offset + 2] >> 2;
			u32 data_size = src.get<u16>(offset + 2) & 0x03FF;
			u32 seg_addr = src.get<u32>(offset + 4);

			//TODO f->glBufferSubData(GL_ARRAY_BUFFER,vtx_indx,data_size,);
			break;
		}
		case f3d_cmds::G_DL: { //TODO
			u32 seg_addr = src.get<u32>(offset + 4);
			//glCallList(_display_lists[seg_addr])
			if (src[offset + 1])
				end_list == true;
			break;
		}
		case f3d_cmds::G_CLEARGEOMETRYMODE:{
			u32 val = src.get<u32>(offset + 4);
			glDisable( 
				((val & G_ZBUFFER) ? GL_DEPTH_TEST : 0)
				| ((val & (G_CULL_FRONT | G_CULL_BACK) == (G_CULL_FRONT | G_CULL_BACK)) 
					? GL_CULL_FACE : 0)
				| ((val & G_CULL_BACK) ? GL_CULL_FACE : 0)
				| ((val & G_FOG) ? GL_FOG : 0 )
				| ((val & G_LIGHTING) ? GL_LIGHTING : 0)
			);

			u32 cull_side = val & (G_CULL_FRONT | G_CULL_BACK);
			
			if (cull_side == G_CULL_FRONT)
				glCullFace(GL_BACK);
			else if (cull_side == G_CULL_BACK)
				glCullFace(GL_FRONT);

			if (val & G_SHADING_SMOOTH)
				glShadeModel(GL_FLAT);

			//	G_SHADE = 0x00000004,

			//	G_TEXTURE_GEN = 0x00040000,
			//	G_TEXTURE_GEN_LINEAR = 0x00080000,
			//	G_CLIPPING = 0x00800000,
		}
		case f3d_cmds::G_SETGEOMETRYMODE: {
			u32 val = src.get<u32>(offset + 4);
			glEnable(
				((val & G_ZBUFFER) ? GL_DEPTH_TEST : 0)
				| ((val & G_CULL_FRONT) ? GL_CULL_FACE : 0)
				| ((val & G_CULL_BACK) ? GL_CULL_FACE : 0)
				| ((val & G_FOG) ? GL_FOG : 0)
				| ((val & G_LIGHTING) ? GL_LIGHTING : 0)
			);
			//	G_SHADE = 0x00000004,
			//	G_TEXTURE_GEN = 0x00040000,
			//	G_TEXTURE_GEN_LINEAR = 0x00080000,
			//	G_CLIPPING = 0x00800000,

			u32 cull_side = val & (G_CULL_FRONT | G_CULL_BACK);
			if (cull_side == G_CULL_FRONT)
				glCullFace(GL_FRONT);
			else if (cull_side == G_CULL_BACK)
				glCullFace(GL_BACK);
			else if (cull_side != 0)
				glCullFace(GL_FRONT_AND_BACK);

			if (val & G_SHADING_SMOOTH)
				glShadeModel(GL_SMOOTH);
			else
				glShadeModel(GL_FLAT);

		}
		case f3d_cmds::G_ENDDL: {
			end_list = true;
			break;
		}
		case f3d_cmds::G_TEXTURE: { //TODO
			u8 mip_map_levels = (src[offset + 2] >> 3) & 0x07;
			u8 tile_descriptor_enabled = src[offset + 2] & 0x07;
			u16 S = src.get<u16>(offset + 4);
			u16 T = src.get<u16>(offset + 6);
		}
		case f3d_cmds::G_RDPPIPESYNC: { //TODO
			glFlush();
		}
		case f3d_cmds::G_LOADTLUT: { //TODO

		}
		case f3d_cmds::G_SETTILE: { //TODO

		}
		case f3d_cmds::G_SETTIMG: { //TODO
			u8 format = src[offset + 1] >> 5;
			u8 bit_size = src[offset + 1] >> 3 & 0x03;
			u32 seg_addrs = src.get<u32>(offset + 4);
		}
		default:
			break;
		}
	}
	return std::vector<GLuint>();
}
