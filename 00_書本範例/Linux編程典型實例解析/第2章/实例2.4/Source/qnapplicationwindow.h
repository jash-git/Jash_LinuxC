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
    QToolBar *family_tools;                     //文件工具条
    QStatusBar *status_bar;                   //状态条
    
    QWidget * center_widget;                //中心组件
    QnDoc* p_doc;                                   //文档类
public slots:
    void show_time();
protected:
    void paintEvent( QPaintEvent* );
};

#endif
    
