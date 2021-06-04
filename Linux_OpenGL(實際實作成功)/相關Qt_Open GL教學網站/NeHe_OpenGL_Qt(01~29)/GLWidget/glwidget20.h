/*
�ɰ�:
��ĿǰΪֹ���Ѿ�ѧ�����ʹ��alpha��ϣ���һ��͸��������Ⱦ����Ļ���ˣ�
���е�ʹ������������������ô�ĸ���������⡣ʹ���ɰ弼�������ᰴ�����ɰ��λ�þ�ȷ�Ļ��ơ�
 */

#ifndef GLWIDGET20_H
#define GLWIDGET20_H

#include "glwidget.h"

class GLWidget20 : public GLWidget
{
public:
    GLWidget20(int timeInterval = 0, QWidget *parent = 0);
protected:
    void loadGLTextures();
    void drawQuads1();
    void drawQuads2();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
private:
    bool masking; // �Ƿ�ʹ�á���ģ��
    bool scene; // ������һ������
    GLuint texture[5];
    GLfloat roll; // ��������
};

#endif // GLWIDGET20_H
