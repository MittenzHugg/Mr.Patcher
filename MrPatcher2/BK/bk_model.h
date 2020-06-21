/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_model.h
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
#include "bk_asset.h"
#include "MrP_Model.h"
#include <QtOpenGL>
#include <QOpenGLBuffer>
#include "f3dex2.h"

struct vert_head {
	s16 sX;
	s16 sY;
	s16 sZ;
	s16 lX;
	s16 lY;
	s16 lZ;
	s16 cX;
	s16 cY;
	s16 cZ;
	u16 unknown_0x12;
	u16 cnt;
	u16 unknown_0x16;
	
};

enum bk_geo_type {
	GEO_SORT = 0x01,
	GEO_BONE = 0x02,
	GEO_LOAD_DL = 0x03,
	
	GEO_SKINNING = 0x05,
	
	GEO_LOD = 0x08,
	GEO_REF_POINT = 0x0A,
	GEO_SELECTOR = 0x0C,
	GEO_DRAW_DIST = 0x0D,
};

class bk_model_geo {
public:
	bk_model_geo(const n64_span& buffer) :cmd((bk_geo_type) buffer.get<u32>()) {};
	bk_geo_type cmd;
};

class bk_model :
	public bk_asset, public MrP_Model
{
public:
	bk_model(bk_asset&& src);
	bk_model(u32 indx, const n64_span& tbl_entry, const n64_span& data);

	using bk_asset::compress;
	using bk_asset::decompress;

	using bk_asset::compressed;
	using bk_asset::type;
	using bk_asset::type_num;

	vert_head* getVertHead(void) { return &_vertex_header; }

	void dl_init(void);
	GLuint gl_dl(void) { return _display_list; };
	std::vector<u16>* getTris(void) { return &tri_list; }
	

	void draw(void) override;
	float scale(void) { return _vertex_header.lX - _vertex_header.sX; }

private:

	void _init(void);
	void _parse_texture_header(void);
	void _parse_display_list_header(void);
	void _parse_vertex_header(void);
	//void _parse_vertex_bones(void);
	//void _parse_collision_header(void);
	//void _parse_effects_header(void);


	struct {
		u32 START;
		u32 geo_offset;
		u16 texture_offset;
		u16 geo_type;
		u32 display_list_offset;
		u32 vertex_offset;
		u32 unkown_0x14;
		u32 animation_offset;
		u32 collision_offset;
		u32 effects_end;
		u32 effecst_offset;
		u32 vertex_bone_offset;
		u32 unknown_0x2C;
		u16 tri_count;
		u16 vert_count;
		u16 unknown_0x34;
		u16 unknown_0x36;
	}_header;

	struct {
		u32 size;
		u16 cnt;
		u16 unkown_0x06;
		
	}_texture_header;

	struct {
		u32 cnt;
		u32 unknown_0x04;
	}_display_list_header;
	n64_span _display_list_data;
	
	QOpenGLVertexArrayObject _vao;
	QOpenGLBuffer _vbo;
	QOpenGLBuffer _ibo;
	GLuint _display_list;
	QOpenGLBuffer dlbo;

	vert_head _vertex_header;

	u32 tri_count = 0;
	n64_span _texture_data;
	n64_span _vertex_data;
	std::vector<GLushort> tri_list;
};

