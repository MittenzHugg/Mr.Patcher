/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   n64_file.cpp
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

/**********************************************************
 * n64_file.cpp
 **********************************************************
* N64 File:
 *     This class represents a compressible file inside
 * an N64 ROM. If the _comp_method() and _decomp_method()
 * is not overwritten by the derived class, both compress()
 * & decompress() will return the same span.
 *
 *     Compression/decompression is performed lazily. To
 * re-evaluate a compression/decompression a new n64_file
 * needs to be created.
 *
 * Author: Mittenz 5/15/2020
 **********************************************************/
#include <cstring>
#include "n64_file.h"


//move constructor
n64_file::n64_file(n64_file&& src) : _comp_buffer{src._comp_buffer}
	, _comp_own{src._comp_own}
	, _decomp_buffer{src._decomp_buffer}
	, _decomp_own{src._decomp_own}
{
	src._comp_own = false;
	src._decomp_own = false;
}

n64_file::n64_file(const n64_file& src)
	: _comp_own{ src._comp_own }
	, _decomp_own{ src._decomp_own }
{
	if (_comp_own) {
		u8* tmp_buffer = (u8*)malloc(src._comp_buffer->size());
		memcpy(tmp_buffer, src._comp_buffer->begin(), src._comp_buffer->size());
		this->_comp_buffer = new n64_span(tmp_buffer, src._comp_buffer->size());
	}
	else {
		this->_comp_buffer = src._comp_buffer;
	}

	if(_decomp_own) {
		u8* tmp_buffer = (u8*)malloc(src._decomp_buffer->size());
		memcpy(tmp_buffer, src._decomp_buffer->begin(), src._decomp_buffer->size());
		this->_decomp_buffer = new n64_span(tmp_buffer, src._decomp_buffer->size());
	}
	else {
		this->_decomp_buffer = src._decomp_buffer;
	}
}


//references existing buffer
n64_file::n64_file(const n64_span& file, bool comp, bool own)
{
	( comp ? _comp_buffer : _decomp_buffer) = new n64_span(file);
	( comp ? _comp_own : _decomp_own) = own;
}

n64_file::n64_file(u8* rom_ptr, u32 offset, u32 size, bool comp, bool own)
{
	( comp ? _comp_buffer : _decomp_buffer) = new n64_span(rom_ptr + offset, (size_t)size);
	( comp ? _comp_own : _decomp_own) = own;
}

n64_file::~n64_file()
{
	if (_comp_own) 
		free(_comp_buffer->begin());
	if (_decomp_own) 
		free(_decomp_buffer->begin());
	if (_comp_buffer != nullptr && _comp_buffer != _decomp_buffer)
		delete _comp_buffer;
	if (_decomp_buffer != nullptr)
		delete _comp_buffer;
}

const n64_span& const n64_file::decompress(void) const {
	if (_decomp_buffer == nullptr) {
		if (_comp_buffer == nullptr)
			throw "N64 File data never initialized";
		_decomp_method();
	}
	if (_decomp_buffer == nullptr)
		throw "Unable to decompress file";
	return *_decomp_buffer;
}

const n64_span& const n64_file::compress(void) const{
	if (_comp_buffer == nullptr) {
		if (_decomp_buffer == nullptr)
			throw "N64 File data never initialized";
		_comp_method();
	}
	if (_comp_buffer == nullptr)
		throw "Unable to compress file";
	return *_comp_buffer;
}

void n64_file::_comp_method() const{
	_comp_buffer = _decomp_buffer;
	return;
}

void n64_file::_decomp_method() const {
	_decomp_buffer = _comp_buffer;
	return;
}