#include <QtWidgets>
#include <QtOpenGL>
#include <QVector>


class Point
{
public:
    Point(float x1, float y1);
    Point();
    float x;
    float y;
};

class Vertex
{
public:
    Vertex(Point cen, int col, bool is_m);
    Vertex();
    Point center;
    int color = 0; // =1 - blue
    bool is_moved = false;
};

class Ray
{
public:
    QVector <Point> path = {};
};

class Mirror
{
public:
    Vertex left;
    Vertex right;
    Mirror();
    Mirror(Vertex l, Vertex r);
    double r = 0;

};
