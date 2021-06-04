/********************************************************************************************
  **filename: qncenterwidget.cpp
  *******************************************************************************************/
#include "qncenterwidget.h"
#include "qnapplicationwindow.h"
#include "qndoc.h"
#include <qpainter.h>

QnCenterWidget::QnCenterWidget( QWidget* parent, const char* name )
    : QWidget(parent, name )
{
    setPalette(QPalette(QColor(255,255,255)) );
}

void QnCenterWidget::paintEvent( QPaintEvent *e )
{
    QPainter m_painter;
    QString str;
   
    str = get_document()->getStr();
    m_painter.begin(this);
    m_painter.drawText(0,10, str);
    m_painter.end();
}

QnDoc* QnCenterWidget::get_document()
{
    QnApplicationWindow*  p_parent;
    p_parent = (QnApplicationWindow*) parent();
    QnDoc *m_doc;
    m_doc = (QnDoc*)p_parent->getDoc();
    return m_doc;
}

void QnCenterWidget::on_zhao()
{
    get_document()->setStr("Zhao is the 1st family name!");
    repaint();
}

void QnCenterWidget::on_qian()
{
    get_document()->setStr("Qian is the 2nd family name!");
    repaint();
}
void QnCenterWidget::on_sun()
{
    get_document()->setStr("sun is the 3rd family name!");
    repaint();
}

void QnCenterWidget::on_li()
{
    get_document()->setStr("li is the 4th family name!");
    repaint();
}
