#include <ctime>
#include "glwidget10.h"

const float piover180 = 0.0174532925f;

GLWidget10::GLWidget10(int timeInterval, QWidget *parent)
    : GLWidget(timeInterval, parent)
{
    scaling = 1.0;
    xrot = yrot = zrot = 0.0;
    blend = false;
    lightEnabled = false;
    heading = 0;
    xpos = 0;
    zpos = 0;
    yrot = 0;
    walkbias = 0;
    walkbiasangle = 0;
    lookupdown = 0;
//    filter = 0;
    setWindowTitle(tr("����3D���� ������������"));
    QIcon icon(":/images/Ability_Gouge.jpg");
    setWindowIcon(icon);
    showMaximized();
}

void GLWidget10::loadTriangles()
{
    QFile file("data/world.txt");
    if (file.open(QFile::ReadOnly)) {
        QTextStream textStream(&file);
        Vertex v[3];
        int vcount = 0;
        bool allok, ok;
        QString str;

        while (!textStream.atEnd()) {
            /* remove whitespace from both ends and replace multiple consecutive
               whitespaces with a single space character within the string */
            str = textStream.readLine().simplified();
            QStringList line = str.split(" ", QString::SkipEmptyParts);

            if (line.count() == 5) {
                allok = true;
                v[vcount].x = line[0].toFloat(&ok); allok &= ok;
                v[vcount].y = line[1].toFloat(&ok); allok &= ok;
                v[vcount].z = line[2].toFloat(&ok); allok &= ok;
                v[vcount].u = line[3].toFloat(&ok); allok &= ok;
                v[vcount].v = line[4].toFloat(&ok); allok &= ok;

                if (allok)
                    vcount++;

                if (vcount == 3) {
                    vcount = 0;
                    Triangle t;
                    t.vertices[0] = v[0];
                    t.vertices[1] = v[1];
                    t.vertices[2] = v[2];
                    triangles.append(t);
                }
            }
        }
        file.close();
    } else {
        QMessageBox::critical(this, "Error", "Open file word.txt failed !", QMessageBox::Ok);
        exit(-1);
    }
}

void GLWidget10::loadGLTextures()
{
    QString images[] = {"Hacker2.png", "apple.jpg", "w2.jpg", "brown.png", "apple1.jpg", "FreeBSD.png"};
//    QDir imageDir("./images");
//    imageDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
//    QFileInfoList fileList = imageDir.entryInfoList();

    glGenTextures(textureNum, &texture[0]);
    for (int i = 0; i < textureNum; i++) {
        QImage t, b(":/images/" + images[i]);
        b.scaled(256, 256);
        t = GLWidget::convertToGLFormat(b);

        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

    }
}

void GLWidget10::initializeGL()
{    
    for (int i = 0; i < 4; i++) {
        lightAmbient[i] = 0.5f;
        lightDiffuse[i] = 1.0f;
    }

    lightAmbient[3] = 1.0f;

    lightPosition[0] = 0.0f;
    lightPosition[1] = 0.0f;
    lightPosition[2] = 2.0f;
    lightPosition[3] = 1.0f;

    loadGLTextures();
    loadTriangles();

    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient); // ���û�����
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse); // ���������
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition); // ���ù�Դλ��
    glEnable(GL_LIGHT1); // ����1�Ź�Դ

    glEnable(GL_TEXTURE_2D); // ��������ӳ��
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // ��ɫ����
    glClearDepth(1.0f); // ������Ȼ���
    glEnable(GL_DEPTH_TEST); // ������Ȳ���
    glDepthFunc(GL_LEQUAL); // ��Ȳ�������
    glShadeModel(GL_SMOOTH); // ������Ӱƽ��
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ͸������
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f); // ȫ���ȣ�50% alpha ���
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // ����ԭ����alphaͨ��ֵ�İ�͸����Ϻ���
}

