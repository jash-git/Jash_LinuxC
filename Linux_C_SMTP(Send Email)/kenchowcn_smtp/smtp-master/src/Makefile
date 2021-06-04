all: smtp

smtp: smtp.o base64.o
	gcc smtp.o base64.o -g -o smtp

smtp.o: smtp.c
	gcc -c smtp.c

base64.o: base64.c
	gcc -c base64.c

clean:
	rm -rf *o smtp *.tmp
