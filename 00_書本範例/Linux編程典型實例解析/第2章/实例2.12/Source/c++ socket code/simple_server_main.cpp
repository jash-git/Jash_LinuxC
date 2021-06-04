#include "ServerSocket.h"
#include "SocketException.h"
#include <string>

int main ( int argc, int argv[] )
{
	std::cout << "waiting...\n";

	try {
	      	// Create the socket
		ServerSocket server ( 30000 );
		while ( true ) {
			ServerSocket new_sock;
			
			server.accept ( new_sock );
			try {
				while ( true ) {
					std::string s;
					new_sock >> s;
					s+='a';
					new_sock << s;
				}
			}
			catch ( SocketException& ) {}
		}
	}
	catch ( SocketException& e ) {
		std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
	}

	return 0;
}
