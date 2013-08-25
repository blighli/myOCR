#include "CubeWidget.h"
#include "freeglut/freeglut.h"
#include <QtGui>

CubeWidget::CubeWidget( QWidget *parent ) : QGLWidget(parent)
{
	mImage = NULL;
	mHash = new QHash<int, int>();

	rotationX = 0;
	rotationY = 0;
	rotationZ = 0;

	resize(600, 600);
}

CubeWidget::~CubeWidget()
{
	delete mHash;
}

void CubeWidget::initializeGL()
{
	char *argv[] = {"", NULL};
	int argc = 1;
	glutInit( &argc, argv );

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

	glLineWidth(1.0);

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

	glPushMatrix();
	glTranslatef(0.51, 0.51, 0.51);
	glColor3f(1.0, 1.0, 1.0);
	glutWireCube(1.0);
	glPopMatrix();

	int r = 0;
	int g = 0;
	int b = 0;
	QHash<int, int>::const_iterator i = mHash->constBegin();
	
	int pointCount = 1;
	int pixelCount = 1;
	if(mImage)
	{
		pointCount = mHash->size();
		pixelCount = mImage->width * mImage->height;
	}

	while (i != mHash->constEnd()) {
		
		int key = i.key();
		int value = i.value();


		r = key / (256*256);
		g = (key - r*256*256)/256;
		b = key - r*256*256 - g*256;

		glPushMatrix();
		
		glColor3f(r/255.0, g/255.0, b/255.0);
		glTranslatef(r/255.0, g/255.0, b/255.0);

		float radius = sqrtf((float)value / (float)pixelCount);
		//if(radius > 0.01)
		//{
			glutSolidSphere( radius, 5, 5);
		//}
		//else
		//{
		//	glutSolidCube(radius);
		//}
		glPopMatrix();

		++i;
	}


}

void CubeWidget::setImage( IplImage* image )
{
	mImage = image;

	if(mImage)
	{
		int r = 0;
		int g = 0;
		int b = 0;

		mHash->clear();
		for(int y=0;y<mImage->height;y++)
		{
			uchar* ptr = (uchar*)(mImage->imageData + y * mImage->widthStep);
			for(int x=0;x<mImage->width;x++)
			{
				if(mImage->nChannels == 1)
				{
					b = ptr[x];
					g = ptr[x];
					r = ptr[x];
				}
				else if(mImage->nChannels == 3)
				{
					b = ptr[3 * x + 0];
					g = ptr[3 * x + 1];
					r = ptr[3 * x + 2];
				}
				int  key = r*256*256 + g*256 + b;
				int value = mHash->value(key, 0) + 1;
				mHash->insert(key, value);
			}
		}
	}

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
