#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QGLWidget>
#include <opencv2/opencv.hpp>

class CubeWidget : public QGLWidget
{
	Q_OBJECT
public:
	CubeWidget(QWidget *parent = 0);
	void setImage(IplImage* image);
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	IplImage* mImage;

	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	QPoint lastPos;


};

#endif //CUBEWIDGET_H