#include "bk_rom_manager.h"
#include "libdeflate.h"
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <Windows.h>
#include <direct.h>
#include "n64Buffer.h"
#include "bk_enums.h"
#include "gs_parser.h"
#include "crc_n64.c"
#include <QProgressDialog>
#include <QMessageBox>

#define EXP_PAK_RAM_OFFSET 0x400000

static const char* asmStrings[0x11]{
	"",
	"c_libs",
	"game_engine",
	"spiral_mountain",
	"mumbos_mountain",
	"treasure_trove_cove",
	"clankers_cavern",
	"bubblegloop_swamp",
	"freezeezy_peak",
	"gobis_valley",
	"mad_moster_mansion",
	"rusty_bucket_bay",
	"click_clock_wood",
	"gruntys_lair",
	"cutscenes",
	"final_boss"
};

bk_rom_manager::bk_rom_manager(n64_rom* inROM) {
	bk_rom = inROM;
	FILE* tempFile = NULL;

	//bootloader asm
	asm_code[bootloader] = new bk_file(0x1000, 0x4BE0, bk_rom->buffer, bk_file_types::ASM, false, 0x400);
	asm_code[bootloader]->decomp();

	//basic libraries asm
	uint32_t tmp_rom_offset = getAddressFromLI(asm_code[bootloader], 0x7A, 0x82);
	uint32_t tmp_ram_offset = getAddressFromLI(asm_code[bootloader], 0x5A, 0x66);
	uint32_t tmp_rom_size = getAddressFromLI(asm_code[bootloader], 0x7E, 0x86);
	tmp_rom_size -= tmp_rom_offset;
	asm_code[basicLibs] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM, true, tmp_ram_offset);
	asm_code[basicLibs]->decomp();

	tmp_rom_offset = asm_code[basicLibs]->rom_offset + asm_code[basicLibs]->comp_size;
	tmp_ram_offset = asm_code[basicLibs]->ram_offset + asm_code[basicLibs]->uncomp_size;
	tmp_rom_size -= asm_code[basicLibs]->comp_size;
	asm_vars[basicLibs] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM_VAR, true, tmp_ram_offset);

	//game engine asm
	tmp_rom_offset = getAddressFromLI(asm_code[bootloader], 0x17FA, 0x1822);
	tmp_ram_offset = getAddressFromLI(asm_code[basicLibs], 0x0E, 0x1A);
	tmp_rom_size = getAddressFromLI(asm_code[bootloader], 0x17FE, 0x1826);
	tmp_rom_size -= tmp_rom_offset;
	asm_code[engine] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM, true, tmp_ram_offset);
	asm_code[engine]->decomp();

	tmp_rom_offset = asm_code[engine]->rom_offset + asm_code[engine]->comp_size;
	tmp_ram_offset = asm_code[engine]->ram_offset + asm_code[engine]->uncomp_size;
	tmp_rom_size -= asm_code[engine]->comp_size;
	asm_vars[engine] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM_VAR, true, tmp_ram_offset);

	//level_asm_files_info
	const uint32_t level_upper[0x0D] = {
		0x18F2,//sm
		0x187A,//mm
		0x1872,//ttc
		0x180A,//cc
		0x1882,//bgs
		0x1892,//fp
		0x181A,//gv
		0x187A,//mmm
		0x188A,//rbb
		0x18EA,//ccw
		0x1902,//lair
		0x18FA,//cs
		0x190A//fight
	};
	for (uint32_t i = bk_asm_file::sm; i < bk_asm_file::expansion_pak; i++) {
		uint32_t upper = level_upper[i - bk_asm_file::sm];
		tmp_rom_offset = getAddressFromLI(asm_code[bootloader], upper, upper + 0x28);
		upper += 0x04;
		tmp_rom_size = getAddressFromLI(asm_code[bootloader], upper, upper + 0x28);
		tmp_rom_size -= tmp_rom_offset;
		asm_code[i] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM, true);
		asm_code[i]->decomp();
		tmp_rom_offset = asm_code[i]->rom_offset + asm_code[i]->comp_size;
		tmp_ram_offset = asm_code[i]->ram_offset + asm_code[i]->uncomp_size;
		tmp_rom_size -= asm_code[i]->comp_size;
		asm_vars[i] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM_VAR, true);
		asm_code[i]->clear_buffers();
	}

	//expansion pak
	tmp_rom_offset = getAddressFromLI(asm_code[bootloader], 0x1806, 0x1806 + 0x28);
	tmp_ram_offset = EXP_PAK_RAM_OFFSET;
	tmp_rom_size = 0;
	asm_code[expansion_pak] = new bk_file(tmp_rom_offset, tmp_rom_size, bk_rom->buffer, bk_file_types::ASM, true, tmp_ram_offset);
	asm_code[expansion_pak]->comp_size = 0x400000;
	asm_vars[expansion_pak] = nullptr;
	asm_code[bootloader]->clear_buffers();
	//rom_dynamic_files
	dyn_file_cnt = n64_get_word(bk_rom->buffer + 0x5E90);
	dyn_file_table = bk_rom->buffer + 0x5E98;
	dyn_file_base = dyn_file_table + dyn_file_cnt * 0x08;
}

