#################################################################################################################
#all : test_jash.cpp tinystr.cpp tinystr.h tinyxml.cpp tinyxml.h tinyxmlerror.cpp tinyxmlparser.cpp
#	g++ test_jash.cpp tinystr.cpp tinystr.h tinyxml.cpp tinyxml.h tinyxmlerror.cpp tinyxmlparser.cpp -o all
#################################################################################################################
#main.cpp為主程式進入點，會引入 func.h檔裡的function。
#我們如果使用 gcc command 的方式的，每次更新就要先看func的內容是否更新，或他是否存在，
#來決定要不要建func.o，再同樣的檢查main.o，最後連結來建立主程式。
#故這時make就發揮它的威力了，我們新建一個makefile，然後根據rules來寫建造我們應用程式的內容。

#專有名詞說明:
#target: 工作目標
#prerequisites: 必要條件
#commands: 命令
#timestamp:時間戳記

#一條rule的內容
#target: prereq1 prereq2 
#      commands

#要注意的是commands要擺在跳格字符(tab character)之後。

#makefile的範例(檔案皆在同一資料夾之內):

#1  # A basic makefile for "test"
#2  CC = g++
#3
#4  test: main.o func.o 
#5      $(CC) -o test main.o func.o
#6
#7  main.o:    main.cpp func.h
#8      $(CC) -c main.cpp
#9   
#10 func.o:    func.cpp func.h
#11     $(CC) -c func.cpp

#說明:
#第一行是註解，以#字號為做為註解的標記。
#第二行的 CC是變數，這裡的內容是g++的編譯器。
#以第四行為例，test為target，它的prerequisites為 main.o 和 fun.o，
#第五行為它執行的命令，在這裡 $(CC) 會被替換成 g++，最後執行建造 test程式

CC = g++

DEL_FILE      = rm -rf

OBJECTS       = test_jash.o \
		tinystr.o \
		tinyxml.o \
		tinyxmlerror.o \
		tinyxmlparser.o

all:EXE clean 

EXE: $(OBJECTS)
	$(CC) -o all $(OBJECTS)

test_jash.o: test_jash.cpp tinystr.h \
			tinyxml.h
	$(CC) -c test_jash.cpp

tinystr.o: tinystr.cpp tinystr.h
	$(CC) -c tinystr.cpp

tinyxml.o: tinyxml.cpp tinyxml.h
	$(CC) -c tinyxml.cpp

tinyxmlerror.o: tinyxmlerror.cpp tinyxml.h
	$(CC) -c tinyxmlerror.cpp

tinyxmlparser.o: tinyxmlparser.cpp tinyxml.h
	$(CC) -c tinyxmlparser.cpp

clean:
	$(DEL_FILE) $(OBJECTS)



