#pragma once

#include <QWidget>
#include "ui_QtBKTextEditor.h"

class QtBKTextEditor : public QWidget
{
	Q_OBJECT

public:
	QtBKTextEditor(QWidget *parent = Q_NULLPTR);
	~QtBKTextEditor();

private:
	Ui::QtBKTextEditor ui;
};
