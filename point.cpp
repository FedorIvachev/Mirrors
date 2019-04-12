#include "point.h"

Point::Point(float x1, float y1)
{
    x = x1;
    y = y1;
}

Point::Point()
{
    x = 0;
    y = 0;
}

Vertex::Vertex(Point cen, int col = 0, bool is_m = false)
{
    center = cen;
    color = col;
    is_moved = is_m;
}

Vertex::Vertex(){}

Mirror::Mirror(){}

Mirror::Mirror(Vertex l, Vertex r)
{
    left = l;
    right = r;
}
