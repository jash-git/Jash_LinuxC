all:hello slib.so

hello:

       gcc -o hello hello.c -ldl

slib.so:slib.o

       gcc -shared -lc -o slib.so slib.o

slib.o:

       gcc -c -fpic slib.c