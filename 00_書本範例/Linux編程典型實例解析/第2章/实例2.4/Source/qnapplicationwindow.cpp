/*********************************************************
  **filename : qnapplication.cpp
  ********************************************************/
#include "qncenterwidget.h"
#include "qnapplicationwindow.h"
#include "qndoc.h"
#include <qapplication.h>
#include <qmenubar.h>
#include <qpopmenu.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qstatusbar.h>
#include <qwhatsthis.h>
#include <qdatetime.h>
#include <qtimer.h>
#include <qpainter.h>
#include <qdesktopwidget.h>

//the following are xpm file for ICON
#include "qt.xpm"
#include "zhao.xpm"
#include "qian.xpm"
#include "sun.xpm"
#include "li.xpm"
#include "qq.xpm"

QnApplicationWindow::QnApplicationWindow( QWidget* parent, const char *name )
    : QMainWindow(parent, name)
{
    //����ICON����Caption
    QPixmap qt_icon, zhao_icon, qian_icon, sun_icon, li_icon, qq_icon;
    qt_icon = QPixmap( qt );
    zhao_icon = QPixmap( zhao );
    qian_icon = QPixmap (qian );
    sun_icon = QPixmap( sun );
    li_icon = QPixmap( li );
    qq_icon = QPixmap( qq );
    
    setIcon( qt_icon );
    setCaption( "My first Qt programe");
    
    QDesktopWidget *d = QApplication::desktop();
    int w = d->width();     // returns desktop width
    int h = d->height();    // returns desktop height
    int width, height;
    width = 400;
    height = 400;
    setGeometry((w - width)/2, (h - height)/2, width, height);
    //setPalette( QPalette( QColor( 255, 255, 50 ) ) );
    
    center_widget = new QnCenterWidget( this, "center_widget" );
    p_doc = new QnDoc(this, "doc");
    QPopupMenu *family_name = new QPopupMenu( this );
     family_tools = new QToolBar( this, "name_family_toolbar" );
    QToolButton *zhao_button, *qian_button, *sun_button,  *li_button, *qq_button;
    
    //���ò˵���
    family_name->insertItem( zhao_icon , center_widget, SLOT(on_zhao()));
    family_name->insertItem( QString("&Qian"), center_widget, SLOT(on_qian()) );
    family_name->insertItem( QString("&Sun"), center_widget, SLOT(on_sun()) );
    family_name->insertItem( QString("&Li"), center_widget, SLOT(on_li()) );
    
    menuBar()->insertItem("&Family", family_name );
    menuBar()->insertItem("&Name", family_name );
    
    //���ù�����
    zhao_button = new QToolButton( family_tools );
    zhao_button->setIconSet( zhao_icon );
    qian_button = new QToolButton( family_tools );
    qian_button->setIconSet( qian_icon );
    sun_button = new QToolButton( family_tools );
    sun_button->setIconSet( sun_icon );
    li_button = new QToolButton( family_tools );
    li_button->setIconSet( li_icon );
    qq_button = new QToolButton( family_tools );
    qq_button->setIconSet( qq_icon );
    
    //����״̬��
    status_bar = statusBar();
    status_bar->message("please select a task to do...");
    
    //�����������
    
    setCentralWidget( center_widget );
    
    //�����ĵ������
   
    
    QTimer *m_timer = new QTimer(this, "m_timer");    
    m_timer->start( 1000 );
    
    //
    
    connect(m_timer, SIGNAL(timeout()), this, SLOT(show_time()) ) ;
    //����������ť����Ӧ������ϵ
    connect( zhao_button, SIGNAL(clicked()), center_widget, SLOT(on_zhao()) );
    connect( qian_button, SIGNAL(clicked()), center_widget, SLOT(on_qian()) );
    connect( sun_button, SIGNAL(clicked()), center_widget, SLOT(on_sun()) );
    connect( li_button, SIGNAL(clicked()), center_widget, SLOT(on_li()) );
    connect( qq_button, SIGNAL(clicked()), qApp, SLOT(quit()) );

}

void QnApplicationWindow::show_time()
{
    QTime m_time;QString str_time;
    m_time = QTime::currentTime();
    str_time = m_time.toString("hh:mm:ss.zzz");
    status_bar->message( str_time );
}

void QnApplicationWindow::paintEvent( QPaintEvent* e)
{

}
    
