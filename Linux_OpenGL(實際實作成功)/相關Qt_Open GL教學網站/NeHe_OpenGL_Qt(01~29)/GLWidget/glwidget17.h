/*
2Dͼ������
����һ���У��㽫ѧ�����ʹ���ı���������ͼ��������ʾ����Ļ�ϡ�
�㽫ѧ����ΰ�256����ͬ�����ִ�һ��256x256������ͼ���зֱ���ȡ��������Ϊÿһ�����ִ���һ����ʾ�б�
���Ŵ���һ���������������������ϣ�������֡�
*/

#ifndef GLWIDGET17_H
#define GLWIDGET17_H

#include "glwidget.h"

class GLWidget17 : public GLWidget
{
public:
    GLWidget17(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures(); // ����ͼƬ��ת��������
    void buildFont(); // �����ַ���ʾ�б�
    void killFont();
    void glPrint(GLint x, GLint y, const char *string, int set);
    void drawQuad();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
private:
    GLuint base; // �����������ʾ�б�Ŀ�ʼλ��
    GLuint texture[2]; // ������������
    GLfloat cnt1, cnt2; // �����ƶ�������
    bool fullScreen;
    GLfloat xrot, yrot, zrot;
    GLfloat scaling;
    QPoint lastPos;
};

#endif // GLWIDGET17_H
