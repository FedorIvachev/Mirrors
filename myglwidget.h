// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QVector>
#include <room.h>
#include <QMainWindow>


class MyGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit MyGLWidget(QWidget *parent = 0);
    ~MyGLWidget();
signals:

public slots:

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


    QSize sizeHint() const;

public slots:
    void setMirrorsNumber(int z);
    void setRayStrength(int z);

signals:
    void mirrorsNumberChanged(int z);
    void RayStrengthChanged(int z);

private:
    void draw();
    void drawPolygon();
    void drawCircle(double cx, double cy, double r, int num_segments);
    void drawRay();
    void drawCurve(double cx, double cy, double r, int num_segments);
    void updateMirrorsCoord();
    void updateCurvesCoord();
    room r;
    QPoint lastPos;
};

#endif // MYGLWIDGET_H

