/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_asset.cpp
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

#include "bk_asset.h"
#include <algorithm>
#include <numeric>
#include <execution>

libdeflate_compressor* bk_asset::comper = libdeflate_alloc_compressor(12); 			//TODO: relax compression?
libdeflate_decompressor* bk_asset::decomper = libdeflate_alloc_decompressor();

//move constructor
bk_asset::bk_asset(bk_asset&& src)
	: n64_file(std::move(src))
	, index{src.index}
	, _offset{src._offset}
	, _compressed{src._compressed}
	, _type_num{src._type_num}
	, _type{src._type}
{}

bk_asset::bk_asset(const bk_asset& src)
	: n64_file(src)
	, index{ src.index }
	, _offset{src._offset}
	, _compressed{src._compressed}
	, _type_num{ src._type_num }
	, _type{ src._type }
{}

bk_asset::bk_asset(u32 indx, const n64_span& tbl_entry, const n64_span& data)
	:bk_asset(indx, tbl_entry.get<u32>(), tbl_entry.get<u16>(0x04), tbl_entry.get<u16>(0x06),
		data)
{}

bk_asset::bk_asset(u32 indx, u32 offset, u16 compressed, u16 type, const n64_span& data)
	: _offset{ offset }
	, index{ indx }
	, _compressed{ compressed != 0 }
	, _type_num{ type }
	, n64_file(data, compressed != 0)
{
	if (_type_num == 4) _type = BK_EMPTY;
	else if (indx > 0x1517) _type = BK_MIDI;
	else if (indx > 0x146A) _type = BK_LEVEL_MODEL;
	else if (indx > 0x0A0A) _type = BK_TEXT;
	else if (indx > 0x09A3) _type = BK_BUTTON_FILE;
	else if (indx > 0x07D8) _type = BK_SPRITE;
	else if (indx > 0x07CE) _type = BK_TRANSITION;
	else if (indx > 0x07B9) _type = BK_SKY;
	else if (indx > 0x071C) _type = BK_LEVEL_SETUP;
	else if (indx > 0x02D0) _type = ((_type_num == 0) ? BK_3D_MODEL : BK_2D_MODEL);
	else _type = BK_ANIMATION;
}

void bk_asset::_comp_method() const
{
	u8* tmpBuffer = (u8*)calloc(_decomp_buffer->size() + 0xC, sizeof(u8));
	if (tmpBuffer == nullptr)
		throw "Bad memory allocation";

	n64_span tmpSpan = n64_span(tmpBuffer, tmpBuffer + _decomp_buffer->size() + 0x06);

	tmpSpan.set<u16>(0x1172);
	u32 outsize = libdeflate_deflate_compress(comper, _decomp_buffer->begin(), _decomp_buffer->size(), tmpSpan.begin() + 0x06, tmpSpan.size() - 0x06);
	if (outsize == 0)
		throw "Unable to compress code buffer.";
	tmpSpan.set<u32>(outsize, 0x02);
	size_t comp_size = outsize + 0x06;
	tmpSpan.set<u16>(0x1172);

	tmpBuffer = (u8*)realloc(tmpBuffer, comp_size);
	if (tmpBuffer == nullptr)
		throw "Bad memory reallocation";

	_comp_buffer = new n64_span(tmpBuffer, (size_t)comp_size);
	_comp_own = true;

	return;
}

void bk_asset::_decomp_method() const {
	uint8_t* tmpBuffer = (uint8_t*)malloc(0x400000);
	if (tmpBuffer == nullptr)
		throw "Bad memory allocation.";
	size_t tmpCompSize = _comp_buffer->size();
	if (tmpCompSize == 0) { return; }
	size_t tmpDecompSize;

	//decompress Code
	enum libdeflate_result decompResult = libdeflate_deflate_decompress_ex(decomper, _comp_buffer->begin() + 6, _comp_buffer->size() - 6
		, tmpBuffer, 0x400000, &tmpCompSize, &tmpDecompSize);
	//decompress data
	tmpCompSize += 6;
	tmpBuffer = (uint8_t*)realloc(tmpBuffer, tmpDecompSize);
	if (tmpBuffer == nullptr)
		throw "Bad memory reallocation.";
	_decomp_buffer = new n64_span(tmpBuffer, tmpDecompSize);
	_decomp_own = true;
	return;
}


