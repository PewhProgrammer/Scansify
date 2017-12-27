#include "OGLWidget.h"
#include <qtimer.h>
#include <qpainter.h>



OGLWidget::OGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	/*
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(1000);
	*/

}

OGLWidget::~OGLWidget()
{
}

void OGLWidget::initializeGL() 
{
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

void OGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	GLubyte* s;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w / h, 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);
}

void OGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5, -0.5, 0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.5, -0.5, 0);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.5, 0);
	glEnd();

	QPainter p(this);
	p.setPen(Qt::red);
	p.drawLine(rect().topLeft(), rect().bottomRight());

}


