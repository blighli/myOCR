#include "CubeWidget.h"

CubeWidget::CubeWidget( QWidget *parent ) : QGLWidget(parent)
{
	mImage = NULL;
}

void CubeWidget::initializeGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
}

void CubeWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, (GLint)w, (GLint)h);
}

void CubeWidget::paintGL()
{

}

void CubeWidget::setImage( IplImage* image )
{
	mImage = image;
}
