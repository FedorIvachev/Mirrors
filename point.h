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
    Point Start;
    Point Second;
    QVector <Point> path = {};
    int color = 0;
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

class Area
{
public:
    Point Center;
    int color = 0;
    int ach = 0;
    float r = 0.2;
};
