#ifndef GLWIDGET10_H
#define GLWIDGET10_H

#include "glwidget.h"

const int textureNum = 6;

struct Vertex
{
    GLfloat x, y, z;
    GLfloat u, v; // ��������
};

struct Triangle
{
    Vertex vertices[3];
};

class GLWidget10 : public GLWidget
{
public:
    GLWidget10(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadTriangles();
    void loadGLTextures();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:    
    GLfloat scaling;
    GLfloat xrot, yrot, zrot;
    QPoint lastPos;
    GLfloat heading;
    GLfloat xpos;
    GLfloat zpos;
    //GLfloat yrot; // ��ת�Ƕ�
    GLfloat walkbias;
    GLfloat walkbiasangle;
    GLfloat lookupdown;
//    GLuint filter; // �˲�����
    GLuint texture[textureNum];
    QList<Triangle> triangles;
    bool blend; // �Ƿ���
    bool lightEnabled; // ��Դ����
    GLfloat lightAmbient[4]; // ������
    GLfloat lightDiffuse[4]; // �����
    GLfloat lightPosition[4]; // ��Դλ��
};

#endif // GLWIDGET10_H
