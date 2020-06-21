/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   QtModelViewWidget.cpp
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

#include "QtModelViewWidget.h"
#include <algorithm>

QtModelViewWidget::QtModelViewWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
}

QtModelViewWidget::~QtModelViewWidget()
{
}

void QtModelViewWidget::setProj(MrP_project* proj)
{
	//load list values
	m_proj = proj;
	auto model_list = m_proj->models();
	ui.openGLWidget->setModel(*model_list.at(0));
	
	
}