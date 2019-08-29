#pragma once
/* snesrc - SNES Recompiler
*
* Mar 23, 2010: addition by spinout to actually fix CRC if it is incorrect
*
* Copyright notice for this file:
*  Copyright (C) 2005 Parasyte
*
* Based on uCON64's N64 checksum algorithm by Andreas Sterbenz
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/



#include <stdint.h>

#define CRC_ROL(i, b) (((i) << (b)) | ((i) >> (32 - (b))))
#define CRC_BYTES2LONG(b) ( (b)[0] << 24 | \
                        (b)[1] << 16 | \
                        (b)[2] <<  8 | \
                        (b)[3] )

#define CRC_N64_HEADER_SIZE	0x40
#define CRC_N64_BC_SIZE		(0x1000 - CRC_N64_HEADER_SIZE)


#define CRC_N64_CRC1         0x10
#define CRC_N64_CRC2         0x14

#define CRC_CHECKSUM_START   0x00001000
#define CRC_CHECKSUM_LENGTH  0x00100000
#define CRC_CHECKSUM_CIC6102 0xF8CA4DDC
#define CRC_CHECKSUM_CIC6103 0xA3886759
#define CRC_CHECKSUM_CIC6105 0xDF26F436
#define CRC_CHECKSUM_CIC6106 0x1FEA617A

unsigned int crc_table[256];

void crc_gen_table(void);
unsigned int crc32(uint8_t *data, int len);
int crc_N64GetCIC(uint8_t *data);
int crc_N64CalcCRC(uint32_t *crc, uint8_t *data);

