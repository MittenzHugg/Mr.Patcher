/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   bk_enums.h
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

typedef enum bk_level {
	MM = 0x01,
	TTC = 0x02,
	CC = 0x03,
	BGS = 0x04,
	FP = 0x05,
	LAIR = 0x06,
	GV = 0x07,
	CWW = 0x08,
	RBB = 0x09,
	MMM = 0x0A,
	SM = 0x0B,
	FIGHT = 0x0C,
	CS = 0x0D
} bk_level_t;