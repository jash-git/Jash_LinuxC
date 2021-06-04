/* ����������
 */

#ifndef GLWIDGET28_H
#define GLWIDGET28_H

#include "glwidget.h"

struct Point3D
{
    double x, y, z;
};

struct BezierPatch // ��������Ƭ�ṹ
{
    Point3D anchors[4][4]; // ��4x4�������
    GLuint dlBPatch; // ������Ƭ����ʾ�б�����
    GLuint texture; // ��Ƭ������
};

class GLWidget28 : public GLWidget
{
public:
    GLWidget28(int timeInterval = 0, QWidget *parent = 0);

protected:
    Point3D pointAdd(Point3D p, Point3D q); // �����������:p = p + q
    Point3D pointTimes(double c, Point3D p); // �����ͱ������:p = c * p
    Point3D makePoint(double a, double b, double c); // ����һ��3D����
    Point3D Bernstein(float u, Point3D *p); // ���㱴�������̵�ֵ������u�ķ�Χ[0,1]
    GLuint genBezier(BezierPatch patch, int divs); // ���ɱ������������ʾ�б�
    void initBezier();
    void loadGLTexture(GLuint *texPtr, const QString &imgName);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

private:
    float xrot, yrot, zrot;
    int divs; // ϸ�־��ȣ������������ʾ����
    bool showCtrlPoints; // �Ƿ���ʾ���Ƶ�
    BezierPatch myBezier;
    GLfloat scaling;
    QPoint lastPos;
};

#endif // GLWIDGET28_H
