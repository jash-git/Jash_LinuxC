/*
 �ߡ�����������ʱ����ͶӰ�ͼ򵥵�������
 */

#ifndef GLWIDGET21_H
#define GLWIDGET21_H

#include "glwidget.h"

struct Object // ��¼��Ϸ�еĶ���
{
    int fx, fy; // ʹ�ƶ����ƽ��
    int x, y; // ��ǰ��Ϸ�ߵ�λ��
    float spin; // ��ת����
};

class GLWidget21 : public GLWidget
{
public:
    GLWidget21(int timeInterval = 0, QWidget *parent = 0);
protected:
    void resetObjects();
    void loadGLTextures();
    void buildFonts();
    void killFonts();
    void glPrint(GLint x, GLint y, int set, const char *fmt, ...);
    void movePlayer();
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void timeOut();
    void keyPressEvent(QKeyEvent *e);
private:
    bool vline[11][10]; // ���洹ֱ�����11�������У�ÿ�������е�10���Ƿ��߹�
    bool hline[10][11];
    bool filled; // �����Ƿ�����
    bool gameOver; // ��Ϸ�Ƿ����
    bool antialiasing; // �Ƿ����÷�����
    int delay; // ���˵���ͣʱ��
    int adjust; // ������ʾ���ٶ�
    int lives; // ��ҵ�����
    int level; // �ڲ���Ϸ�ĵȼ�
    int level2; // ��ʾ����Ϸ�ȼ�
    int stage; // ��Ϸ�Ĺؿ�
    Object player; // �����Ϣ
    Object enemy[9]; // ���9�����˵���Ϣ
    Object hourglass; // ɳ©
    int steps[6]; // ����������ʾ���ٶ�
    GLuint texture[2]; // �ַ�����
    GLuint base; // �ַ���ʾ�б�Ŀ�ʼֵ
};

#endif // GLWIDGET21_H