bk_rom_manager::~bk_rom_manager(void) {

}

void bk_rom_manager::export_rom(char* dir) {

	if (dir == NULL) {
		return;
	}
	char odir[1024];
	struct stat st = { 0 };
	char ofp[1024];
	FILE* ofile = NULL;

	int cur_prog = 0;
	char temp_str[1024];
	QString prog_str("Exporting Decompressed ROM...\n");
	QProgressDialog prog(prog_str, "Cancel", cur_prog, dyn_file_cnt + 0xF);
	prog.setWindowModality(Qt::WindowModal);

	//EXPORT ASM
	strcpy(odir, dir);
	strcat(odir, "/asm");
	if (stat(odir, &st) == -1) {
		_mkdir(odir);
	}
	//bootloader;
	strcpy(ofp, odir);
	strcat(ofp, "/bootloader.bin");
	ofile = fopen(ofp, "wb");
	asm_code[bootloader]->decomp();
	fwrite(asm_code[bootloader]->uncomp_buffer, sizeof(uint8_t), asm_code[bootloader]->uncomp_size, ofile);
	fclose(ofile);
	asm_code[bootloader]->clear_buffers();
	prog.setValue(++cur_prog);
	if (prog.wasCanceled()) {
		return;
	}

	for (int i = bk_asm_file::basicLibs; i < bk_asm_file::expansion_pak; i++) {
		strcpy(ofp, odir);
		strcat(ofp, "/");
		strcat(ofp, asmStrings[i]);
		strcat(ofp, ".bin");
		ofile = fopen(ofp, "wb");
		asm_code[i]->decomp();
		asm_vars[i]->decomp();
		//fwrite(asm_code[i]->rom_buffer, sizeof(uint8_t), asm_code[i]->comp_size, ofile);
		//fwrite(asm_vars[i]->rom_buffer, sizeof(uint8_t), asm_vars[i]->comp_size, ofile);
		fwrite(asm_code[i]->uncomp_buffer, sizeof(uint8_t), asm_code[i]->uncomp_size, ofile);
		fwrite(asm_vars[i]->uncomp_buffer, sizeof(uint8_t), asm_vars[i]->uncomp_size, ofile);
		
		fclose(ofile);
		asm_code[i]->clear_buffers();
		prog.setValue(++cur_prog);
		if (prog.wasCanceled()) {
			return;
		}
	}
	

	//Export game assets
	bk_file dynFile;
	strcpy(odir, dir);
	strcat(odir, "/animations");
	if (stat(odir, &st) == -1) {
		_mkdir(odir);
	}
	uint16_t dynType = 0;
	for (int i = 0; i < dyn_file_cnt; i++) {
		dynType = n64_get_half(dyn_file_table + i * 0x08 + 0x06);
		if (dynType != 0x0004) {
			bk_file_type_t tmpType = UNKNOWN;
			if ((i >= 0x2CA && i < 0x71C)
				|| (i >= 0x07B6) && (i < 0xA0B)) {
				strcpy(odir, dir);
				if (dynType == 0x0000) {
					strcat(odir, "/3D_models");
					tmpType = MODEL;
				}
				else if (dynType == 0x0001) {
					strcat(odir, "/sprites");
					tmpType = SPRITE;
				}
				else if (dynType != 0x0004) {
					strcat(odir, "/unknown");
				}
				if (stat(odir, &st) == -1) {
					_mkdir(odir);
				}
			}
			else if (i == 0x71C) {
				strcpy(odir, dir);
				strcat(odir, "/level_setups/");
				tmpType = LEVEL;
				if (stat(odir, &st) == -1) {
					_mkdir(odir);
				}
			}
			else if (i == 0xA0B) {
				strcpy(odir, dir);
				strcat(odir, "/texts");
				tmpType = TEXT;
				if (stat(odir, &st) == -1) {
					_mkdir(odir);
				}
			}
			else if (i == 0x146A) {
				strcpy(odir, dir);
				strcat(odir, "/level_models");
				tmpType = LEVEL_MODEL;
				if (stat(odir, &st) == -1) {
					_mkdir(odir);
				}
			}
			else if (i == 0x1516) {
				strcpy(odir, dir);
				strcat(odir, "/music");
				tmpType = MUSIC;
				if (stat(odir, &st) == -1) {
					_mkdir(odir);
				}
			}
			uint32_t tmpAddr = n64_get_word(dyn_file_table + i * 0x08);
			dynFile.rom_offset = tmpAddr;
			dynFile.comp_size = n64_get_word(dyn_file_table + (i + 1) * 0x08) - tmpAddr;
			dynFile.rom_buffer = dyn_file_base + tmpAddr;
			dynFile.type = tmpType;
			dynFile.compressed = (bool)n64_get_half(dyn_file_table + i * 0x08 + 0x04);
			dynFile.decomp();
			
			strcpy(ofp, odir);
			strcat(ofp, "/");
			char tmpstr[256];
			itoa(i, tmpstr, 16);
			int j = 0;
			while (j < (4 - strlen(tmpstr))) {
				strcat(ofp, "0");
				j++;
			}
			for (j = 0; j < strlen(tmpstr); j++) {
				tmpstr[j] = toupper(tmpstr[j]);
			}
			strcat(ofp, tmpstr);
			strcat(ofp, ".bin");
			ofile = fopen(ofp, "wb");
			if (ofile != NULL) {
				fwrite(dynFile.uncomp_buffer, sizeof(uint8_t), dynFile.uncomp_size, ofile);
				fclose(ofile);
				dynFile.clear_buffers();
			}
			else {
				dynFile.clear_buffers();
				return;
			}
		}
		prog.setValue(++cur_prog);
		if (prog.wasCanceled()) {
			return;
		}
	}
}

