附件中的example.tar.gz 是程序代码，推荐使用RedHat 6.0以上版本。

1.在Linux下将example.tar.gz 解包：
	tar xvzf example.tar.gz
2.包中已经有个预先编译好的可执行文件example,启动X Window后，执行
	./example
3.如果上面的程序不能正常运行，请检查：
	1）X Window系统安装完整。是否启动了X Window，如果没有，请执行startx
	2）可能某些库没有安装全，请安装Qt 1.44库，一般的Linux版本都带有，请用rpm命令安装
	3）如果系统中有libqt.so文件，还是不能运行，请把该文件所在路径加到环境变量LD_LIBRARY_PATH中,用下面的命令，并用实际的路径更换/usr/lib：
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH;/usr/lib
	4）检查是否安装了stdc++库
4.编译源代码需要对Makefile做一定的修改，请把以下的宏定义该成和你系统相匹配，我的Qt安装在/usr/lib/qt-1.44/ 目录下：
	CC=g++
	QTINC=/usr/lib/qt-1.44/include
	QTLIB=/usr/lib/qt-1.44/lib
	MOC=/usr/lib/qt-1.44/bin/moc
5.确认Makefile中的宏定义正确无误后，输入"make"，即可。编译完成后会生成一个新的执行程序叫example,运行方法同前。
6.该程序在多个Linux版本下测试过.