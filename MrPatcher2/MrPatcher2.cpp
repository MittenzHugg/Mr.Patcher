#include "MrPatcher2.h"
#include <QFileDialog>
#include <QMessagebox>
#include "libdeflate.h"


//#include "stdafx.h"
//#include "Core/Assembler.h"

//#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
//#define ARMIPSNAME "ARMIPS64"
//#else
//#define ARMIPSNAME "ARMIPS"
//#endif

MrPatcher2::MrPatcher2(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	path = "";

	connect(ui.in_rom_button, SIGNAL(clicked()), this, SLOT(openROM()));
	connect(ui.file_openROM_action, SIGNAL(triggered()), this, SLOT(openROM()));
	
	connect(ui.patch_select_button, SIGNAL(clicked()), this, SLOT(openPatch()));

	connect(ui.out_rom_button, SIGNAL(clicked()), this, SLOT(selectOutROM()));
	connect(ui.patchROM_button, SIGNAL(clicked()), this, SLOT(patchROM()));

	connect(ui.export_decomp_action, SIGNAL(triggered()), this, SLOT(exportDecomp()));
}

MrPatcher2::~MrPatcher2() {
	if (patchList != nullptr){
		patchList->freeAll();
	}
}

void	MrPatcher2::openROM(void){
	in_fileName = QFileDialog::getOpenFileName(this,
		tr("Open ROM File"),
		path,
		tr("N64 ROM (*.z64 *.n64 *.v64)")
	);
	if (in_fileName == NULL) {
		return;
	}
	path = in_fileName.left(in_fileName.lastIndexOf("/"));
	out_fileName = in_fileName.left(in_fileName.lastIndexOf(".")) + ".mod.z64";
	ui.out_rom_lineEdit->setText(out_fileName);

	ui.in_rom_lineEdit->setText("");
	if (inputROM != nullptr) {
		delete inputROM;
	}
	inputROM = new n64_rom(in_fileName.toLocal8Bit().data());
	if (inputROM->gameID == UNKNOWN_GAME) {
		QMessageBox errMsg;
		errMsg.critical(0, "ERROR", "Unrecognized Game");
		ui.in_rom_lineEdit->setText("");
		return;
	}
	ui.in_rom_lineEdit->setText(in_fileName);

	switch (inputROM->gameID) {
	case BANJOKAZOOIE_NTSC:
		ui.statusBar->showMessage("Banjo-Kazooie: NTSC-U v1.0");
		break;
	case BANJOKAZOOIE_NTSC_REV1:
		ui.statusBar->showMessage("Banjo-Kazooie: NTSC-U v1.1");
		break;
	case BANJOKAZOOIE_PAL:
		ui.statusBar->showMessage("Banjo-Kazooie: PAL");
		break;
	case BANJOKAZOOIE_JP:
		ui.statusBar->showMessage("Banjo-Kazooie: JP");
		break;
	default:
		break;
	}

	//make BK objs
	switch (inputROM->gameID) {
	case BANJOKAZOOIE_NTSC:
	case BANJOKAZOOIE_NTSC_REV1:
	case BANJOKAZOOIE_PAL:
	case BANJOKAZOOIE_JP:
		if (bk_rom_mngr != nullptr) {
			delete bk_rom_mngr;
		}
		bk_rom_mngr = new bk_rom_manager(inputROM);
		ui.text_editor_action->setEnabled(true);
		//connect(ui.text_editor_action, SIGNAL(triggered()), this, SLOT(BK_TextEditor_Opened()));
		break;
	default:
		break;
	}

	//enable patch list elements
	ui.patch_select_button->setEnabled(true);
	ui.patch_line->setEnabled(true);

	ui.out_rom_button->setEnabled(true);
	ui.out_rom_lineEdit->setEnabled(true);

	ui.patchROM_button->setEnabled(true);

	ui.export_decomp_action->setEnabled(true);

	//ui.menuTools->setEnabled(true);


	return;
}

void	MrPatcher2::selectOutROM(void) {
	QString temp_fileNam = QFileDialog::getSaveFileName(this,
		tr("Select Output ROM File"),
		out_fileName,
		tr("N64 ROM (*.z64 *.n64 *.v64)")
	);
	if (temp_fileNam == NULL) return;
	out_fileName = temp_fileNam;
	ui.out_rom_lineEdit->setText(out_fileName);
}

