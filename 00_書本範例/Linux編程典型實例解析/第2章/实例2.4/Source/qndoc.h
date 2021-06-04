/********************************************************************************************
  **filename£ºqndoc.h
  *******************************************************************************************/
#ifndef QN_DOC_H
#define QN_DOC_H
#include <qobject.h>

class QnDoc:public QObject
{
    Q_OBJECT
public:
    QnDoc(QObject* parent, const char *name);
    QString getStr();
    void setStr(QString _str);
private:
    QString str;
};


#endif 
