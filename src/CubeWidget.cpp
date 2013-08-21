#include "CubeWidget.h"
#include "freeglut/freeglut.h"
#include <QtGui>

CubeWidget::CubeWidget( QWidget *parent ) : QGLWidget(parent)
{
	mImage = NULL;

	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;

	resize(600, 600);
}

void CubeWidget::initializeGL()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

}

void CubeWidget::resizeGL( int w, int h )
{
	glViewport(0, 0, (GLint)w, (GLint)h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0, 0.0, 5.0, // eye location
		0.0, 0.0, 0.0, // center location
		0.0, 1.0, 0.0); // up vector
}

void CubeWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(rotationX, 1.0, 0.0, 0.0);
	glRotatef(rotationY, 0.0, 1.0, 0.0);
	glRotatef(rotationZ, 0.0, 0.0, 1.0);

	glLineWidth(5.0);


	glPushMatrix();
	glTranslatef(0, -0.01, 0);
	glBegin(GL_POLYGON);
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glVertex3f(1, 0, 1);
	glVertex3f(1, 0, 0);
	glEnd();
	glPopMatrix();
	
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	if(mImage)
	{
		glPointSize(1.0);
		glBegin(GL_POINTS);
		for(int y=0;y<mImage->height;y++)
		{
			uchar* ptr = (uchar*)(mImage->imageData + y * mImage->widthStep);
			for(int x=0;x<mImage->width;x++)
			{
				int r = ptr[3 * x + 0];
				int g = ptr[3 * x + 1];
				int b = ptr[3 * x + 2];
				glColor3f(r/255.0, g/255.0, b/255.0);
				
				glVertex3f(r/255.0, g/255.0, b/255.0);
			}
		}
		glEnd();
	}


	//glutWireSphere(1.0, 10, 10);

}

void CubeWidget::setImage( IplImage* image )
{
	mImage = image;
}

void CubeWidget::mousePressEvent( QMouseEvent *event )
{
	lastPos = event->pos();
}

void CubeWidget::mouseMoveEvent( QMouseEvent *event )
{
	GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
	GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
	if (event->buttons() & Qt::LeftButton) {
		rotationX += 180 * dy;
		rotationY += 180 * dx;
		updateGL();
	}
	lastPos = event->pos();

}
