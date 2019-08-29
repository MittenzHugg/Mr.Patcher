#pragma once
#include <cstdint>
#include "n64_rom.h"
#include "bk_file.h"
#include "patchObj.h"
#include <QCombobox>

#define BK_MNGR_PATCH_IN_DYNAMIC_REGION (0x01<<0)
#define BK_MNGR_PATCH_IN_BOOTLOADER (0x01<<1)
#define BK_MNGR_PATCH_IN_UNKNOWN_REGION (0x01<<2)
#define BK_MNGR_ASM_PATCH (0x01<<3)
#define BK_MNGR_C_PATCH (0x01<<4)

typedef enum bk_asm_file {
	bootloader,
	basicLibs,
	engine,
	sm,
	mm,
	ttc,
	cc,
	bgs,
	fp,
	gv,
	mmm,
	rbb,
	ccw,
	lair,
	cs,
	fight,
	expansion_pak,
} bk_asm_file_t;

class bk_rom_manager
{
public:

	n64_rom* bk_rom;
	
	bk_rom_manager(n64_rom * inROM);
	~bk_rom_manager();

	//bk_file* getFile(uint32_t fileIndex);

	uint8_t* mod_rom_buffer;

	void export_rom(char* dir);

	void set_asm_combo_options(QComboBox* cbox);
	void applyPatches(patchObj* patch_list);
	FILE* packROM(void);
	//QList<QString> comboList;
	

private:
	bk_file* asm_code[0x11] = { nullptr };
	bk_file* asm_vars[0x11] = { nullptr };

	uint8_t* dyn_file_table;
	uint32_t dyn_file_cnt;
	uint8_t* dyn_file_base;


	//functions
	uint32_t getAddressFromLI(bk_file* section, uint32_t upper, uint32_t lower);

};


