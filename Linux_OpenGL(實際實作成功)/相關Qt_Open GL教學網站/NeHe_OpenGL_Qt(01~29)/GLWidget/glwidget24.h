/*
��չ�����ú�TGAͼ���ļ��ļ���:
��ȡ�Կ�֧�ֵ�OpenGL����չ������ָ���ļ������������ʾ������
 */
#ifndef GLWIDGET24_H
#define GLWIDGET24_H

#include "glwidget.h"

struct TextureImage // ��������TGAͼ���ļ��Ľṹ
{
    GLubyte *imageData; // ͼ������ָ��
    GLuint bpp; // ÿ��������ռ��λ��(����Ϊ24��32)
    GLuint width, height; // ͼ���/�߶�
    GLuint texID; // �����IDֵ
};

class GLWidget24 : public GLWidget
{
public:
    GLWidget24(int timeInterval = 0, QWidget *parent = 0);
protected:
    bool loadTGA(TextureImage *texture, const char *fileName);
    void buildFonts();
    void killFonts();
    void glPrint(GLint x, GLint y, int set, const char *fmt, ...);
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void keyPressEvent(QKeyEvent *e);
    void timeOut();
private:
    int scroll; // ����������Ļ
    int maxTokens; // ������չ�ĸ���
    int swidth, sheight; // ���ÿ�/�߶�
    GLuint base; // �ַ���ʾ�б�Ŀ�ʼֵ
    TextureImage textures[1];
};

#endif // GLWIDGET24_H