void bk_rom_manager::set_asm_combo_options(QComboBox* cbox) {
	for (int i = bk_asm_file::sm; i < bk_asm_file::expansion_pak; i++) {
		cbox->addItem(QString(asmStrings[i]));
	}
	return;
}

void bk_rom_manager::applyPatches(patchObj* patch_list) {
	int patch_cnt = patch_list->count();
	uint32_t warns = 0;

	//generate static ROM section
	bk_file * patchable_files[7] = {
		asm_code[bootloader],
		asm_code[basicLibs],
		asm_vars[basicLibs],
		asm_code[engine],
		asm_vars[engine],
		nullptr,
		nullptr
	};

	for (int i = 0; i < patch_cnt; i++) {
		patchObj* curr_patch = patch_list->get(i + 1);
		
		//get associated level asm;
		int cur_lvl = curr_patch->myComboBox->currentIndex();
		patchable_files[5] = asm_code[bk_asm_file::sm + cur_lvl];
		patchable_files[6] = asm_vars[bk_asm_file::sm + cur_lvl];


		for (int j = 0; j < 7; j++) {
			patchable_files[j]->decomp();
		}
		asm_code[expansion_pak]->uncomp_buffer = (uint8_t*)calloc(sizeof(uint8_t),0x400000);

		if (patch_list->myFileName.endsWith(".yml", Qt::CaseInsensitive)
			|| patch_list->myFileName.endsWith(".yaml", Qt::CaseInsensitive)
			|| patch_list->myFileName.endsWith(".gs", Qt::CaseInsensitive)) {
			
			gs_parser gsp(patch_list->myFileName.toLocal8Bit().data());
			uint32_t tmp_addr;
			uint8_t tmp_val;
			while (gsp.get_byte(&tmp_addr, &tmp_val) != -1) {
				//
				//if code is in expansion pak
				if (tmp_addr >= (EXP_PAK_RAM_OFFSET+0x100) && (tmp_addr < 0x800000 )) {
					//write patched byte
					uint32_t f_offset = tmp_addr - asm_code[expansion_pak]->ram_offset;
					asm_code[expansion_pak]->uncomp_buffer[f_offset] = tmp_val;
					asm_code[expansion_pak]->modified = true;
					//change size
					if (f_offset > asm_code[expansion_pak]->uncomp_size) {
						asm_code[expansion_pak]->uncomp_size = f_offset;
					}
				}
				else {
					//find file with
					bk_file * cur_f = nullptr;
					for (int j = 0; j < 7 && cur_f == nullptr ; j++) {
						if (tmp_addr > patchable_files[j]->ram_offset) {
							uint32_t f_offset = tmp_addr - patchable_files[j]->ram_offset;
							if (f_offset < patchable_files[j]->uncomp_size) {
								cur_f = patchable_files[j];
							}
						}
					}
					//write patched bytes
					if (cur_f != nullptr) {
						uint32_t f_offset = tmp_addr - cur_f->ram_offset;
						cur_f->uncomp_buffer[f_offset] = tmp_val;
						cur_f->modified = true;
					}
					else{
						if ((tmp_addr >= (asm_code[bootloader]->ram_offset + asm_code[bootloader]->uncomp_size)) && (tmp_addr < asm_code[basicLibs]->ram_offset)) {
							warns |= BK_MNGR_PATCH_IN_DYNAMIC_REGION;
						}
						else if (tmp_addr >= EXP_PAK_RAM_OFFSET && (tmp_addr < EXP_PAK_RAM_OFFSET + 0x100)) {
							warns |= BK_MNGR_PATCH_IN_BOOTLOADER;
						}
						else {
							warns |= BK_MNGR_PATCH_IN_UNKNOWN_REGION;
						}
					}
				}
			}
		}
		else if (patch_list->myFileName.endsWith(".asm", Qt::CaseInsensitive)
			|| patch_list->myFileName.endsWith(".s", Qt::CaseInsensitive)
			|| patch_list->myFileName.endsWith(".o", Qt::CaseInsensitive)) {
			//generate .asm header for armmips
			//TODO: armmips
			warns |= BK_MNGR_ASM_PATCH;
		}
		else if (patch_list->myFileName.endsWith(".c")) {
			//TODO: compile c to 
			warns |= BK_MNGR_C_PATCH;
		}
	}
	if (warns != 0) {
		QString warn_msg("");
		if (warns & BK_MNGR_PATCH_IN_DYNAMIC_REGION) {
			warn_msg.append("WARNING: a patch contains byte(s) written to dynamic RAM section. Incompatible byte(s) ignored.\n");
		}
		if (warns & BK_MNGR_PATCH_IN_BOOTLOADER) {
			warn_msg.append("WARNING: a patch contains byte(s) written between 0x80400000 and 0x80400100 allocated for bootloader. Incompatible byte(s) ignored.\n");
		}
		if (warns & BK_MNGR_PATCH_IN_UNKNOWN_REGION) {
			warn_msg.append("WARNING: a patch contains byte(s) written to unknown RAM section. Incompatible byte(s) ignored.\n");
		}
		if (warns & BK_MNGR_ASM_PATCH) {
			warn_msg.append("WARNING: Mr Patcher does not yet support assembly patches due to slow & lazy devs. Incompatible patch(es) ignored.\n");
		}
		if (warns & BK_MNGR_C_PATCH) {
			warn_msg.append("WARNING: Mr Patcher does not yet support C patches due to slow & lazy devs. Incompatible patch(es) ignored.\n");
		}
		QMessageBox msgBox(QMessageBox::Warning, QString("Patch Warning"),warn_msg, QMessageBox::Ok);
		msgBox.exec();
	}

	//modify stick speed to ignore checksum
	if (asm_code[engine]->modified) {
		//Rare Devs are artful dicks.
		uint32_t crc1;
		uint32_t crc2;
		asm_code[engine]->get_checksums(&crc1,&crc2);
		
		uint32_t eng_var_off;
		switch (bk_rom->gameID)
		{
		case BANJOKAZOOIE_NTSC:
		case BANJOKAZOOIE_NTSC_REV1:
			eng_var_off = 0xF264;
			break;
		case BANJOKAZOOIE_PAL:
			eng_var_off = 0xF374;
			break;
		case BANJOKAZOOIE_JP:
			eng_var_off = 0xFA24;
			break;
		default:
			break;
		}

		n64_set_word(asm_vars[engine]->uncomp_buffer + eng_var_off, crc2);
		asm_vars[engine]->modified = true;
	}
	if (asm_vars[engine]->modified) {
		//Rare Devs are artful dicks.
		uint32_t crc1;
		uint32_t crc2;
		asm_vars[engine]->get_checksums(&crc1, &crc2);

		uint32_t libs_var_off;
		switch (bk_rom->gameID)
		{
		case BANJOKAZOOIE_NTSC:
			libs_var_off = 0xF64;
			break;
		case BANJOKAZOOIE_NTSC_REV1:
		case BANJOKAZOOIE_PAL:
		case BANJOKAZOOIE_JP:
			libs_var_off = 0xE54;
			break;
		default:
			break;
		}

		n64_set_word(asm_vars[basicLibs]->uncomp_buffer + libs_var_off, crc2);
		asm_vars[basicLibs]->modified = true;
	
	}
	if (asm_code[basicLibs]->modified) {
		//Rare Devs are artful dicks.
		uint32_t crc1;
		uint32_t crc2;
		asm_code[basicLibs]->get_checksums(&crc1, &crc2);

		uint32_t rom_off = 0x5E78;

		n64_set_word(bk_rom->buffer + rom_off, crc1);
		n64_set_word(bk_rom->buffer + rom_off + 0x04, crc2);
	}
	if (asm_vars[basicLibs]->modified) {
		//Rare Devs are artful dicks.
		uint32_t crc1;
		uint32_t crc2;
		asm_vars[basicLibs]->get_checksums(&crc1, &crc2);

		uint32_t rom_off = 0x5E80;

		n64_set_word(bk_rom->buffer + rom_off, crc1);
		n64_set_word(bk_rom->buffer + rom_off + 0x04, crc2);
	}
	//CUSTOM BOOT CODE FOR EXPANSION PAK CODE
	if (asm_code[expansion_pak]->modified) {
		//change bootloader
		//move original boot to start of expansion;
		memcpy(asm_code[expansion_pak]->uncomp_buffer, asm_code[bootloader]->uncomp_buffer + 0x50, 0x0F4);
		n64_set_half(asm_code[expansion_pak]->uncomp_buffer + 0x20, 0);
		n64_set_half(asm_code[expansion_pak]->uncomp_buffer + 0x22, 0);

		//get expansion_pak compressed size
		asm_code[expansion_pak]->comp();
		
		//set expansion boot jump location
		asm_code[bootloader]->uncomp_buffer[0x5B] = 0x40;
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0x66, 0x0000);

		uint16_t tempUpper = (uint16_t)((asm_code[expansion_pak]->rom_offset & 0xFFFF0000) >> 16);
		uint16_t tempLower = (uint16_t)((asm_code[expansion_pak]->rom_offset) & 0x0000FFFF);
		if (tempLower > (0x7FFF)) {
			tempUpper += 1;
		}
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0x7A, tempUpper);
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0x82, tempLower);

		uint32_t ep_rom_end = asm_code[expansion_pak]->rom_offset + asm_code[expansion_pak]->mod_comp_size;
		tempUpper = (uint16_t)((ep_rom_end & 0xFFFF0000) >> 16);
		tempLower = (uint16_t)((ep_rom_end) & 0x0000FFFF);
		if (tempLower > (0x7FFF)) {
			tempUpper += 1;
		}
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0x7E, tempUpper);
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0x86, tempLower);

		//remove second decomp
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0xF8, 0);
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0xFA, 0);

		//////set jump location
		asm_code[bootloader]->uncomp_buffer[0x127] = 0x40;
		n64_set_half(asm_code[bootloader]->uncomp_buffer + 0x12A, 0x0000);
		asm_code[bootloader]->modified = true;
	}

	return;
}

