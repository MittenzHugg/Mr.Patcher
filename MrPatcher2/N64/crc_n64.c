/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   crc_n64.cpp
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
#include "crc_n64.h"



void crc_gen_table(void) {
	unsigned int crc, poly;
	int	i, j;

	poly = 0xEDB88320;
	for (i = 0; i < 256; i++) {
		crc = i;
		for (j = 8; j > 0; j--) {
			if (crc & 1) crc = (crc >> 1) ^ poly;
			else crc >>= 1;
		}
		crc_table[i] = crc;
	}
}

unsigned int crc32(uint8_t *data, int len){
	unsigned int crc = ~0;
	int i;

	for (i = 0; i < len; i++) {
		crc = (crc >> 8) ^ crc_table[(crc ^ data[i]) & 0xFF];
	}

	return ~crc;
}
;
int crc_N64GetCIC(uint8_t *data){
	switch (crc32(&data[CRC_N64_HEADER_SIZE], CRC_N64_BC_SIZE)) {
	case 0x6170A4A1: return 6101;
	case 0x90BB6CB5: return 6102;
	case 0x0B050EE0: return 6103;
	case 0x98BC2C86: return 6105;
	case 0xACC8580A: return 6106;
	}

	return 6105;
}
int crc_N64CalcCRC(uint32_t *crc, uint8_t *data){
	int bootcode, i;
	unsigned int seed;

	unsigned int t1, t2, t3;
	unsigned int t4, t5, t6;
	unsigned int r, d;


	switch ((bootcode = crc_N64GetCIC(data))) {
	case 6101:
	case 6102:
		seed = CRC_CHECKSUM_CIC6102;
		break;
	case 6103:
		seed = CRC_CHECKSUM_CIC6103;
		break;
	case 6105:
		seed = CRC_CHECKSUM_CIC6105;
		break;
	case 6106:
		seed = CRC_CHECKSUM_CIC6106;
		break;
	default:
		return 1;
	}

	t1 = t2 = t3 = t4 = t5 = t6 = seed;

	i = CRC_CHECKSUM_START;
	while (i < (CRC_CHECKSUM_START + CRC_CHECKSUM_LENGTH)) {
		d = CRC_BYTES2LONG(&data[i]);
		if ((t6 + d) < t6) t4++;
		t6 += d;
		t3 ^= d;
		r = CRC_ROL(d, (d & 0x1F));
		t5 += r;
		if (t2 > d) t2 ^= r;
		else t2 ^= t6 ^ d;

		if (bootcode == 6105) t1 += CRC_BYTES2LONG(&data[CRC_N64_HEADER_SIZE + 0x0710 + (i & 0xFF)]) ^ d;
		else t1 += t5 ^ d;

		i += 4;
	}
	if (bootcode == 6103) {
		crc[0] = (t6 ^ t4) + t3;
		crc[1] = (t5 ^ t2) + t1;
	}
	else if (bootcode == 6106) {
		crc[0] = (t6 * t4) + t3;
		crc[1] = (t5 * t2) + t1;
	}
	else {
		crc[0] = t6 ^ t4 ^ t3;
		crc[1] = t5 ^ t2 ^ t1;
	}

	return 0;
}

