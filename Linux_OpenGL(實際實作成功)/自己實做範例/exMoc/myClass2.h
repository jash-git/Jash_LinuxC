#ifndef MyClass2_H
#define MyClass2_H
#include <qobject.h>
#include <stdio.h>
class myclass2:public QObject
{
	Q_OBJECT
	public :
		void displayDigit(int value);
	signals:
		void passValue(int);


};
#endif
