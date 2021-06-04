#include "mainwidget.h"

int
main(int argc,char** argv)
{
	QApplication a(argc,argv);
	MainWidget w;
	w.setGeometry(150,150,300,200);
	a.setMainWidget(&w);
	w.show();
	return a.exec();
}
