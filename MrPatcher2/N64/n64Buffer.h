/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   n64Buffer.h
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
#include <cstdint>

uint32_t n64_get_word(uint8_t* src) {
	return (((uint32_t)src[0]) << 0x18) | (((uint32_t)src[1]) << 0x10) | (((uint32_t)src[2]) << 0x08) | ((uint32_t)src[3]);
};

uint16_t n64_get_half(uint8_t* src) {
	return (((uint16_t)src[0]) << 0x8) | ((uint16_t)src[1]);
};

void n64_set_half(uint8_t* dst, uint16_t src) {
	*dst = (uint8_t)((src & 0xFF00) >> 8);
	*(dst + 1) = (uint8_t)(src & 0x00FF);
	return;
};

void n64_set_word(uint8_t* dst, uint32_t src) {
	*dst = (uint8_t)((src & 0xFF000000) >> 24);
	*(dst + 1) = (uint8_t)((src & 0x00FF0000) >> 16);
	*(dst + 2) = (uint8_t)((src & 0x0000FF00) >> 8);
	*(dst + 3) = (uint8_t)(src & 0x000000FF);
	return;
};