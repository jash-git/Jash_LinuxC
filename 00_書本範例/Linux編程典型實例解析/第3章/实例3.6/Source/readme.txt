1.��pptp.sql�ļ��ϴ���Linux��������/tmpĿ¼���������
mysql -u root -p < /tmp/pptp.sql
ע��������������ǰ�Ѿ����������е�Ҫ��װ��MySQL��
���������Աຯ��������ʹ����MySQL C����װMySQLʱ���밲װmysql-devel��

2.��ppp-2.4.1.tar.gz��pptpd-1.2.1.tar.gz�ļ��ϴ���Linux��������/usr/localĿ¼�����н�ѹ�����
tar zxvf ppp-2.4.1.tar.gz
tar zxvf pptpd-1.2.1.tar.gz
��װ���
cd ppp-2.4.1
./configure
make
make install
cd pptpd-1.2.1
./configure
make
make install
���޸ĺ��auth.c��main.c��Makefile�ļ�����ppp-2.4.1/pppdĿ¼�µ�ԭ�ļ������±���Ͱ�װ��
cd ppp-2.4.1
make
make install

3.����VPN���������������
/usr/local/sbin/pptpd

