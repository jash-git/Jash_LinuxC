/***************************************************************************************
  **filename : qnapplicationwindow.h
  **************************************************************************************/
#ifndef QN_APPLICATIONWINDOW_H
#define QN_APPLICATIONWINDOW_H

#include <qmainwindow.h>

#include "qndoc.h"
class QMultiLineEdit;
class QToolBar;

class QnApplicationWindow : public QMainWindow
{
    Q_OBJECT
public:
    QnApplicationWindow( QWidget *parent = 0, const char *name = 0 );
    QnDoc *getDoc() {return p_doc;}
    QWidget *getView() {return center_widget;}
private:
    QToolBar *family_tools;                     //�ļ�������
    QStatusBar *status_bar;                   //״̬��
    
    QWidget * center_widget;                //�������
    QnDoc* p_doc;                                   //�ĵ���
public slots:
    void show_time();
protected:
    void paintEvent( QPaintEvent* );
};

#endif
    
