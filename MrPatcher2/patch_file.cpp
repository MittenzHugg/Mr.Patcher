#include "patch_file.h"
#include <QObject>

patch_file::patch_file(QGridLayout * parent, int row) {
	prev = nullptr;
	next = nullptr;

	ui_grid = parent;

	//spawn lineEdit
	myButton = new QToolButton();
	myButton->setText("+");

	//connect(myButton, SIGNAL(clicked()), this, SLOT(myPlusButtonPressed()));

	myComboBox = new QComboBox();
	myComboBox->setEnabled(false);

	myLine = new QLineEdit();
	myLine->setReadOnly(true);

	ui_grid->addWidget(myLine, row, 0);
	ui_grid->addWidget(myComboBox, row, 1);
	ui_grid->addWidget(myButton, row, 2);

}


patch_file::~patch_file() {
	if (prev != nullptr) {
		prev->next = next;
	}
}

void patch_file::myPlusButtonPressed(void) {
	myButton->setText("-");
}