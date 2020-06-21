/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*  patchObj.cpp
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

#include "patchObj.h"
#include <qfiledialog.h>
#include <list>

patchObj::patchObj(QObject* parent, QGridLayout* grid, int row, QString fileName)
	: QObject(parent)
{
	myComboBox = false;
	prev = nullptr;
	next = nullptr;

	headPtr = nullptr;

	ui_grid = grid;
	myRow = row;

	myFileName = fileName;

	//spawn lineEdit
	myButton = new QToolButton();
	myButton->setText("-");


	myComboBox = new QComboBox();
	myLine = new QLineEdit();
	myLine->setReadOnly(true);
	myLine->setText(fileName);

	ui_grid->addWidget(myLine, myRow, 0);
	ui_grid->addWidget(myComboBox, myRow, 1);
	ui_grid->addWidget(myButton, myRow, 2);

	connect(myButton, SIGNAL(clicked()), this, SLOT(myButtonClicked()));
}

patchObj::~patchObj()
{
	ui_grid->removeWidget(myLine);
	ui_grid->removeWidget(myComboBox);
	ui_grid->removeWidget(myButton);
	delete myButton;
	delete myComboBox;
	delete myLine;
}

std::list<patchObj*>& patchObj::toList(void) {
	if (this->next == nullptr) 
		return std::list<patchObj*>{this};
	else {
		auto out_list = next->toList();
		out_list.push_front(this);
		return out_list;
	}
}

void patchObj::shiftUp(void){
	myRow--;
	ui_grid->removeWidget(myLine);
	ui_grid->removeWidget(myComboBox);
	ui_grid->removeWidget(myButton);
	ui_grid->addWidget(myLine, myRow, 0);
	ui_grid->addWidget(myComboBox, myRow, 1);
	ui_grid->addWidget(myButton, myRow, 2);
	if (next != nullptr) {
		next->shiftUp();
	}

}
void patchObj::freeAll(void) {
	if (next != nullptr) {
		next->freeAll();
	}
	if (prev == nullptr) {
		*headPtr = nullptr;
	}
	else {
		prev->next = nullptr;
	}
	delete this;
}

void patchObj::add(patchObj* baby) {
	if (next == nullptr) {
		next = baby;
		baby->prev = this;
	}
	else {
		next->add(baby);
	}
}

patchObj* patchObj::get(int row) {
	if (row == myRow) {
		return this;
	}
	else {
		if (next != nullptr) {
			return next->get(row);
		}
		else {
			return nullptr;
		}
	}
}
int patchObj::count(void) {
	if (next == nullptr) {
		return 1;
	}
	else
		return next->count() + 1;
}

void patchObj::remove(int row) {
	if (row == myRow) {
		if (prev == nullptr) {
			*headPtr = next;
			if (next != nullptr) {
				next->headPtr = headPtr;
			}
		}
		else{
			prev->next = next;
		}

		if (next != nullptr) {
			next->prev = prev;
			next->shiftUp();		
		}
		delete this;
	}
	else {
		if (next != nullptr) {
			next->remove(row);
		}
	}
}
void patchObj::myButtonClicked(void) {
	emit minusClicked(myRow);
}