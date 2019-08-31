#pragma once
#include <cstdint>

typedef enum bk_file_types {
	ASM,
	ASM_VAR,
	MODEL,
	SPRITE,
	TEXT,
	LEVEL,
	LEVEL_MODEL,
	MUSIC,
	UNKNOWN,
} bk_file_type_t;

class bk_file {
public:
	uint32_t rom_offset;
	size_t comp_size;
	uint8_t* rom_buffer = nullptr;
	bool compressed;
	uint32_t ram_offset;
	size_t uncomp_size;
	uint8_t* uncomp_buffer = nullptr;
	bool modified;
	uint32_t mod_ROMOffset;
	size_t mod_comp_size;
	bk_file_type_t type;
	uint8_t* mod_buffer = nullptr;

	bk_file();
	bk_file(uint32_t offset, uint32_t size, uint8_t* rom_ptr, bk_file_type_t t, bool comp, uint32_t ram_off = 0);
	~bk_file();
	
	uint8_t* decomp(void);
	uint8_t* comp(bool pad = false);
	void clear_buffers(void);
	void get_checksums(uint32_t* crc1, uint32_t* crc2);

};
