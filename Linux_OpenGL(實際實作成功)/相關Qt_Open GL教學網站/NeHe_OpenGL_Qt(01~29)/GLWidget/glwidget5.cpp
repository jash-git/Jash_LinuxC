#include "glwidget5.h"

GLWidget5::GLWidget5(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    rtri = rquad = 0.0f;

    setWindowTitle(tr("3D�ռ�"));
    QIcon icon(":/images/war4.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget5::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget5::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget5::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����Ļ����Ȼ���
    glLoadIdentity(); // ���õ�ǰ��ģ�͹۲����

    glTranslatef(-1.5f,0.0f,-6.0f); // ��Ե�ǰ���ڵ���Ļλ���ƶ�
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glRotatef(rtri, 0.0f, 1.0f, 0.0f); // ��y����ת������

    glBegin(GL_TRIANGLES); // ��ʼ���ƽ������ĸ�����

    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glVertex3f(0.0f, 1.0f, 0.0f); // �����ε��϶��㣨ǰ���棩

    glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ
    glVertex3f(-1.0f, -1.0f, 1.0f); // �����ε����¶��㣨ǰ���棩

    glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
    glVertex3f(1.0f, -1.0f, 1.0f); // �����ε����¶��㣨ǰ���棩

    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glVertex3f(0.0f, 1.0f, 0.0f); // �����ε��϶��㣨�Ҳ��棩

    glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
    glVertex3f(1.0f, -1.0f, 1.0f); // �����ε����¶��㣨�Ҳ��棩

    glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ
    glVertex3f(1.0f, -1.0f, -1.0f); // �����ε����¶��㣨�Ҳ��棩

    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glVertex3f(0.0f, 1.0f, 0.0f); // �����ε��϶��㣨����棩

    glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ
    glVertex3f(1.0f, -1.0f, -1.0f); // �����ε����¶��㣨����棩

    glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
    glVertex3f(-1.0f, -1.0f, -1.0f); // �����ε����¶��㣨����棩

    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glVertex3f(0.0f, 1.0f, 0.0f); // �����ε��϶��㣨����棩

    glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
    glVertex3f(-1.0f, -1.0f, -1.0f); // �����ε����¶��㣨����棩

    glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ
    glVertex3f(-1.0f, -1.0f, 1.0f); // �����ε����¶��㣨����棩

    glEnd();

    glLoadIdentity(); // ����ģ�͹۲����
    glTranslatef(1.5f, 0.0f, -6.0f); // ����1.5��λ����������Ļ6.0��λ
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glRotatef(rquad, 1.0f, 1.0f, 1.0f);

    /* ��ʼ�������壬���������ı�����ɣ����е��ı��ζ�����ʱ��˳�����
       �����Ͻ�->���Ͻ�->���½�->���½�)
     */
    glBegin(GL_QUADS);

    glColor3f(0.5f, 0.5f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f); // �ı��ε����϶��㣨���棩
    glColor3f(0.1f, 0.0f, 0.2f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // �ı��ε����϶��㣨���棩
    glColor3f(0.2f, 0.2f, 0.3f);
    glVertex3f(-1.0f, 1.0f, 1.0f); // �ı��ε����¶��㣨���棩
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex3f(1.0f, 1.0f, 1.0f); // �ı��ε����¶��㣨���棩

    glColor3f(1.0f, 0.5f, 0.0f); // ��ɫ
    glVertex3f(1.0f, -1.0f, 1.0f); // �ı��ε����϶��㣨���棩
    glColor3f(0.2f, 0.4f, 0.4f);
    glVertex3f(-1.0f, -1.0f, 1.0f); // �ı��ε����϶��㣨���棩
    glColor3f(0.3f, 0.4f, 0.1f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // �ı��ε����¶��㣨���棩
    glColor3f(0.5f, 0.2f, 0.4f);
    glVertex3f(1.0f, -1.0f, -1.0f); // �ı��ε����¶��㣨���棩

    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glVertex3f(1.0f, 1.0f, 1.0f); // �ı��ε����϶��㣨ǰ�棩
    glColor3f(0.6f, 0.8f, 0.9f);
    glVertex3f(-1.0f, 1.0f, 1.0f); // �ı��ε����϶��㣨ǰ�棩
    glVertex3f(-1.0f, -1.0f, 1.0f); // �ı��ε����¶��㣨ǰ�棩
    glVertex3f(1.0f, -1.0f, 1.0f); // �ı��ε����¶��㣨ǰ�棩

    glColor3f(1.0f, 1.0f, 0.0f); // ��ɫ
    glVertex3f(1.0f, -1.0f, -1.0f); // �ı��ε����϶��㣨���棩
    glColor3f(0.6f, 0.2f, 0.1f);
    glVertex3f(-1.0f, -1.0f, -1.0f); // �ı��ε����϶��㣨���棩
    glVertex3f(-1.0f, 1.0f, -1.0f); // �ı��ε����¶��㣨���棩
    glVertex3f(1.0f, 1.0f, -1.0f); // �ı��ε����¶��㣨���棩

    glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
    glVertex3f(-1.0f, 1.0f, 1.0f); // �ı��ε����϶��㣨���棩
    glColor3f(0.7f, 0.5f, 0.9f);
    glVertex3f(-1.0f, 1.0f, -1.0f); // �ı��ε����϶��㣨���棩
    glVertex3f(-1.0f, -1.0f, -1.0f); // �ı��ε����¶��㣨���棩
    glVertex3f(-1.0f, -1.0f, 1.0f); // �ı��ε����¶��㣨���棩
/*
    glColor3f(1.0f, 0.0f, 1.0f); // ������ɫ
    glVertex3f(1.0f, 1.0f, -1.0f); // �ı��ε����϶��㣨���棩
    glVertex3f(1.0f, 1.0f, 1.0f); // �ı��ε����϶��㣨���棩
    glVertex3f(1.0f, -1.0f, 1.0f); // �ı��ε����¶��㣨���棩
    glVertex3f(1.0f, -1.0f, -1.0f); // �ı��ε����¶��㣨���棩
*/
    glEnd();
}

void GLWidget5::timeOut()
{
    rtri += 0.5f;
    rquad -= 0.25f;
    updateGL();
}

void GLWidget5::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void GLWidget5::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget5::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx = GLfloat(e->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(e->y() - lastPos.y()) / height();
    if (e->buttons() & Qt::LeftButton) {
        xrot -= 180 * dy;
        yrot -= 180 * dx;
        updateGL();
    } else if (e->buttons() & Qt::RightButton) {
        xrot += 180 * dy;
        zrot += 180 * dx;
        updateGL();
    }
    lastPos = e->pos();
}

void GLWidget5::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
