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