void	MrPatcher2::openPatch(void) {

	//file dialog
	QString curr_filename = QFileDialog::getOpenFileName(this,
		tr("Open Patch File"),
		path,
		tr("All Formats (*.yml *.yaml *.gs *.asm *.s *.o *.elf *.c);;Gameshark (*.yml *.yaml *.gs);;Assembly (*.asm *.s);;Object (*.o *.elf);;C (*.c)")
	);
	if (curr_filename == NULL) {
		return;
	}

	//TODO: check patch validity;

	//new patch_obj
	int curr_row = 2;
	if (patchList != nullptr) {
		curr_row += patchList->count();
	}
	ui.patch_grid->removeWidget(ui.patch_line);
	ui.patch_grid->removeWidget(ui.patch_comboBox);
	ui.patch_grid->removeWidget(ui.patch_select_button);
	patchObj* curr_patch = new patchObj(this,ui.patch_grid, curr_row-1, curr_filename);
	ui.patch_grid->addWidget(ui.patch_line, curr_row, 0);
	ui.patch_grid->addWidget(ui.patch_comboBox, curr_row, 1);
	ui.patch_grid->addWidget(ui.patch_select_button, curr_row, 2);
	switch (inputROM->gameID) {
	case BANJOKAZOOIE_NTSC:
	case BANJOKAZOOIE_NTSC_REV1:
	case BANJOKAZOOIE_PAL:
	case BANJOKAZOOIE_JP:
		bk_rom_mngr->set_asm_combo_options(curr_patch->myComboBox);
		break;
	}

	//add_patch to patch_list
	if (patchList != nullptr) {
		patchList->add(curr_patch);
	}
	else {
		patchList = curr_patch;
		curr_patch->headPtr = &patchList;
	}

	connect(curr_patch, SIGNAL(minusClicked(int)), this, SLOT(remove_patch(int)));

	return;
}

void	MrPatcher2::remove_patch(int row) {
	int curr_row = 0;
	if (patchList != nullptr) {
		curr_row += patchList->count();
	}
	patchObj* curr_patch = patchList->get(row);
	
	patchList->remove(row);

	ui.patch_grid->removeWidget(ui.patch_line);
	ui.patch_grid->removeWidget(ui.patch_comboBox);
	ui.patch_grid->removeWidget(ui.patch_select_button);
	ui.patch_grid->addWidget(ui.patch_line, curr_row, 0);
	ui.patch_grid->addWidget(ui.patch_comboBox, curr_row, 1);
	ui.patch_grid->addWidget(ui.patch_select_button, curr_row, 2);

	return;
}

void	MrPatcher2::patchROM(void) {
	FILE* tmp_rom = NULL;
	//apply patches and pack rom
	switch (inputROM->gameID)
	{
	case BANJOKAZOOIE_NTSC:
	case BANJOKAZOOIE_PAL:
	case BANJOKAZOOIE_JP:
	case BANJOKAZOOIE_NTSC_REV1:
		if (patchList != nullptr) {
			bk_rom_mngr->applyPatches(patchList);
		}
		tmp_rom = bk_rom_mngr->packROM();
		break;
	default:
		break;
	}

	FILE* f_out = fopen(out_fileName.toLocal8Bit().data(),"wb");
	//set buffer order based on endianness
	if (out_fileName.endsWith(".z64", Qt::CaseInsensitive)) {
		uint8_t tmp;
		while (fread(&tmp, sizeof(uint8_t), (size_t)1, tmp_rom)!= 0) {
			fwrite(&tmp, sizeof(uint8_t), (size_t)1, f_out);
		}
	}
	else if (out_fileName.endsWith(".n64", Qt::CaseInsensitive)) {
		uint8_t tmp[4];
		while (fread(&tmp, sizeof(uint8_t), (size_t)4, tmp_rom)) {
			fwrite(&tmp[3], sizeof(uint8_t), (size_t)1, f_out);
			fwrite(&tmp[2], sizeof(uint8_t), (size_t)1, f_out);
			fwrite(&tmp[1], sizeof(uint8_t), (size_t)1, f_out);
			fwrite(tmp, sizeof(uint8_t), (size_t)1, f_out);
		}
	}
	else if (out_fileName.endsWith(".v64", Qt::CaseInsensitive)) {
		uint8_t tmp[2];
		while (fread(&tmp, sizeof(uint8_t), (size_t)2, tmp_rom)) {
			fwrite(&tmp[1], sizeof(uint8_t), (size_t)1, f_out);
			fwrite(tmp, sizeof(uint8_t), (size_t)1, f_out);
		}
	}
	fclose(tmp_rom);
	fclose(f_out);

};

void	MrPatcher2::exportDecomp(void) {
	QString dir = QFileDialog::getExistingDirectory(this,
		tr("Open Directory"),
		path,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (dir == NULL) return;
	
	switch (inputROM->gameID) {
	case BANJOKAZOOIE_NTSC:
	case BANJOKAZOOIE_NTSC_REV1:
	case BANJOKAZOOIE_JP:
	case BANJOKAZOOIE_PAL:
		bk_rom_mngr->export_rom(dir.toLocal8Bit().data());
		break;
	default:
		break;
	}
}

void	MrPatcher2::BK_TextEditor_Opened(void) {
	txt_editor = new QtBKTextEditor();
	txt_editor->show();
}