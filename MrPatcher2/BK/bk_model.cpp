/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_model.cpp
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

#include "bk_model.h"


bk_model::bk_model(bk_asset&& src)
	:bk_asset(std::move(src)),
	_ibo(QOpenGLBuffer::IndexBuffer)

{
	_init();
}

bk_model::bk_model(u32 indx, const n64_span& tbl_entry, const n64_span& data)
	:bk_asset(indx,tbl_entry, data)
{
	_init();
	//_header.geometry_offset = decompress()->seq_get<u32>(offset);
	//_header.texture_offset = decompress()->seq_get<u16>(offset);
}

void bk_model::dl_init(void)
{
	if (type() == bk_asset_type_e::BK_EMPTY)
		return;

	//set up vao
	_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&_vao);

	_vbo.create();
	_vbo.bind();
	std::vector<f3dex_vtx>* vboData = f3dex2::gen_vbo(_vertex_data, _vertex_header.cnt);
	_vbo.allocate(vboData->data(), _vertex_header.cnt * sizeof(f3dex_vtx));
	_vbo.release();

	//generate tri_list
	std::vector<u16> vcache(32);
	u32 vBase = 0;
	for (u32 offset = 0; offset < _display_list_data.size(); offset += 8) {
		u8 cmd = _display_list_data[offset];
		if (cmd == 0x04) {
			//u64 x = _display_list_data->get<u64>(offset);
			u8 start = _display_list_data[offset +1]/2;
			u8 cnt = _display_list_data[offset + 2] >> 2;
			u32 vBase_i = (_display_list_data.get<u32>(offset + 4) & 0x0000FFFF) / 16;
			for (u8 i = start; i < start + cnt; i++)
			{
				vcache[i] = vBase_i + i - start;
			}

		}
		else if (cmd == 0xBF) {
			for (u8 i = 5; i < 8; i++) {
				tri_list.push_back(vcache[_display_list_data[offset + i] / 2]);
			}
		}
		else if (cmd == 0xB1 || cmd == 0xB5) {
			for (u8 i = 1; i < 4; i++) {
				tri_list.push_back(vcache[_display_list_data[offset + i] / 2]);
			}
			for (u8 i = 5; i < 8; i++) {
				tri_list.push_back(vcache[_display_list_data[offset + i] / 2]);
			}
		}
	}
	_ibo.create();
	_ibo.bind();
	_ibo.allocate(tri_list.data(), tri_list.size());
	_ibo.release();

	_vbo.bind();
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glEnableVertexAttribArray(2);
	f->glVertexAttribPointer(0, 3, GL_SHORT, GL_TRUE, sizeof(f3dex_vtx), NULL); //position
	f->glVertexAttribPointer(1, 2, GL_SHORT, GL_TRUE, sizeof(f3dex_vtx), (void*)(8)); //uv
	f->glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE,GL_TRUE, sizeof(f3dex_vtx), (void*)(12)); //color
	_vbo.release();
}

void bk_model::draw(void)
{
	if (type() == bk_asset_type_e::BK_EMPTY)
		return;
	//if display_list == null_ptr
		//TODO process geo
	QOpenGLVertexArrayObject::Binder vaoBinder(&_vao);


	//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glPointSize(2.5f);
	glDrawArrays(GL_POINTS, 0, _vertex_header.cnt);
	
	_vbo.bind();
	_ibo.bind();
	glDrawElements(GL_TRIANGLES, tri_list.size(), GL_UNSIGNED_SHORT, 0);
	
	
	//glNewList(_display_list, GL_COMPILE);

	//dlbo.bind();
	//glDrawElements(GL_TRIANGLES, tri_count, GL_UNSIGNED_BYTE, 0);
	//dlbo.release();
	//glCallList(_display_list);
}

void bk_model::_init(void) {

	if (type() == bk_asset_type_e::BK_EMPTY)
		return;

	u32 offset = 0;
	const n64_span dcmp = decompress();
	_header.START = dcmp.seq_get<u32>(offset);
	if (_header.START != 0x0000000B) {
		throw "Unknown File Type";
		return;
	}

	_header.geo_offset			= dcmp.seq_get<u32>(offset);
	_header.texture_offset		= dcmp.seq_get<u16>(offset);
	_header.geo_type			= dcmp.seq_get<u16>(offset);
	_header.display_list_offset = dcmp.seq_get<u32>(offset);
	_header.vertex_offset		= dcmp.seq_get<u32>(offset);
	_header.unkown_0x14			= dcmp.seq_get<u32>(offset);
	_header.animation_offset	= dcmp.seq_get<u32>(offset);
	_header.collision_offset	= dcmp.seq_get<u32>(offset);
	_header.effects_end			= dcmp.seq_get<u32>(offset);
	_header.effecst_offset		= dcmp.seq_get<u32>(offset);
	_header.vertex_bone_offset	= dcmp.seq_get<u32>(offset);
	_header.unknown_0x2C		= dcmp.seq_get<u32>(offset);
	_header.tri_count			= dcmp.seq_get<u16>(offset);
	_header.vert_count			= dcmp.seq_get<u16>(offset);
	_header.unknown_0x34		= dcmp.seq_get<u16>(offset);
	_header.unknown_0x36		= dcmp.seq_get<u16>(offset);

	if (_header.texture_offset != 0) {
		_parse_texture_header();
	}
	if (_header.display_list_offset != 0) {
		_parse_display_list_header();
	}
	if (_header.vertex_offset != 0) {
		_parse_vertex_header();
	}
}

void  bk_model::_parse_texture_header(void) {
	u32 offset = _header.texture_offset;
	const n64_span dcmp = decompress();
	_texture_header.size = dcmp.seq_get<u32>(offset);
	_texture_header.cnt = dcmp.seq_get<u16>(offset);
	_texture_header.unkown_0x06 = dcmp.seq_get<u16>(offset);
	_texture_data = dcmp.slice(offset, _texture_header.size);
}

void  bk_model::_parse_display_list_header(void) {
	u32 offset = _header.display_list_offset;
	const n64_span dcmp = decompress();
	_display_list_header.cnt = dcmp.seq_get<u32>(offset);
	_display_list_header.unknown_0x04 = dcmp.seq_get<u32>(offset);
	_display_list_data = dcmp.slice(offset, _display_list_header.cnt*8);
}

void bk_model::_parse_vertex_header(void) {
	u32 offset = _header.vertex_offset;
	const n64_span dcmp = decompress();
	_vertex_header.sX = dcmp.seq_get<s16>(offset);
	_vertex_header.sY = dcmp.seq_get<s16>(offset);
	_vertex_header.sZ = dcmp.seq_get<s16>(offset);
	_vertex_header.lX = dcmp.seq_get<s16>(offset);
	_vertex_header.lY = dcmp.seq_get<s16>(offset);
	_vertex_header.lZ = dcmp.seq_get<s16>(offset);
	_vertex_header.cX = dcmp.seq_get<s16>(offset);
	_vertex_header.cY = dcmp.seq_get<s16>(offset);
	_vertex_header.cZ = dcmp.seq_get<s16>(offset);
	_vertex_header.unknown_0x12 = dcmp.seq_get<u16>(offset);
	_vertex_header.cnt = dcmp.seq_get<u16>(offset);
	_vertex_header.unknown_0x16 = dcmp.seq_get<u16>(offset);
	_vertex_data = dcmp.slice(_vertex_header.cnt*16, offset);
}
