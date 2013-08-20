#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QGLWidget>

class CubeWidget : public QGLWidget
{
	Q_OBJECT
public:
	CubeWidget(QWidget *parent = 0);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

};

#endif //CUBEWIDGET_H