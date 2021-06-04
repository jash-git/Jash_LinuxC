/********************************************************************************************************
  **filename: qndoc.cpp
  *******************************************************************************************************/
#include "qndoc.h"
#include "qobject.h"

QnDoc::QnDoc( QObject* parent, const char *name)
    :QObject( parent, name)
{
    str="hehe, I'm a str number of QnDoc";
}

QString QnDoc::getStr()
{
    return str;
}

void QnDoc::setStr(QString _str)
{
    str = _str;
}