std::vector<const bk_asset*> bk_asset_section::parse(const n64_span& bin_section)
{
	u32 _asset_count = bin_section.get<u32>(0);

	size_t asset_table_size = 0x08 * _asset_count;
	const n64_span _asset_table = bin_section.slice(0x08, asset_table_size);

	u32 asset_buffer_start = asset_table_size + 0x08;
	u32 asset_buffer_size = _asset_table.get<u32>(8 * (_asset_count - 1));
	const n64_span _asset_buffer = bin_section.slice(asset_buffer_start, asset_buffer_size);

	std::vector<u32> iAsset(_asset_count - 2);
	std::iota(iAsset.begin(), iAsset.end(), 0);

	std::vector<const bk_asset*>* _assets = new std::vector<const bk_asset*>(iAsset.size());

	std::transform(std::execution::par_unseq,
		iAsset.begin(), iAsset.end(), _assets->begin(),
		[&](u32 i)->const bk_asset* {
			u32 rom_off = _asset_table.get<u32>(i * 0x08);
			u32 size = _asset_table.get<u32>((i + 1) * 0x08) - rom_off;

			return new bk_asset(i,
				_asset_table.slice(i * 0x08, 0x08),
				_asset_buffer.slice(rom_off, size)
			);
		}
	);

	std::sort(std::execution::par_unseq,
		_assets->begin(), _assets->end(),
		[](const bk_asset* lhs, const bk_asset* rhs) {
			return lhs->index < rhs->index;
		}
	);

	return *_assets;
}

const n64_span& bk_asset_section::build(std::vector<const bk_asset*>& assets)
{
	std::sort(std::execution::par_unseq,
		assets.begin(), assets.end(),
		[](const bk_asset* lhs, const bk_asset* rhs)-> bool {
			return (lhs->index < rhs->index); }
	);

	u32 max_indx = assets.back()->index;

	//assume set is full and unique

	std::vector<size_t> ass_sizes;
	ass_sizes.resize(assets.size());
	std::transform(std::execution::par_unseq,
		assets.begin(), assets.end(), ass_sizes.begin(),
		[](const bk_asset* asset) -> size_t {
			return asset->compress().size();
		}
	);

	std::vector<size_t> data_offsets = std::vector<size_t>{ 0 };
	data_offsets.resize(assets.size() + 1);

	std::partial_sum(ass_sizes.begin(), ass_sizes.end(), data_offsets.begin() + 1);
	size_t data_comp_size = data_offsets.back();
	data_offsets.pop_back();

	std::for_each(std::execution::par_unseq,
		assets.begin(), assets.end(),
		[&](const bk_asset* x) {
			x->_offset = data_offsets.at(x->index);
		}
	);

	//write data
	u8* out_buffer = (u8*)calloc(data_comp_size + (max_indx + 1) * 8, sizeof(u8));
	n64_span* oSpan = new n64_span(out_buffer, (size_t)data_comp_size + (max_indx + 1) * 8);
	u8* out_data = out_buffer + 8 * (max_indx + 1) + 8;


	oSpan->set<u32>(max_indx);
	std::for_each(assets.begin(), assets.end(),
		[&](const bk_asset* x) {
			//header
			oSpan->set<u32>((u32)x->_offset, 8 + 8 * x->index);
			oSpan->set<u16>((u16)x->_compressed, 8 + 8 * x->index + 4);
			oSpan->set<u16>((u16)x->_type_num, 8 + 8 * x->index + 6);
			//body
			memcpy(out_data + x->_offset, x->compress().begin(), x->compress().size());
		}
	);


	return *oSpan;
}