FILE* bk_rom_manager::packROM(void) {
	FILE* tmp_rom = tmpfile();
	//copy original ROM to file;
	fwrite(bk_rom->buffer, sizeof(uint8_t), bk_rom->size, tmp_rom);
	
	//step through all asm files;
	for (int i = bk_asm_file::bootloader; i <= bk_asm_file::expansion_pak; i++) {
		if (asm_code[i] != nullptr) {
			if (asm_code[i]->modified) {
				//overwrite ROM
				if (i != bk_asm_file::expansion_pak) {
					asm_code[i]->comp();
				}
				else {
					asm_code[i]->comp(true);
				}
				fseek(tmp_rom, asm_code[i]->rom_offset, SEEK_SET);
				fwrite(asm_code[i]->mod_buffer, sizeof(uint8_t), asm_code[i]->mod_comp_size, tmp_rom);
				if (asm_vars[i] != nullptr) {
					//shift varaiables to immediately follow code
					asm_vars[i]->rom_offset = asm_code[i]->rom_offset + asm_code[i]->mod_comp_size;
				}
			}
			else {
				asm_code[i]->mod_comp_size = asm_code[i]->comp_size;
			}
			asm_code[i]->clear_buffers();
		}
		if (asm_vars[i] != nullptr) {
			if (!(asm_vars[i]->modified)) {
				asm_vars[i]->mod_buffer = (uint8_t*)malloc(asm_vars[i]->comp_size);
				memcpy(asm_vars[i]->mod_buffer, asm_vars[i]->rom_buffer, asm_vars[i]->comp_size);
				asm_vars[i]->mod_comp_size = asm_vars[i]->comp_size;
			}
			else {
				asm_vars[i]->comp();
			}
			fseek(tmp_rom, asm_vars[i]->rom_offset, SEEK_SET);
			fwrite(asm_vars[i]->mod_buffer, sizeof(uint8_t), asm_vars[i]->mod_comp_size, tmp_rom);
			asm_vars[i]->clear_buffers();
			//pad to original combined size
			uint32_t shrinkage = (asm_code[i]->comp_size + asm_vars[i]->comp_size) - (asm_code[i]->mod_comp_size + asm_vars[i]->mod_comp_size);
			uint8_t* padding = (uint8_t*)calloc(shrinkage, sizeof(uint8_t));
			fwrite(padding, sizeof(uint8_t), shrinkage, tmp_rom);
			free(padding);

		}
	}
	
	//step through all asset files;
	
	//fix crc
	crc_gen_table();
	uint8_t* crcBuffer;
	crcBuffer = (uint8_t*)malloc(CRC_CHECKSUM_START + CRC_CHECKSUM_LENGTH);

	rewind(tmp_rom);
	fread(crcBuffer, sizeof(uint8_t), CRC_CHECKSUM_START + CRC_CHECKSUM_LENGTH, tmp_rom);
	int cic = crc_N64GetCIC(crcBuffer);

	uint32_t crc[2];
	uint8_t crc1[4];
	uint8_t crc2[4];
	if (crc_N64CalcCRC(crc, crcBuffer)) {
		//fprintf(stderr, "ERROR: Unable to calculate CRC\n\r");
	}
	else {
		for (int temp = 0; temp < 4; temp++) {
			crc1[temp] = (uint8_t)((0xFF << (8 * (3 - temp)) & crc[0]) >> (8 * (3 - temp)));
			crc2[temp] = (uint8_t)((0xFF << (8 * (3 - temp)) & crc[1]) >> (8 * (3 - temp)));
		}
		if (memcmp(crc1, &crcBuffer[CRC_N64_CRC1],4*sizeof(uint8_t))){
			fseek(tmp_rom, CRC_N64_CRC1, SEEK_SET);
			fwrite(crc1, sizeof(uint8_t), 4, tmp_rom);
			//	printf("(Bad, fixed)\n");
		}
		if (memcmp(crc2, &crcBuffer[CRC_N64_CRC2], 4 * sizeof(uint8_t))) {
			fseek(tmp_rom, CRC_N64_CRC2, SEEK_SET);
			fwrite(crc2, sizeof(uint8_t), 4, tmp_rom);
			//	printf("(Bad, fixed)\n");
		}
	}
	free(crcBuffer);
	rewind(tmp_rom);
	return tmp_rom;
}

uint32_t bk_rom_manager::getAddressFromLI(bk_file* section, uint32_t upper, uint32_t lower) {
	uint32_t tmp = (uint32_t)n64_get_half(section->uncomp_buffer + upper);
	tmp = tmp << 0x10;
	int16_t tmp2 = (int16_t)n64_get_half(section->uncomp_buffer + lower);
	return (uint32_t)((int32_t)tmp + tmp2);
}