void GLWidget10::resizeGL(int width, int height)
{
    height = height ? height : 1;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget10::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����Ļ����Ȼ���
    glLoadIdentity(); // ���õ�ǰ��ģ�͹۲����

    GLfloat x_m, y_m, z_m, u_m, v_m;
    GLfloat xtrans = -xpos;
    GLfloat ztrans = -zpos;
    GLfloat ytrans = -walkbias - 0.25f;
    GLfloat sceneroty = 360.0f - yrot; // ����ͷ�������°ڶ�

//    glRotatef(xrot, 1.0, 0.0, 0.0);
//    glRotatef(yrot, 0.0, 0.0, 1.0);
//    glRotatef(zrot, 0.0, 0.0, 1.0);

    glRotatef(lookupdown, 1.0f, 0.0f, 0.0f); // ������ת
    glRotatef(sceneroty, 0.0f, 1.0f, 0.0f); // ������Ϸ���������Է�����������ת

    glTranslatef(xtrans, ytrans, ztrans); // ����Ϸ��Ϊ����ƽ�Ƴ���

    glScalef(scaling, scaling, scaling);
    srand(time(NULL));
    for (QList<Triangle>::const_iterator i = triangles.begin(); i != triangles.end(); ++i) {        
        glBindTexture(GL_TEXTURE_2D, texture[rand() % textureNum]);
        glBegin(GL_TRIANGLES);
        glNormal3f(0.0f, 0.0f, 1.0f); // ָ��ǰ��ķ���

        for (int j = 0; j < 3; j++) {
            x_m = (*i).vertices[j].x;
            y_m = (*i).vertices[j].y;
            z_m = (*i).vertices[j].z;
            u_m = (*i).vertices[j].u;
            v_m = (*i).vertices[j].v;
            glTexCoord2f(u_m, v_m); glVertex3f(x_m, y_m, z_m); // ������������Ͷ���
        }
        glEnd();
    }
}

void GLWidget10::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_L:
        lightEnabled = !lightEnabled;
        if (lightEnabled)
            glEnable(GL_LIGHTING);
        else
            glDisable(GL_LIGHTING);
        updateGL();
        break;
    case Qt::Key_B:
        if (blend) {
            blend = false;
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        } else {
            blend = true;
            glEnable(GL_BLEND);
            glDisable(GL_DEPTH_TEST);
        }
        updateGL();
        break;
//    case Qt::Key_F:
//        filter++;
//        if (filter > 2)
//            filter = 0;
//        updateGL();
//        break;
    case Qt::Key_Up:
        xpos -= (float)sin(heading * piover180) * 0.05f;
        zpos -= (float)cos(heading * piover180) * 0.05f;

        if (walkbiasangle >= 359.0f)
            walkbiasangle = 0.0f;
        else
            walkbiasangle += 10.0f;

        walkbias = (float)sin(walkbiasangle * piover180) / 20.0f; // ʹ��Ϸ�߲�����Ծ��
        updateGL();
        break;
    case Qt::Key_Down:
        xpos += (float)sin(heading * piover180) * 0.05f;
        zpos += (float)cos(heading * piover180) * 0.05f;

        if (walkbiasangle <= 1.0f)
            walkbiasangle = 359.0f;
        else
            walkbiasangle -= 10.0f;

        walkbias = (float)sin(walkbiasangle * piover180) / 20.0f;
        updateGL();
        break;
    case Qt::Key_Left:
        heading += 1.5f;
        yrot = heading;
        updateGL();
        break;
    case Qt::Key_Right:
        heading -= 1.5f;
        yrot = heading;
        updateGL();
        break;
    case Qt::Key_PageUp:
        lookupdown -= 1.0f;
        updateGL();
        break;
    case Qt::Key_PageDown:
        lookupdown += 1.0f;
        updateGL();
        break;
    default:
        GLWidget::keyPressEvent(e);
    }
}

void GLWidget10::mousePressEvent(QMouseEvent *e)
{
    lastPos = e->pos();
}

void GLWidget10::mouseMoveEvent(QMouseEvent *e)
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

void GLWidget10::wheelEvent(QWheelEvent *e)
{
    double numDegrees = -e->delta() / 8.0;
    double numSteps = numDegrees / 15.0;
    scaling *= pow(1.125, numSteps);
    updateGL();
}
