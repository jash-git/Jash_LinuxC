//#include <ctime>
//#include <iostream>
//using namespace std;
#include <time.h>
#include <stdio.h>
int main() {
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	/*
    cout << (now->tm_year + 1900) << '-' 
         << (now->tm_mon + 1) << '-'
         <<  now->tm_mday
		 <<  " " << now->tm_hour
		 <<  ":" <<  now->tm_min
		 <<  ":" <<  now->tm_sec
         << endl;
	*/
	printf("<br/><p>Now Time: %d-%d-%d %d:%d:%d<br/><p>",(now->tm_year + 1900),(now->tm_mon + 1),now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
}
