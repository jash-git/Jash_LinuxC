vpath %.h ./include

CFLAGS = -g -I ./include
CC = gcc
objects = lex.yy.o hy.o hanyu.tab.o speed.tab.o speed_sqlpat.o speed_utility.o func_demo.o

func_demo:$(objects)
	$(CC) $(CFLAGS) -o func_demo $(objects)

lex.yy.o: speed.tab.h
hy.o: hanyu.tab.h
hanyu.tab.o: queryopt.h
speed.tab.o: queryopt.h speed_utility.h speed_sqlpat.h
speed_sqlpat.o: queryopt.h speed_utility.h speed_sqlpat.h
speed_utility.o: queryopt.h speed_utility.h speed_sqlpat.h
func_demo.o: queryopt.h speed_utility.h speed_sqlpat.h

.PHONY:clean
clean:
	rm -f func_demo $(objects)
