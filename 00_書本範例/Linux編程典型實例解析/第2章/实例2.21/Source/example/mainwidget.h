#ifndef _MAINWIDGET_H_
#define _MAINWIDTET_H_

#include <qmenubar.h>
#include <qapplication.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qpainter.h>
#include <qpen.h>
#include <qtoolbar.h>
#include <qpixmap.h>
#include <qtoolbutton.h>
#include <qmainwindow.h>
#include <qstatusbar.h>

class MainWidget:public QMainWindow{
	Q_OBJECT
	public:
		MainWidget(QWidget* parent=0,const char* name=0);
	public slots:
		void setColor();
		void exitMain();
	protected:
		void paintEvent(QPaintEvent*);
		void mousePressEvent(QMouseEvent*);
	private:
		QMenuBar* menu;
		QColor color;
		QPoint point;
};

class MainDialog:public QDialog{
	Q_OBJECT
	public:
		MainDialog(QWidget* parentl=0,const char* namel=0);
};

#endif
