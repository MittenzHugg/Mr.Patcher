#include "patchObj.h"
#include <qfiledialog.h>

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