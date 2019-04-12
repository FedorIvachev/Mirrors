// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <cmath>

#include "myglwidget.h"




MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    ray = Ray();

    setAutoBufferSwap(true);

}

MyGLWidget::~MyGLWidget()
{
}

QSize MyGLWidget::sizeHint() const
{
    return QSize(400, 400);
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
    int eps = 10;
    for (int i = 0; i < v.length(); i++) {
        if ( ((v[i].center.x +1) * size().width() * 1.0 / 2 - event->pos().x() <= 40) &&
             ((v[i].center.x +1) * size().width() * 1.0 / 2 - event->pos().x() >= -40) &&
             ((1 - v[i].center.y) * size().height() * 1.0 / 2 - event->pos().y() <= 40) &&
             ((1 - v[i].center.y) * size().height() * 1.0 / 2 - event->pos().y() >= -40))
        {
            v[i].color = 1;
            lastPos = event->pos();
            break;
        }
    }
    //QTextStream Qcout(stdout);
    //Qcout <<v[1].center.x << " " << size().width() << " " << event->pos().x() << endl;

    //Qcout << v[1].center.y << " " << size().width() << " " << event->pos().y()<< endl;

    updateGL();
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    for (int i = 0; i < v.length(); i++) {
        v[i].color = 0;
    }

    //QTextStream Qcout(stdout);
    //Qcout << "fff" << endl;
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    for (int i = 0; i < v.length(); i++) {
        if (v[i].color == 1) {
            float dx = (event->x() - lastPos.x()) * 1.0 / (size().width() - 60);
            float dy = (event->y() - lastPos.y()) * 1.0 / (size().height() - 60);
            if (event->buttons() & Qt::LeftButton) {
                v[i].center.x += dx * 2;
                v[i].center.y -= dy * 2;
                updateMirrorsCoord();
            }
            //QTextStream Qcout(stdout);
            //Qcout << dx << endl;
            lastPos = event->pos();

            ray.path.clear();
            ray.path.append(Point(-0.5,0));
            ray.path.append(Point(-0.499,0.002));
            path_built = false;

            updateGL();
        }
    }


}

void MyGLWidget::updateMirrorsCoord()
{
    m.clear();
    for (int i = 0; i < n - 1; i++) {
        Mirror mir(v[i], v[i + 1]);
        m.append(mir);
    }
    Mirror mir(v.last(), v[0]);
    m.append(mir);
}

void MyGLWidget::updateCurvesCoord()
{
}

Point MyGLWidget::rotateCS(Point a, float t)
{
    return Point(a.x * cos(t) - a.y * sin(t), a.x * sin(t) + a.y * cos(t));
}
void MyGLWidget::setMirrorsNumber(int z)
{

    const double PI=3.14159265358979323846;
    emit mirrorsNumberChanged(z);
    n = z;
    v.clear();
    for(int i = 0; i < n; i++) {
        double angle=i*2*PI/n;
        Point a(cos(angle), sin(angle));
        Vertex v1 (a, 0, false);
        v.append(v1);
    }
    updateMirrorsCoord();

    ray.path.clear();
    ray.path.append(Point(-0.5,0));
    ray.path.append(Point(-0.499,0.002));
    path_built = false;
    updateGL();
}

void MyGLWidget::initializeGL()
{
}

void MyGLWidget::paintGL()
{
    QSize viewport_size = size();
    glViewport(0, 0, viewport_size.width(), viewport_size.height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 5, 7); // near and far match your triangle Z distance

    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f,0.0f,-6.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_BLEND);

    //glBegin(GL_TRIANGLES);
    //glVertex3f( 0.0f, 1.0f, 0.0f);
    //glVertex3f(-1.0f,-1.0f, 0.0f);
    //glVertex3f( 1.0f,-1.0f, 0.0f);
    //glEnd();
    draw();

}

void MyGLWidget::resizeGL(int width, int height)
{
    QGLWidget::resize(width,height);
}

void MyGLWidget::draw()
{
    if (!path_built) get_path(10);
    drawRay();
    drawPolygon();
}

void MyGLWidget::drawPolygon()
{
    double r=1;
    int sides=n;

    const double PI=3.14159265358979323846;

    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<sides;i++){
        double angle=i*2*PI/sides;
        glVertex2d(v[i].center.x + 1.5, v[i].center.y);
    }
    glVertex2d(v[0].center.x + 1.5, v[0].center.y);
    glEnd();
    for(int i = 0; i < sides; i++) {
        drawCircle(v[i].center.x + 1.5, v[i].center.y, 0.1, 100);
    }
    for(int i = 0; i < sides; i++) {
        float k = -(m[i].left.center.x - m[i].right.center.x) / (m[i].left.center.y - m[i].right.center.y);
        Point P(0.1 + (m[i].left.center.x + m[i].right.center.x) / 2,
                (0.1 + (m[i].left.center.x + m[i].right.center.x) / 2) * k + (m[i].left.center.y + m[i].right.center.y) / 2 - k * (m[i].left.center.x + m[i].right.center.x) / 2);

        drawCurve(P.x + 1.5,
                  P.y,
                  5, 5);
    }

}



