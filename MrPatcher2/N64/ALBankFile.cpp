/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   ALBankFile.cpp
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
#include "ALBankFile.h"
#include <algorithm>
#include <execution>




ALBankFile::ALBankFile(const n64_span& albankfile, const n64_span& alwavefile)
{
	u32 offset = 0;
	revision = albankfile.seq_get<s16>(offset);
	bankCount = albankfile.seq_get<s16>(offset);
	banks.reserve(bankCount);
	for (auto i = 0; i < bankCount; i++) {
		banks.push_back(ALBank(albankfile, albankfile.seq_get<u32>(offset), alwavefile));
	}
}

ALBank::ALBank(const n64_span& albankfile, u32 offset, const n64_span& alwavefile)
{
	u32 _offset = offset;
	instCount = albankfile.seq_get<s16>(_offset);
	flags = albankfile.seq_get<u8>(_offset);
	pad = albankfile.seq_get<u8>(_offset);
	sampleRate = albankfile.seq_get<s32>(_offset);
	percussion = ALInstrument(albankfile, albankfile.seq_get<u32>(_offset), alwavefile);
	for (auto i = 0; i < instCount; i++) {
		instArray.push_back(ALInstrument(albankfile, albankfile.seq_get<u32>(_offset), alwavefile));
	}
}

ALInstrument::ALInstrument(const n64_span& albankfile, u32 offset, const n64_span& alwavefile)
{
	u32 _offset = offset;
	volume = albankfile.seq_get<u8>(_offset);
	pan = albankfile.seq_get<u8>(_offset);
	priority = albankfile.seq_get<u8>(_offset);
	flags = albankfile.seq_get<u8>(_offset);
	tremType = albankfile.seq_get<u8>(_offset);
	tremRate = albankfile.seq_get<u8>(_offset);
	tremDepth = albankfile.seq_get<u8>(_offset);
	tremDelay = albankfile.seq_get<u8>(_offset);
	vibType = albankfile.seq_get<u8>(_offset);
	vibRate = albankfile.seq_get<u8>(_offset);
	vibDepth = albankfile.seq_get<u8>(_offset);
	vibDelay = albankfile.seq_get<u8>(_offset);
	bendRange = albankfile.seq_get<s16>(_offset);
	soundCount = albankfile.seq_get<s16>(_offset);
	for (auto i = 0; i < soundCount; i++) {
		soundArray.push_back(ALSound(albankfile, albankfile.seq_get<u32>(_offset), alwavefile));
	}
}

ALSound::ALSound(const n64_span& albankfile, u32 offset, const n64_span& alwavefile)
{
	u32 _offset = offset;
	envelope = ALEnvelope(albankfile, albankfile.seq_get<u32>(_offset));
	keyMap = ALKeyMap(albankfile, albankfile.seq_get<u32>(_offset));
	wavetable = ALWaveTable(albankfile, albankfile.seq_get<u32>(_offset), alwavefile);
	samplePan = albankfile.seq_get<u8>(_offset);
	sampleVolume = albankfile.seq_get<u8>(_offset);
	flags = albankfile.seq_get<u8>(_offset);
}

ALEnvelope::ALEnvelope(const n64_span& albankfile, u32 offset)
{
	u32 _offset = offset;
	attackTime = albankfile.seq_get<s32>(_offset);
	decayTime = albankfile.seq_get<s32>(_offset);
	releaseTime = albankfile.seq_get<s32>(_offset);
	attackVolume = albankfile.seq_get<u8>(_offset);
	decayVolume = albankfile.seq_get<u8>(_offset);
}

ALKeyMap::ALKeyMap(const n64_span& albankfile, u32 offset)
{
	u32 _offset = offset;
	velocityMin = albankfile.seq_get<u8>(_offset);
	velocityMax = albankfile.seq_get<u8>(_offset);
	keyMin = albankfile.seq_get<u8>(_offset);
	keyMax = albankfile.seq_get<u8>(_offset);
	keyBase = albankfile.seq_get<u8>(_offset);
	detune = albankfile.seq_get<s8>(_offset);
}

ALWaveTable::ALWaveTable(const n64_span& albankfile, u32 offset, const n64_span& alwavefile)
{
	u32 _offset = offset;
	base = albankfile.seq_get<u32>(_offset);
	len = albankfile.seq_get<s32>(_offset);
	wave = alwavefile.slice(base, len);
	type = albankfile.seq_get<u8>(_offset);
	flags = albankfile.seq_get<u8>(_offset);
}
