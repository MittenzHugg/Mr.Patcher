#pragma once

#include <QtWidgets/QMainWindow>
#include <QString>
#include "ui_MrPatcher2.h"
#include "n64_rom.h"
#include "bk_rom_manager.h"
#include "patchObj.h"
#include "QtBKTextEditor.h"

class MrPatcher2 : public QMainWindow
{
	Q_OBJECT

public:
	MrPatcher2(QWidget *parent = Q_NULLPTR);
	~MrPatcher2();

private:
	Ui::MrPatcher2Class ui;
	
	QString in_fileName;
	QString path;
	QString out_fileName;

	n64_rom* inputROM = nullptr;
	n64_rom* outputROM = nullptr;
	
	bk_rom_manager* bk_rom_mngr = nullptr;

	patchObj* patchList = nullptr;

	QtBKTextEditor* txt_editor;

public slots:
	void	openROM(void);
	void	selectOutROM(void);
	void	patchROM(void);
	void	openPatch(void);
	void	remove_patch(int row);
	void    exportDecomp(void);
	void	BK_TextEditor_Opened(void);
};