void MyGLWidget::drawCircle(double cx, double cy, double r, int num_segments) {
    //glColor3f(0.0f, 1.0f, 1.0f);  // Blue
    //glBegin(GL_LINE_LOOP);
    //for (int ii = 0; ii < num_segments; ii++)   {
    //    float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
    //    float x = r * cosf(theta);//calculate the x component
    //    float y = r * sinf(theta);//calculate the y component
    //    glVertex2f(x + cx, y + cy);//output vertex
    //}
    //glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f, 0.0f, 1.0f);  // Blue
        glVertex2f(cx, cy);       // Center of circle
        GLfloat angle;
        for (int i = 0; i <= num_segments; i++) { // Last vertex same as first vertex
           angle = i * 2.0f * 3.1415926f / num_segments;  // 360 deg for all segments
           glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
           //glVertex2f(lastPos.x() * 1.0 / size().width() + cos(angle) * r,
                      //lastPos.y() * 1.0 / size().height() + sin(angle) * r);
        }
     glEnd();
}

void MyGLWidget::drawRay()
{
    glBegin(GL_LINE_STRIP);

    glColor4f(0.0f, 1.0f, 1.0f, 1);
    for (int i = 0; i < ray.path.length(); i++) {
        glVertex2f(ray.path[i].x + 1.5, ray.path[i].y);
        glColor4f(0.0f, 1.0f, 1.0f, 1 - i * 1.0 / ray.path.length());
    }
    glEnd();
}

void MyGLWidget::drawCurve(double cx, double cy, double r, int num_segments)
{

    Point n(cx, cy);
    Point zer(0,0);
    float nl = distance(zer, n);
    Point nn(n.x / nl, n.y / nl);
    drawCircle(cx, cy, 0.1, 100);

    //glBegin(GL_LINE_LOOP);
    //glVertex2f(0.5f, 0.5f);
    //for (int i = 0; i < num_segments; i++) {
    //    double angle = 2*3.1415926f*i/num_segments;
    //    glVertex2f (cx+cos(angle)*r, cy+sin(angle)*r);
    //}
    //glEnd();
}

void MyGLWidget::get_path(int strength) {
    if (strength <= 0) {
        return;
    }
    for (int i = 0; i < m.length(); i++) {
        Point inter;
        inter = get_intersection(m[i].left.center, m[i].right.center, ray.path.last(), ray.path[ray.path.length()-2]);
        QTextStream Qcout(stdout);
        Qcout << "Intersect " << i << " " << m[i].left.center.x << " " << m[i].left.center.y << " "
                 << m[i].right.center.x << " " << m[i].right.center.y << " "
              << inter.x << " " << inter.y << endl;

        if ((lies_beetween(m[i].left.center, m[i].right.center, inter) &&
            (lies_beetween(ray.path[ray.path.length()-2], inter, ray.path.last())))) {

            Point p1(m[i].left.center.x - inter.x, m[i].left.center.y - inter.y);
            Point p2(ray.path.last().x - inter.x, ray.path.last().y - inter.y);
            //float angle = scal(p1, p2) / (distance(m[i].left.center, inter) * distance(ray.path.last(), inter));
            Point n(p1.x, p1.y);
            Point zer(0, 0);
            float nl = distance(zer, n);
            Point nn(n.x / nl, n.y / nl);
            float h = 2 * scal(p2, nn);
            Point q(h * nn.x, h * nn.y);
            Point r((p2.x - q.x) / 2000 + inter.x, (p2.y - q.y) / 2000 + inter.y);


            QTextStream Qcout(stdout);
            Qcout << "Intersect " << i << " " << inter.x << " " << inter.y << " " << p1.x << " " << p1.y  << endl;

            //ray.path.append(inter);
            //Point zer(0, 0);
            ray.path.append(inter);
            ray.path.append(r);
            strength--;
            path_built = true;
            get_path(strength);
            return;
        }

        //QTextStream Qcout(stdout);
        //Qcout << "Intersect " << i << " " << v[i].center.x << " " << v[i].center.y << " " << inter.x << " " << inter.y<< endl;

    }
    return;
}

Point MyGLWidget::get_intersection(Point a1, Point a2, Point a3, Point a4)
{
    float u1 = (a1.x * a2.y - a1.y * a2.x) * (a3.x - a4.x) - (a1.x - a2.x) * (a3.x * a4.y - a3.y * a4.x);
    float d = (a1.x - a2.x) * (a3.y - a4.y) - (a1.y - a2.y) * (a3.x - a4.x);
    float u2 = (a1.x * a2.y - a1.y * a2.x) * (a3.y - a4.y) - (a1.y - a2.y) * (a3.x * a4.y - a3.y * a4.x);


    Point ret(u1 / d, u2 / d);
    //Qcout << ret.x << " " << ret.y << endl;
    return ret;
}

float MyGLWidget::distance(Point a, Point b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool MyGLWidget::lies_beetween(Point a, Point b, Point c) {
    if (distance(a, c) + distance(b, c) - distance(a, b) <= 0.00001)
    {
        return true;
    } else {
        return false;
    }
}

float MyGLWidget::scal(Point a, Point b)
{
    return a.x * b.x + a.y * b.y;
}

