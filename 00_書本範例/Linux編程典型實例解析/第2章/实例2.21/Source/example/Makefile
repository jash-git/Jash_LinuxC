CC=g++
QTINC=/usr/lib/qt-1.44/include
QTLIB=/usr/lib/qt-1.44/lib
MOC=/usr/lib/qt-1.44/bin/moc

example:main.o mainwidget.o mainwidget.moc.o
	${CC} main.o mainwidget.o mainwidget.moc.o -o example -lqt -L${QTLIB}
main.o:main.cpp
	${CC} -c main.cpp -I${QTINC}
mainwidget.o:mainwidget.cpp
	${CC} -c mainwidget.cpp -I${QTINC}
mainwidget.moc.o:mainwidget.moc.cpp
	${CC} -c mainwidget.moc.cpp -I${QTINC}
mainwidget.moc.cpp:mainwidget.h
	${MOC} mainwidget.h -o mainwidget.moc.cpp
