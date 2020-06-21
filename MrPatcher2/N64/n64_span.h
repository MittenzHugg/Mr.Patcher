/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   n64_span.h
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
 * n64_span.h
 **********************************************************
 * N64 Span:
 *     This class represents a span of big endian memory.
 *     Think of it as a "view" of a memory section.
 *     Does not convey any ownership of the object
 * Author: Mittenz 5/15/2020
 **********************************************************/
#pragma once
#include <type_traits>
#include "simpTypes.h"

class n64_span
{
public:
	n64_span() = default;
	n64_span(const n64_span& src)
		:_begin(src._begin)
		, _end(src._end){}
	n64_span(u8* const begin,  u8* const end)
		: _begin(begin)
		, _end(end) {}
	n64_span(u8* const begin, const size_t size) 
		: _begin(begin)
		, _end(begin + size) {}

	inline u8* const begin(void) const { return _begin; }
	inline u8* const end(void) const { return _end; }
	inline const size_t size(void) const { return _end - _begin; }
	
	const u8& operator[](int index) const { if (index >= size())exit(0); return _begin[index]; }
	u8& operator[](int index) { if (index >= size())exit(0); return _begin[index]; }

	n64_span& operator = (const n64_span& other) {
		if (this != &other) {
			_begin = other._begin;
			_end = other._end;
		}
		return *this;
	}

	inline const n64_span slice(u32 offset, size_t size = -1) const{
		return n64_span(_begin + offset, size);
	}

	//return fundemental types from span
	template <typename T> 
	inline T get(u32 offset = 0) const {
		return n64_be<T>::get(*this, offset);
	}

	//return fundemental types from span and advances the offset to next value
	template <typename T> 
	T seq_get(u32& offset) const {
		T ret_val = n64_be<T>::get(*this, offset);
		offset += sizeof(T);
		return ret_val;
	}

	//write fundemental types to span
	template <typename T> void set(T value, u32 offset = 0) {
		if (!std::is_fundamental<T>::value)
			return;
		u8 size = sizeof(T);
		u8* start = begin() + offset;
		T tmp = value;
		while (size) {
			size--;
			start[size] = (u8)(tmp & 0xFF);
			tmp = tmp >> 8;
		}
		return;
	}

	template <typename T> T seq_set(T value, u32& offset){
		set(value, offset);
		offset += sizeof(T);
		return ret_val;
	}


private:
	u8* _begin = nullptr;
	u8* _end = nullptr;
};

//big endian helper struct
template<typename T> 
struct n64_be{
	static T get(const n64_span& const span, u32 offset) {
		if (std::is_fundamental<T>::value) {
			u8 size = sizeof(T);
			u8 count = 1;
			u8* start = span.begin() + offset;
			T value = (T)(start[0]);
			while (count < size) {
				value = value << 8;
				value |= (T)start[count];
				count++;
			}
			return value;
		}
		else
			return NULL;
	}
};