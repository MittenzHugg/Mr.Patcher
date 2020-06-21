/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   patchObj.h
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

#include <QObject>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QToolButton>

class patchObj : public QObject
{
	Q_OBJECT

public:
	patchObj(QObject *parent, QGridLayout* grid, int row, QString fileName);
	~patchObj();

	

	patchObj* prev;
	patchObj* next;
	patchObj** headPtr;

	//QList<QString>comboList;

	std::list<patchObj*>& toList(void);
	void shiftUp(void);
	void freeAll(void);
	void add(patchObj* baby);
	patchObj* get(int row);
	void remove(int row);
	int count(void);

	QString myFileName;
	QGridLayout* ui_grid;
	QLineEdit* myLine;
	QComboBox* myComboBox;
	QToolButton* myButton;
	int myRow;

private:

public slots:
	void myButtonClicked(void);

signals:
	void minusClicked(int row);

};
