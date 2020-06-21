/*************************************************************************
*   Copyright (C) 2020 Michael (Mittenz) Salino-Hugg
*   Contact: Twitter: @MittenzSRL
**************************************************************************
*   GLModelWidget.cpp
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


#include "GLModelWidget.h"
#include <QOpenGLShaderProgram>
#include <qcoreapplication>
#include <limits>

bool GLModelWidget::m_transparent = false;

GLModelWidget::GLModelWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
	if (m_transparent) {
		QSurfaceFormat fmt = format();
		fmt.setAlphaBufferSize(8);
		setFormat(fmt);
	}
	initializeGL();
}

GLModelWidget::~GLModelWidget()
{
	cleanup();
}

void GLModelWidget::setModel(const MrP_Model& model)
{
	m_program->bind();

	m_model = (MrP_Model*)(&model);

	//model.dl_init();
	//std::vector<u16>* tester2 = m_model->getTris();

	m_camera.setToIdentity();
	//m_camera.frustum()
	//m_camera.rotate(180.0, 1.0,0.0);
	//m_camera.translate(0, 0, 0);
	//float inv_scale = model.scale();
	//m_camera.scale(200);

	//vert_head* vH = model.getVertHead();
	//float max = std::numeric_limits<s16>::max();
	////m_camera.perspective(-90.0f, ((float)(vH->lX-vH->sX)) / ((float)(vH->lY - vH->sY)), 0.0001, 1.0);
	//m_camera = m_proj;
	//m_camera.translate(((float)vH->cX) / max, ((float)vH->cY) / max, 3*((float)vH->sZ) / max);
	///*m_camera.frustum(((float)vH->sX)/max, ((float)vH->lX)/max
	//	, ((float)vH->lY) / max, ((float)vH->sY) / max
	//	, ((float)vH->sZ) / max, ((float)vH->lZ) / max
	//);*/
	////m_camera.frustum(-1.0, 1.0
	////	, -1.0f, 1.0f
	////	, -1.0f, 1.0f
	/////);
	
	//m_camera.translate(vH->cX, -vH->cY, 0);
	m_camera.rotate(-45.0, 0.0, 1.0);
	//m_camera.rotate(-30.0, 1.0, 0.0);
	m_program->release();
	


}


void GLModelWidget::cleanup() {
	if (m_program == nullptr)
		return;
	makeCurrent();
	m_logoVbo.destroy();
	delete m_program;
	m_program = nullptr;
	doneCurrent();
}

static const char* vertexShaderSourceCore =
"#version 330 core\n"
"in vec4 vertex;\n"
"uniform mat4 mvMatrix;\n"
"void main() {\n"
"   gl_Position = mvMatrix * vertex;\n"
"}\n";

static const char* fragmentShaderSourceCore =
"#version 330 core\n"
"out highp vec4 fragColor;\n"
"void main() {\n"
"   highp vec3 color = vec3(0.39, 1.0, 0.0);\n"
"   fragColor = vec4(col, 1.0);\n"
"}\n";

static const char* vertexShaderSource =
"attribute vec3 vertex;\n"
"attribute vec2 aTexCoord;\n"
"attribute vec4 aColor;\n"
"varying vec4 ourColor ;\n"
"varying vec2 ourTexCoord;\n"
"uniform mat4 mvMatrix;\n"
"void main() {\n"
"   ourColor = aColor;\n"
"   ourTexCoord = aTexCoord;\n"
"   gl_Position = mvMatrix * vec4(vertex.x, vertex.y, vertex.z, 1.0);\n"
"}\n";

static const char* fragmentShaderSource =
"varying highp vec4 ourColor;\n"
"varying highp vec2 ourTexCoord;\n"
"void main() {\n"
"   gl_FragColor = ourColor;\n"
"}\n";


void GLModelWidget::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLModelWidget::cleanup);

	initializeOpenGLFunctions();
	//glClearColor(0.25, 0.25, 0.25, m_transparent ? 0 : 1);

	m_program = new QOpenGLShaderProgram;
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
	m_program->bindAttributeLocation("vertex", 0);
	//m_program->bindAttributeLocation("normal", 1);
	m_program->link();

	m_program->bind();
	m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
	//m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
	//m_lightPosLoc = m_program->uniformLocation("lightPos");

	
	f3dex_vtx pts[] = {
		{0,0,0,0,0,0,1,0,0,1},
		{5,0,0,0,0,0,0,1,0,1},
		{5,5,0,0,0,0,0,0,1,1},
	};

	float points[] = {
	0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
   -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.5f,  0.0f, 0.0f, 0.0f, 1.0f
	};

	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	
	m_logoVbo.create();
	m_logoVbo.bind();
	//m_logoVbo.allocate(points, 4*6* sizeof(float));
	m_logoVbo.allocate(pts, 3*sizeof(f3dex_vtx));

	m_logoVbo.bind();
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	//f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), NULL);
	f->glVertexAttribPointer(0, 3, GL_SHORT, GL_FALSE, sizeof(f3dex_vtx), NULL);
	//f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	f->glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(f3dex_vtx), (void*)(12));
	m_logoVbo.release();

	m_program->release();
}

void GLModelWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);

	m_world.setToIdentity();
	m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
	m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
	m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

	m_camera = m_proj;
	


	//QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	m_program->bind();
	m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);

	
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_POINTS, 0, m_model->vbo()->size());
	glPointSize(5.0f);//set point size to 10 pixels
	glColor4f(0.5f, 0.5f, 0.0f, 1.0f);

	if (m_model != nullptr) {
		//vert_head* vH = m_model->getVertHead();
		//float max = std::numeric_limits<s16>::max();
		
		//m_camera.translate(((float)vH->cX) / max, ((float)vH->cY) / max, 6 * ((float)vH->sZ) / max);
		//m_camera.rotate(-45.0, 0.0, 1.0);

		m_program->bind();
		m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
		m_model->draw();
	}
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawArrays(GL_POINTS, 0, 3);
	

	m_program->release();

}

void GLModelWidget::resizeGL(int width, int height)
{
	m_proj.setToIdentity();
	m_proj.perspective(-45.0f, GLfloat(width) / GLfloat(height), 0.0001f, 1.0f);
}

void GLModelWidget::mousePressEvent(QMouseEvent* event)
{
}

void GLModelWidget::mouseMoveEvent(QMouseEvent* event)
{
}
