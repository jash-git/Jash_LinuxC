
文件打包结构：
.
|_ readme
|
|
|
|_ pub_code
	|_ Makefile		//编译脚本
	|_ sql.l			//词法分析文件
	|_ speed.y		//语法分析文件
	|_ ...
	|_ func_demo.c	//演示程序
	|_ ...


演示步骤：
0 将源程序包pub_code.tar拷贝到32位linux操作系统下；
1 解压缩源程序包pub_code.tar（tar xf pub_code.tar）；
2 进入pub_code目录，执行make，编译生成演示程序func_demo；
3 演示程序使用，在步骤2中生成可执行程序后，当前目录执行./func_demo，然后观察屏幕输出；
4 步骤3执行完毕退出后，可以查阅演示日志func.log文件在当前目录，用文本编辑器打开查看；
5 如果想重新编译演示程序，请先执行make clean，然后再执行make；
6 注意：执行make时，可能会打印出一些数据类型警告信息，请忽略，它仍然可以生成可执行程序。
7 当前目录下，usql.txt文件与ssql.txt文件内容是测试所用SQL语句，前者是测试不同形式但效果相同的语句能从形式上归一化；后者则是测试对语句进行语法分析的定制重组。


代码文件介绍：
sql.l查询语句的词法分析文件，speed.y查询语句的语法分析文件，它们是实现SQL语法分析的主要程序；
hy.l词法分析文件，hanyu.y语法分析文件，它们两者结合实现对查询语句中的contains('..')内容进行归一化处理；
lex.yy.c、speed.tab.c、speed.tab.h、hy.c、hanyu.tab.c、hanyu.tab.h均是生成文件；
queryopt.h定义了分析过程中保存分析结果使用的数据结果；
func_demo.c演示程序；


