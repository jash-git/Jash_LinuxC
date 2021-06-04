#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>

int main ( int argc, int argv[] )
{
	try {
		ClientSocket client_socket ( "localhost", 30000 );
		std::string s = "abc";
		
		try {
			client_socket << s;
			client_socket >> s;
		}
		catch ( SocketException& ) {}
		std::cout << "replay from server is: \n\"" << s << "\"\n";;
	}
	catch ( SocketException& e ) {
		std::cout << "Exception was caught:" << e.description() << "\n";	}

	return 0;
}
