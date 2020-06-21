/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   QtModelViewWidget.h
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

#include <QWidget>
#include "ui_QtModelViewWidget.h"
#include "MrP_project.h"
#include <vector>

class QtModelViewWidget : public QWidget
{
	Q_OBJECT

public:
	QtModelViewWidget(QWidget *parent = Q_NULLPTR);
	~QtModelViewWidget();

	//void setModel(MrP_Model& model);
	void setProj(MrP_project* model);

public slots:
	

private:
	Ui::QtModelViewWidget ui;
	MrP_project* m_proj = nullptr;
	std::vector<MrP_Model*> model_list;
};
