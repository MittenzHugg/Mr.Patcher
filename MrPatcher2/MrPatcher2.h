/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   MrPatcher2.h
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

#include <QtWidgets/QMainWindow>
#include <QString>
#include "ui_MrPatcher2.h"
#include "n64_rom.h"
#include "bk_rom_manager.h"
#include "bk_rom.h"
#include "patchObj.h"
#include "QtBKTextEditor.h"
#include "QtModelViewWidget.h"
#include "MrP_project.h"

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

	MrP_project* proj = nullptr;
	QString& proj_path = QString("");

	n64_rom* inputROM = nullptr;
	n64_rom* outputROM = nullptr;
	
	bk_rom_manager* bk_rom_mngr = nullptr;
	bk_rom* bkROM = nullptr;

	patchObj* patchList = nullptr;

	QtBKTextEditor* txt_editor;
	QtModelViewWidget m_view;

public slots:
	void	openROM(void);
	void	selectOutROM(void);
	void	patchROM(void);
	void	openPatch(void);
	void	remove_patch(int row);
	void    exportDecomp(void);
	void	BK_TextEditor_Opened(void);

	void	ProjSaveAs(void);
	void	ProjSave(void);
};
