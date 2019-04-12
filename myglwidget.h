// myglwidget.h

#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <point.h>
#include <QVector>

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

signals:
    void mirrorsNumberChanged(int z);

private:
    void draw();
    void drawPolygon();
    void drawCircle(double cx, double cy, double r, int num_segments);
    void drawRay();
    void drawCurve(double cx, double cy, double r, int num_segments);
    void updateMirrorsCoord();
    void updateCurvesCoord();
    Point rotateCS(Point a, float t);

    void get_path(int strength);
    float distance(Point a, Point b);
    bool lies_beetween(Point a, Point b, Point c);
    float scal(Point a, Point b);

    Point get_intersection(Point a1, Point a2, Point b1, Point b2);
    int n = 0;
    QVector <Vertex> v = {};
    Ray ray;
    QVector <Mirror> m = {};
    QVector <Vertex> c = {};
    bool path_built = false;
    QPoint lastPos;
};

#endif // MYGLWIDGET_H

