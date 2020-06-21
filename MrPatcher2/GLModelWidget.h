/*************************************************************************  
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL 
**************************************************************************
*   GLModelWidget.h
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
*/

#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "MrP_Model.h"
#include "bk_model.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLModelWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	GLModelWidget(QWidget *parent = nullptr);
	~GLModelWidget();

	static bool isTransparent() { return m_transparent; };
	static void setTransparent(bool t) { m_transparent = t; }

	//void setModel(MrP_Model& model);
	void setModel(const MrP_Model& model);

public slots:
	void cleanup();
	

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	MrP_Model* m_model = nullptr;

	bool m_core;
	int m_xRot = 0;
	int m_yRot = 0;
	int m_zRot = 0;
	QPoint m_lastPos;

	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_logoVbo;
	QOpenGLBuffer m_elemB;
	u32 tri_cnt;
	QOpenGLShaderProgram* m_program = nullptr;

	int m_projMatrixLoc = 0;
	int m_mvMatrixLoc = 0;
	int m_normalMatrixLoc = 0;
	int m_lightPosLoc = 0;

	QMatrix4x4 m_proj;
	QMatrix4x4 m_camera;
	QMatrix4x4 m_world;

	static bool m_transparent;

};
