/*
 ����ӳ��
 */
#ifndef GLWIDGET23_H
#define GLWIDGET23_H

#include "glwidget.h"

class GLWidget23 : public GLWidget
{
public:
    GLWidget23(int timeInterval = 0, QWidget *parent = 0);
protected:
    void drawCube();
    void loadGLTextures();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLfloat scaling;
    QPoint lastPos;
    GLfloat xrot, yrot, zrot; // ��ת��
    GLfloat xspeed, yspeed; // ��ת�ٶ�
    bool light; // ��Դ����
    GLfloat z; // ������Ļ�ľ���
    GLfloat lightAmbient[4]; // ������
    GLfloat lightDiffuse[4]; // �����
    GLfloat lightPosition[4]; // ��Դλ��
    GLuint filter; // �˲�����
    GLuint texture[6]; // ��������Ĵ洢�ռ�
    int part1, part2; // Բ�̵���ʼ�������Ƕ�
    int p1, p2; // ����
    GLUquadricObj *quadratic; // ���μ�����
    GLuint object; // ���μ������ʶ��
    bool fullScreen;
};

#endif // GLWIDGET23_H
