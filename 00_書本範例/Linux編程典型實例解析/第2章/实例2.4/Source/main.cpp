/****************************************************************************************
  **filename : main.cpp
  ***************************************************************************************/
#include "qnapplicationwindow.h"
#include <qapplication.h>

int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    QnApplicationWindow w;
    a.setMainWidget( &w );
    w.show();
    return a.exec();
}
