#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QGLWidget>
#include <QHash>
#include <opencv2/opencv.hpp>

class CubeWidget : public QGLWidget
{
	Q_OBJECT
public:
	CubeWidget(QWidget *parent = 0);
	~CubeWidget();
	void setImage(IplImage* image);
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	IplImage* mImage;
	QHash<int,int>* mHash;

	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	QPoint lastPos;


};

#endif //CUBEWIDGET_H