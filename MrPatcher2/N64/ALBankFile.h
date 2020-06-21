/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   ALBankFile.h
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
#include "simpTypes.h"
#include "n64_span.h"
#include <vector>

class ALEnvelope
{
public:
    ALEnvelope() = default;
    ALEnvelope(const n64_span& albankfile, u32 offset);
    s32 attackTime;
    s32 decayTime;
    s32 releaseTime;
    u8  attackVolume;
    u8  decayVolume;
};

class ALKeyMap
{
public:
    ALKeyMap() = default;
    ALKeyMap(const n64_span& albankfile, u32 offset);
    u8 velocityMin;
    u8 velocityMax;
    u8 keyMin;
    u8 keyMax;
    u8 keyBase;
    s8 detune;

};

class ALWaveTable
{
public:
    ALWaveTable() = default;
    ALWaveTable(const n64_span& albankfile, u32 offset, const n64_span& alwavefile);
    n64_span wave;
    u32 base;
    s32 len;
    u8  type;
    u8  flags;
};

class ALSound
{
public:
    ALSound(const n64_span& albankfile, u32 offset, const n64_span& alwavefile);
    ALEnvelope envelope;
    ALKeyMap keyMap;
    ALWaveTable wavetable;
    u8          samplePan;
    u8          sampleVolume;
    u8          flags;
};

class ALInstrument
{
public:
    ALInstrument() = default;
	ALInstrument(const n64_span& albankfile, u32 offset, const n64_span& alwavefile);
    u8          volume; 
    u8          pan;           
    u8          priority;       
    u8          flags;
    u8          tremType;       
    u8          tremRate;       
    u8          tremDepth;      
    u8          tremDelay;     
    u8          vibType;        
    u8          vibRate;        
    u8          vibDepth;       
    u8          vibDelay;       
    s16         bendRange;      
    s16         soundCount; 
    std::vector<ALSound> soundArray;
};

class ALBank
{
public:
    ALBank(const n64_span& albankfile, u32 offset, const n64_span& alwavefile);
    s16 instCount;
    u8 flags;
    u8 pad;
    s32 sampleRate;
    ALInstrument percussion;
    std::vector<ALInstrument> instArray;
};

class ALBankFile
{
public:
    ALBankFile(const n64_span& albankfile, const n64_span& alwavefile);
    s16 revision;
    s16 bankCount;
    std::vector<ALBank> banks;
};