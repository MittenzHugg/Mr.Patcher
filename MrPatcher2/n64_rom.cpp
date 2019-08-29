#include "n64_rom.h"
#include "n64_rom.h"
#include "N64\md5.h"
#include <stdio.h>
#include <cstring>
#include <stdlib.h>

n64_rom::n64_rom(char* fileName)
{
	//QErrorMessage errMsg();
	int err = 0;
	if (err = fopen_s(&ROMFile, fileName, "rb")) {
		//errMsg->showMessage(tr("ERROR: Can not open %s").arg(fileName));
		return;
	}

	//correct endianess
	uint8_t ROMSignature[4];
	fread(ROMSignature, sizeof(uint8_t), 4, ROMFile);
	fseek(this->ROMFile, 0, SEEK_END);
	this->size = ftell(this->ROMFile);
	this->buffer = (uint8_t*)malloc(this->size);
	if (this->buffer == nullptr) {
		//TODO: Error Msg
		return;
	}
	rewind(this->ROMFile);
	fread(this->buffer, sizeof(uint8_t), this->size, this->ROMFile);
	if (!memcmp(ROMSignature, N64_SIGNATURE, 4 * sizeof(uint8_t))) {
		//le
		fread(this->buffer, sizeof(uint8_t), this->size, this->ROMFile);
		for (int i = 0; i < this->size; i += 4) {
			uint8_t tmp;
			tmp = this->buffer[i];
			this->buffer[i] = this->buffer[i + 3];
			this->buffer[i + 3] = tmp;
			tmp = this->buffer[i + 1];
			this->buffer[i + 1] = this->buffer[i + 2];
			this->buffer[i + 2] = tmp;
		}
	}
	else if (!memcmp(ROMSignature, V64_SIGNATURE, 4 * sizeof(uint8_t))) {
		//me
		for (int i = 0; i < this->size; i += 2) {
			uint8_t tmp;
			tmp = this->buffer[i];
			this->buffer[i] = this->buffer[i + 1];
			this->buffer[i + 1] = tmp;
		}
	}
	else if (memcmp(ROMSignature, Z64_SIGNATURE, 4 * sizeof(uint8_t))) {
		//invalid ROM
		//errMsg->showMessage(QString("ERROR: Can not open ").append(fileName));
		return;
	}//else Z64 implied

	fclose(ROMFile);


	//perform md5 hash to verify the ROM
	md5_state_t ROM_md5_state;
	md5_byte_t ROM_md5_digest[16];
	md5_init(&ROM_md5_state);
	md5_append(&ROM_md5_state, (const md5_byte_t*)this->buffer, this->size);
	md5_finish(&ROM_md5_state, ROM_md5_digest);
	if (!memcmp(ROM_md5_digest, BANJOKAZOOIE_NTSC_HASH, 16)) {
		this->gameID = BANJOKAZOOIE_NTSC;
	}
	else if (!memcmp(ROM_md5_digest, BANJOKAZOOIE_PAL_HASH, 16)) {
		this->gameID = BANJOKAZOOIE_PAL;
	}
	else if (!memcmp(ROM_md5_digest, BANJOKAZOOIE_JP_HASH, 16)) {
		this->gameID = BANJOKAZOOIE_JP;
	}
	else if (!memcmp(ROM_md5_digest, BANJOKAZOOIE_NTSC_REV1_HASH, 16)) {
		this->gameID = BANJOKAZOOIE_NTSC_REV1;
	}
	else {
		this->gameID = UNKNOWN_GAME;
		return;
	}
	return;
}


n64_rom::~n64_rom()
{
	if (buffer != nullptr) {
		free(buffer);
		buffer = nullptr;
	}
}