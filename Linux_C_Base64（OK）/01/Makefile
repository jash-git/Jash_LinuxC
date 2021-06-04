CC = g++

DEL_FILE      = rm -rf

OBJECTS       = base64.o \
				main.o

all:EXE clean

EXE: $(OBJECTS)
	$(CC) -o all $(OBJECTS)
	
base64.o: base64.h
	$(CC) -c base64.c

main.o: base64.h \
		base64.c
	$(CC) -c main.c

clean:
	$(DEL_FILE) $(OBJECTS)
