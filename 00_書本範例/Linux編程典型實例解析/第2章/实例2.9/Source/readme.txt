将两个目录分别拷贝到Linux环境下，就可以编译调试了。
前提是必须安装gcj工具，以及libgcj库。
其中employee目录中，employee是可执行文件，并且带有调试信息，employee.java是源代码文件
编译命令为：gcj -g --main=Employee -o employee employee.java


其中helloworld目录中，helloworld是可执行文件，helloworld.java是源代码文件
编译命令为：gcj --main=helloworld -o helloworld helloworld.java