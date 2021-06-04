#include "glwidget2.h"

GLWidget2::GLWidget2(int timerInterval, QWidget *parent)
    : GLWidget(timerInterval, parent)
{
    setWindowTitle(tr("��һ�������"));
    //showMaximized();
    QIcon icon(":/images/war1.png");
    setWindowIcon(icon);
}

void GLWidget2::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget2::resizeGL(int width, int height)
{
    height = height?height:1;
    glViewport( 0, 0, (GLint)width, (GLint)height );

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget2::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����Ļ����Ȼ���
    glLoadIdentity(); // ���õ�ǰ��ģ�͹۲����

    glTranslatef(-1.5f,0.0f,-5.0f); // ��Ե�ǰ���ڵ���Ļλ���ƶ�
    glBegin(GL_TRIANGLES); // ��ʼ����������
    glVertex3f( 0.0f, 1.0f, 0.0f); // �϶���
    glVertex3f(-1.0f,-1.0f, 0.0f); // ���¶���
    glVertex3f( 1.0f,-1.0f, 0.0f); // ���¶���
    glEnd(); // �����λ��ƽ���

    /* ʹ��˳ʱ�����������(����->����->����->����), ����˳ʱ����Ƶ�
       �Ƕ���ĺ����.
     */
    glTranslatef(3.0f,0.0f,0.0f); // ��x���������ƶ�3����λ
    glBegin(GL_QUADS); // ��ʼ�����ı���
    glVertex3f(-1.0f, 1.0f, 0.0f); // ���϶���
    glVertex3f( 1.0f, 1.0f, 0.0f); // ���϶���
    glVertex3f( 1.0f,-1.0f, 0.0f); // ���¶���
    glVertex3f(-1.0f,-1.0f, 0.0f); // ���¶���
    glEnd(); // �ı��λ��ƽ���
}
