// myglwidget.cpp

#include <QtWidgets>
#include <QtOpenGL>
#include <cmath>

#include "myglwidget.h"




MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    r.loadCoords();

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
    for (int i = 0; i < r.v.length(); i++) {
        if ( ((r.v[i].center.x +1) * size().width() * 1.0 / 2 - event->pos().x() <= 40) &&
             ((r.v[i].center.x +1) * size().width() * 1.0 / 2 - event->pos().x() >= -40) &&
             ((1 - r.v[i].center.y) * size().height() * 1.0 / 2 - event->pos().y() <= 40) &&
             ((1 - r.v[i].center.y) * size().height() * 1.0 / 2 - event->pos().y() >= -40))
        {
            r.v[i].color = 1;
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
    for (int i = 0; i < r.v.length(); i++) {
        r.v[i].color = 0;
    }

    //QTextStream Qcout(stdout);
    //Qcout << "fff" << endl;
}
void MyGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    for (int i = 0; i < r.v.length(); i++) {
        if (r.v[i].color == 1) {
            float dx = (event->x() - lastPos.x()) * 1.0 / (size().width() - 60);
            float dy = (event->y() - lastPos.y()) * 1.0 / (size().height() - 60);
            if (event->buttons() & Qt::LeftButton) {
                r.v[i].center.x += dx * 2;
                r.v[i].center.y -= dy * 2;
            }
            //QTextStream Qcout(stdout);
            //Qcout << dx << endl;



        }
    }

    lastPos = event->pos();
    r.update();
    updateGL();


}




void MyGLWidget::updateCurvesCoord()
{
}


void MyGLWidget::setMirrorsNumber(int z)
{
    const double PI=3.14159265358979323846;
    emit mirrorsNumberChanged(z);
    r.n = z;
    r.v.clear();
    for(int i = 0; i < r.n; i++) {
        double angle=i*2*PI/r.n;
        Point a(cos(angle), sin(angle));
        Vertex v1 (a, 0, false);
        r.v.append(v1);
    }
    r.update();
    updateGL();
}

void MyGLWidget::setRayStrength(int z)
{
    emit RayStrengthChanged(z);
    r.strength = z;
    r.ray.path.clear();
    r.ray.path.append(Point(-0.5,0));
    r.ray.path.append(Point(-0.499,0.002));
    r.path_built = false;
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
    if (!r.path_built) r.get_path(r.strength);
    drawRay();
    drawPolygon();
    r.saveCoords();
}

void MyGLWidget::drawPolygon()
{
    int sides=r.n;

    const double PI=3.14159265358979323846;

    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(int i=0;i<sides;i++){
        double angle=i*2*PI/sides;
        glVertex2d(r.v[i].center.x + 1.5, r.v[i].center.y);
    }
    glVertex2d(r.v[0].center.x + 1.5, r.v[0].center.y);
    glEnd();
    for(int i = 0; i < sides; i++) {
        drawCircle(r.v[i].center.x + 1.5, r.v[i].center.y, 0.1, 100);
    }
    /*for(int i = 0; i < sides; i++) {
        float k = -(r.m[i].left.center.x - r.m[i].right.center.x) / (m[i].left.center.y - m[i].right.center.y);
        Point P(0.1 + (m[i].left.center.x + m[i].right.center.x) / 2,
                (0.1 + (m[i].left.center.x + m[i].right.center.x) / 2) * k + (m[i].left.center.y + m[i].right.center.y) / 2 - k * (m[i].left.center.x + m[i].right.center.x) / 2);

        //drawCurve(P.x + 1.5, P.y, 5, 5);
    }*/

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
    for (int i = 0; i < r.ray.path.length(); i++) {
        glVertex2f(r.ray.path[i].x + 1.5, r.ray.path[i].y);
        glColor4f(0.0f, 1.0f, 1.0f, 1 - i * 1.0 / r.ray.path.length());
    }
    glEnd();
}

void MyGLWidget::drawCurve(double cx, double cy, double r, int num_segments)
{

    drawCircle(cx, cy, 0.1, 100);

    //glBegin(GL_LINE_LOOP);
    //glVertex2f(0.5f, 0.5f);
    //for (int i = 0; i < num_segments; i++) {
    //    double angle = 2*3.1415926f*i/num_segments;
    //    glVertex2f (cx+cos(angle)*r, cy+sin(angle)*r);
    //}
    //glEnd();
}
