#include "glwidget26.h"

GLWidget26::GLWidget26(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    GLfloat _lightAmbient[4] = {0.7, 0.7, 0.7, 1.0};
    GLfloat _lightDiffuse[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat _lightPosition[4] = {4.0, 4.0, 6.0, 1.0};
    for (int i = 0; i < 4; i++) {
        lightAmbient[i] = _lightAmbient[i];
        lightDiffuse[i] = _lightDiffuse[i];
        lightPosition[i] = _lightPosition[i];
    }
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    xrotspeed = yrotspeed = 0.0;
    zoom = -7.0;
    height = 2.0;

    setWindowTitle(tr("����ƽ�� �ɰ建�� ����"));
    QIcon icon(":/images/INV_Weapon_Crossbow_10.jpg");
    setWindowIcon(icon);
    showMaximized();
}

GLWidget26::~GLWidget26()
{

}

void GLWidget26::loadGLTextures()
{
    QString imageNames[3] = {"Envwall.bmp", "Ball.bmp", "Envroll.bmp"};
    glGenTextures(3, &texture[0]);
    QImage tex, buf;
    for (int i = 0; i < 3; i++) {
        buf.load(":/images/" + imageNames[i]);
        tex = convertToGLFormat(buf);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    }
}

void GLWidget26::initializeGL()
{
    loadGLTextures();
    glShadeModel(GL_SMOOTH);
    glClearColor(0.2, 0.5, 1.0, 1.0);
    glClearDepth(1.0);
    glClearStencil(0); // �����ɰ�ֵ
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_TEXTURE_2D);

    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    q = gluNewQuadric();
    gluQuadricNormals(q, GL_SMOOTH);
    gluQuadricTexture(q, GL_TRUE);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP); // ������������ӳ��
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

void GLWidget26::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // �������
    double eqr[] = {0.0, -1.0, 0.0, 0.0}; // ���ü���ƽ��
    glLoadIdentity();
    glTranslatef(0.0, -1.6, zoom);
    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
    glRotatef(zrot, 0.0, 0.0, 1.0);
    glColorMask(0, 0, 0, 0);
    glEnable(GL_STENCIL_TEST); // �����ɰ建��
    glStencilFunc(GL_ALWAYS, 1, 1); // �����ɰ��������ͨ�����ο�ֵ������ֵ����Ϊ1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // ���õ���Ȳ��Բ�ͨ��ʱ�������ɰ��е�ֵ���䣬���ͨ����ʹ�òο�ֵ�滻�ɰ�ֵ
    glDisable(GL_DEPTH_TEST);
    drawFloor();
    glEnable(GL_DEPTH_TEST);
    glColorMask(1, 1, 1, 1); // ���Ի�����ɫ
    glStencilFunc(GL_EQUAL, 1, 1); // ���������ָ�������ǻ���ʱ�����ı��ɰ建������ֵ
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glEnable(GL_CLIP_PLANE0);
    glClipPlane(GL_CLIP_PLANE0, eqr); // ���ü���ƽ��Ϊ���棬�����õķ���Ϊ����

    glPushMatrix(); { // ���浱ǰ�ľ���
        glScalef(1.0, -1.0, 1.0);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glTranslatef(0.0, height, 0.0);
        glRotatef(xrot, 1.0, 0.0, 0.0);
        glRotatef(yrot, 0.0, 1.0, 0.0);
        drawObject(); // ���Ʒ������
    } glPopMatrix(); // ��������ľ���

    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_STENCIL_TEST);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, 0.8);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawFloor(); // ���Ƶ���
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glTranslatef(0.0, height, 0.0);
//    glScalef(scaling, scaling, scaling);
    glRotatef(xrot, 1.0, 0.0, 0.0);
    glRotatef(yrot, 0.0, 1.0, 0.0);
//    glRotatef(zrot, 0.0, 0.0, 1.0);
    drawObject(); // ������

    xrot += xrotspeed;
    yrot += yrotspeed;
}

void GLWidget26::resizeGL(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width >= height)
        gluPerspective(45.0, (GLdouble)width / (GLdouble)height, 0.1, 100.0);
    else
        gluPerspective(45.0, (GLdouble)height / (GLdouble)width, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget26::drawObject()
{
    // ������
    glColor3f(1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    gluSphere(q, 0.5, 32, 32);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glColor4f(1.0, 1.0, 1.0, 0.4);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);

    gluSphere(q, 0.5, 32, 32);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_BLEND);
}

void GLWidget26::drawFloor()
{
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS); {
        glNormal3f(0.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0.0, 2.0); // ����
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.0, -2.0); // ����
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0.0, -2.0); // ����
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0.0, 2.0); // ����
    } glEnd();
}

void GLWidget26::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F11:
        if (isFullScreen())
            showNormal();
        else
            showFullScreen();
        break;
    case Qt::Key_Right:
        yrotspeed += 0.2;
        updateGL();
        break;
    case Qt::Key_Left:
        yrotspeed -= 0.2;
        updateGL();
        break;
    case Qt::Key_Down:
        xrotspeed += 0.2;
        updateGL();
        break;
    case Qt::Key_Up:
        xrotspeed -= 0.2;
        updateGL();
        break;
    case Qt::Key_A:
        zoom += 0.2;
        updateGL();
        break;
    case Qt::Key_Z:
        zoom -= 0.2;
        updateGL();
        break;
    case Qt::Key_PageUp:
        height += 0.1;
        updateGL();
        break;
    case Qt::Key_PageDown:
        height -= 0.1;
        updateGL();
        break;
    }
}

void GLWidget26::timeOut()
{
    updateGL();
}

void GLWidget26::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget26::mouseMoveEvent(QMouseEvent *e)
{
    GLfloat dx = GLfloat(e->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(e->y() - lastPos.y()) / QWidget::height();
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

void GLWidget26::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
