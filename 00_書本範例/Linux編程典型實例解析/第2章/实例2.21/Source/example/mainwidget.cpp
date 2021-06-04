#include "mainwidget.h"
#include "setcolor.xpm"
#include "exit.xpm"
MainWidget::MainWidget(QWidget* parent,const char*name)
	:QMainWindow(parent,name),color(red),point(0,0)
{
	setCaption("Example");
	setBackgroundColor(white);
	QPopupMenu* option=new QPopupMenu;
	option->insertItem("&Set color",this,SLOT(setColor()));
	option->insertSeparator();
	option->insertItem("E&xit",this,SLOT(exitMain()));
	menu=new QMenuBar(this);	
	menu->insertItem("&Option",option);
	//add toolbox
	QToolBar* tools=new QToolBar("example",this);
	QPixmap colorIcon(setcolor_xpm),exitIcon(exit_xpm);
	QToolButton* setcolor=new QToolButton(colorIcon,"Set Color",0,
					this,SLOT(setColor()),
					tools,"set color");
	QToolButton* exitmain=new QToolButton(exitIcon,"Exit",0,
					this,SLOT(exitMain()),
					tools,"exit");
	statusBar()->message("Ready",2000);
}
void
MainWidget::paintEvent(QPaintEvent*)
{
	QPainter p;
	QPen pen(color);
	p.begin(this);
	p.setPen(pen);
	p.drawText(point,"Hello,World!");
	p.end();
}
void
MainWidget::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == LeftButton){
		statusBar()->message("You pressed mouse key",2000);
		point=e->pos();
		repaint();
	}
}
void
MainWidget::setColor()
{
	statusBar()->message("You opened a dialog",2000);
	MainDialog m;
	color=m.exec()?red:blue;
}

void
MainWidget::exitMain()
{
	QApplication::exit();
}

MainDialog::MainDialog(QWidget* parent,const char* name)
	:QDialog(parent,name,TRUE)
{
	setCaption("Choose color");
	QPushButton *r,*b;
	r=new QPushButton("Red",this);
	b=new QPushButton("Blue",this);
	this->setFixedSize(140,80);
	r->setGeometry(20,20,40,30);
	b->setGeometry(80,20,40,30);
	connect(r,SIGNAL(clicked()),SLOT(accept()));
	connect(b,SIGNAL(clicked()),SLOT(reject()));
}	
