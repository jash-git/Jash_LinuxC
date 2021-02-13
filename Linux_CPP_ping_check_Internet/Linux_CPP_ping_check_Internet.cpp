#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

int main()
{
	if (system("ping -c1 -s1 www.google.com"))
	{
       		cout << "There is no internet connection  \n";
	}
	else
	{
		cout << "There is internet connection  \n";		
	}
	return 0;
}