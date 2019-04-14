#include "room.h"
#include <iostream>

room::room()
{
}

void room::loadCoords()
{
    QFile settings("/Users/rodiv/MyOpenGL/settings.txt");
    settings.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&settings);
    stream >> n;
    v.resize(n);
    for (int i = 0; i < n; i++) stream >> v[i].center.x >> v[i].center.y;
    ray = Ray();
    stream >> strength >> ray.Start.x >> ray.Start.y
            >> ray.Second.x >> ray.Second.y;
    settings.close();
    area = Area();
    area.Center.x = 0;
    area.Center.y = 0;
    update();
}

void room::saveCoords()
{
    QFile settings("/Users/rodiv/MyOpenGL/settings.txt");
    settings.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QTextStream stream(&settings);
    stream << v.length() << endl;
    for (int i = 0; i < v.length(); i++) stream << v[i].center.x << " "
                                                << v[i].center.y << " " << endl;
    stream << strength << endl;
    stream << ray.Start.x << " " << ray.Start.y << endl;
    stream << ray.Second.x << " " << ray.Second.y << endl;
    settings.close();
}

void room::update()
{

    updateMirrorsCoord();

    ray.path.clear();
    ray.path.append(ray.Start);
    ray.path.append(ray.Second);
    path_built = false;
}

void room::updateMirrorsCoord()
{
    m.clear();
    for (int i = 0; i < n - 1; i++) {
        Mirror mir(v[i], v[i + 1]);
        m.append(mir);
    }
    Mirror mir(v.last(), v[0]);
    m.append(mir);
}

void room::get_path(int strength) {
    if (strength <= 0) {
        return;
    }

    if (has_intersection_area(ray.path.last(), ray.path[ray.path.length()-2], area.Center, area.r)) area.ach = 1;

    QVector <Point> inters = {};
    QVector <int> indexes = {};
    for (int i = 0; i < m.length(); i++) {
        Point inter;
        inter = get_intersection(m[i].left.center, m[i].right.center, ray.path.last(), ray.path[ray.path.length()-2]);

        if ((lies_beetween(m[i].left.center, m[i].right.center, inter) &&
            (lies_beetween(ray.path[ray.path.length()-2], inter, ray.path.last())))) {

            inters.append(inter);
            indexes.append(i);
        }
    }

    Point inter;
    int closest_ind = 0;
    float closest_dist = 100000;
    float current_dist;

    for (int k = 0; k < inters.length(); k++) {
        current_dist = distance(inters[k], ray.path.last());
        if (current_dist < closest_dist) {
            closest_dist = current_dist;
            closest_ind = indexes[k];
            inter = inters[k];
        }
    }
    Point p1(m[closest_ind].left.center.x - inter.x, m[closest_ind].left.center.y - inter.y);
    Point p2(ray.path.last().x - inter.x, ray.path.last().y - inter.y);
    //float angle = scal(p1, p2) / (distance(m[i].left.center, inter) * distance(ray.path.last(), inter));
    Point n(p1.x, p1.y);
    Point zer(0, 0);
    float nl = distance(zer, n);
    Point nn(n.x / nl, n.y / nl);
    float h = 2 * scal(p2, nn);
    Point q(h * nn.x, h * nn.y);
    Point r((p2.x - q.x) / 2000 + inter.x, (p2.y - q.y) / 2000 + inter.y);

    ray.path.append(inter);
    ray.path.append(r);
    strength--;
    path_built = true;
    get_path(strength);
    return;
}

Point room::get_intersection(Point a1, Point a2, Point a3, Point a4)
{
    float u1 = (a1.x * a2.y - a1.y * a2.x) * (a3.x - a4.x) - (a1.x - a2.x) * (a3.x * a4.y - a3.y * a4.x);
    float d = (a1.x - a2.x) * (a3.y - a4.y) - (a1.y - a2.y) * (a3.x - a4.x);
    float u2 = (a1.x * a2.y - a1.y * a2.x) * (a3.y - a4.y) - (a1.y - a2.y) * (a3.x * a4.y - a3.y * a4.x);


    Point ret(u1 / d, u2 / d);
    //Qcout << ret.x << " " << ret.y << endl;
    return ret;
}

bool room::has_intersection_area(Point a1, Point a2, Point center, float r)
{
    a1.x -= center.x;
    a1.y -= center.y;
    a2.x -= center.x;
    a2.y -= center.y;
    float a = a1.y - a2.y;
    float b = a2.x - a1.x;
    float c = a1.x * a2.y - a2.x * a1.y;
    if (c*c > r*r*(a*a+b*b)) return false;
    return true;
    /*
    float x0 = -a*c/(a*a+b*b),  y0 = -b*c/(a*a+b*b);

    float d = r * r - c*c/(a*a+b*b);
    float mult = sqrt (d / (a*a+b*b));
    float ax,ay,bx,by;
    ax = x0 + b * mult;
    bx = x0 - b * mult;
    ay = y0 - a * mult;
    by = y0 + a * mult;
    */
}

float room::distance(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool room::lies_beetween(Point a, Point b, Point c) {
    if (distance(a, c) + distance(b, c) - distance(a, b) <= 0.00001)
    {
        return true;
    } else {
        return false;
    }
}

float room::scal(Point a, Point b)
{
    return a.x * b.x + a.y * b.y;
}
