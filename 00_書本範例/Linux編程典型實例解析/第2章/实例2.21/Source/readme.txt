�����е�example.tar.gz �ǳ�����룬�Ƽ�ʹ��RedHat 6.0���ϰ汾��

1.��Linux�½�example.tar.gz �����
	tar xvzf example.tar.gz
2.�����Ѿ��и�Ԥ�ȱ���õĿ�ִ���ļ�example,����X Window��ִ��
	./example
3.�������ĳ������������У����飺
	1��X Windowϵͳ��װ�������Ƿ�������X Window�����û�У���ִ��startx
	2������ĳЩ��û�а�װȫ���밲װQt 1.44�⣬һ���Linux�汾�����У�����rpm���װ
	3�����ϵͳ����libqt.so�ļ������ǲ������У���Ѹ��ļ�����·���ӵ���������LD_LIBRARY_PATH��,��������������ʵ�ʵ�·������/usr/lib��
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH;/usr/lib
	4������Ƿ�װ��stdc++��
4.����Դ������Ҫ��Makefile��һ�����޸ģ�������µĺ궨��óɺ���ϵͳ��ƥ�䣬�ҵ�Qt��װ��/usr/lib/qt-1.44/ Ŀ¼�£�
	CC=g++
	QTINC=/usr/lib/qt-1.44/include
	QTLIB=/usr/lib/qt-1.44/lib
	MOC=/usr/lib/qt-1.44/bin/moc
5.ȷ��Makefile�еĺ궨����ȷ���������"make"�����ɡ�������ɺ������һ���µ�ִ�г����example,���з���ͬǰ��
6.�ó����ڶ��Linux�汾�²��Թ�.