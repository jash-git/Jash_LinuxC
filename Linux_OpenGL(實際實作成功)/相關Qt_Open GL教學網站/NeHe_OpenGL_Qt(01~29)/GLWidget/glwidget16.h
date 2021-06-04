/* fog */
#ifndef GLWIDGET16_H
#define GLWIDGET16_H

#include "glwidget.h"

class GLWidget16 : public GLWidget
{
public:
    GLWidget16(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void drawCube();
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
    GLuint texture[3]; // ����Ĵ洢�ռ�
    GLuint fogMode[3]; // ����ģʽ
    GLuint fogFilter;
    GLfloat fogColor[4];
    bool fullScreen;
};

#endif // GLWIDGET16_H
