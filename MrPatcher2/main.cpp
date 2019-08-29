#include "MrPatcher2.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MrPatcher2 w;
	w.show();
	return a.exec();
}
