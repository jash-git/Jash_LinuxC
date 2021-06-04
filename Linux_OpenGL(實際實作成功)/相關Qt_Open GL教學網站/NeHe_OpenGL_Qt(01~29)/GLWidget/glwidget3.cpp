#include "glwidget3.h"

GLWidget3::GLWidget3(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;

    setWindowTitle(tr("�����ɫ"));
    QIcon icon(":/images/war2.png");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget3::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget3::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget3::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����Ļ����Ȼ���
    glLoadIdentity(); // ���õ�ǰ��ģ�͹۲����

    // �����β���ƽ����ɫ
    glTranslatef(-1.5f,0.0f,-6.0f); // ��Ե�ǰ���ڵ���Ļλ���ƶ�
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glBegin(GL_TRIANGLES); // ��ʼ����������
    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glVertex3f( 0.0f, 1.0f, 0.0f); // �϶���
    glColor3f(0.0f, 1.0f, 0.0f); // ��ɫ
    glVertex3f(-1.0f,-1.0f, 0.0f); // ���¶���
    glColor3f(0.0f, 0.0f, 1.0f); // ��ɫ
    glVertex3f( 1.0f,-1.0f, 0.0f); // ���¶���
    glEnd(); // �����λ��ƽ���

    /* ʹ��˳ʱ�����������(����->����->����->����), ����˳ʱ����Ƶ�
       �Ƕ���ĺ����.
     */
    // �ı��β���ƽ����ɫ
    glTranslatef(3.0f,0.0f,0.0f); // ��x���������ƶ�3����λ
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);

    glColor3f(0.5f, 0.5f, 1.0f);
    glBegin(GL_QUADS); // ��ʼ�����ı���
    glVertex3f(-1.0f, 1.0f, 0.0f); // ���϶���
    glVertex3f( 1.0f, 1.0f, 0.0f); // ���϶���
    glVertex3f( 1.0f,-1.0f, 0.0f); // ���¶���
    glVertex3f(-1.0f,-1.0f, 0.0f); // ���¶���
    glEnd(); // �ı��λ��ƽ���
}

void GLWidget3::keyPressEvent(QKeyEvent *e)
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

void GLWidget3::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget3::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget3::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
