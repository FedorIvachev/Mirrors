#ifndef ROOM_H
#define ROOM_H

#include <point.h>
#include <QVector>

class room
{
public:
    room();
    void loadCoords();
    void saveCoords();
    void updateMirrorsCoord();
    void update();
    void get_path(int strength);
    float distance(Point a, Point b);
    bool lies_beetween(Point a, Point b, Point c);
    float scal(Point a, Point b);

    Point get_intersection(Point a1, Point a2, Point b1, Point b2);
    bool has_intersection_area(Point a1, Point a2, Point center, float r);
    int n = 0;
    QVector <Vertex> v = {};
    Ray ray;
    Area area;
    int strength = 0;
    QVector <Mirror> m = {};
    QVector <Vertex> c = {};

    bool path_built = false;
};

#endif // ROOM_H
