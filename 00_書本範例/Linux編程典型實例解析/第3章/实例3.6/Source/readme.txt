1.将pptp.sql文件上传到Linux服务器的/tmp目录，运行命令：
mysql -u root -p < /tmp/pptp.sql
注：运行上述命令前已经按照文章中的要求安装好MySQL。
　　由于自编函数代码中使用了MySQL C，安装MySQL时必须安装mysql-devel。

2.将ppp-2.4.1.tar.gz，pptpd-1.2.1.tar.gz文件上传到Linux服务器的/usr/local目录，运行解压缩命令：
tar zxvf ppp-2.4.1.tar.gz
tar zxvf pptpd-1.2.1.tar.gz
安装命令：
cd ppp-2.4.1
./configure
make
make install
cd pptpd-1.2.1
./configure
make
make install
将修改后的auth.c，main.c，Makefile文件复盖ppp-2.4.1/pppd目录下的原文件，重新编译和安装：
cd ppp-2.4.1
make
make install

3.启动VPN服务器，运行命令：
/usr/local/sbin/pptpd

