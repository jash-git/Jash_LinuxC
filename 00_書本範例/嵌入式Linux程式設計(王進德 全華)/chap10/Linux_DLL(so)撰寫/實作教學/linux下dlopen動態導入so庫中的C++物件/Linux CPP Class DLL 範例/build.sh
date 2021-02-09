g++ -fPIC -shared -o a.so a.cpp
g++ -fPIC -shared -o b.so b.cpp
g++ -fPIC -shared -o c.so c.cpp

g++ -o testApp main.cpp -ldl

