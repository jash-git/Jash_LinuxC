/********************************************************************************************
  **filename: qncenterwidget.cpp
  ******************************************************************************************/
#ifndef QN_CENTERWIDGET_H
#define QN_CENTERWIDGET_H
#include <qwidget.h>
class QnDoc;

class QnCenterWidget : public QWidget
{
    Q_OBJECT
private:
    QnDoc* get_document();
public:
    QnCenterWidget(QWidget* parent = 0, const char *name = 0);
protected:
    void paintEvent(QPaintEvent *);
public slots:
    void on_sun();
    void on_zhao();
    void on_qian();
    void on_li();
};

#endif
