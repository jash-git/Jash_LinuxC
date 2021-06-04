/*
 ����ϵͳ�����ڴ�����ը����Ȫ������֮���Ч����
 */
#ifndef GLWIDGET19_H
#define GLWIDGET19_H

#include "glwidget.h"

#define MAX_PARTICLES 1000 // ���������Ŀ

struct Particle
{
    bool active; // �Ƿ񼤻�
    float life; // ��������
    float fade; // ˥���ٶ�
    float r, g, b; // �졢�̡���
    float x, y, z; // λ��
    float xi, yi, zi; // ����
    float xg, yg, zg; // ���ٶ�
};

class GLWidget19 : public GLWidget
{
public:
    GLWidget19(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    bool rainbow; // �Ƿ�Ϊ�ʺ�ģʽ
    bool sp, rp; // �ո񡢻س����Ƿ񱻰���
    float slowdown; // ��������
    float xspeed, yspeed; // �ٶ�
    float zoom;
    GLuint col; // ��ǰ����ɫ
    GLuint delay; // �ʺ�Ч���ӳ�
    GLfloat scaling;
    QPoint lastPos;
    GLfloat xrot, yrot, zrot; // ��ת��
    Particle particles[MAX_PARTICLES];
    GLuint texture[1];
};

#endif // GLWIDGET19_H
