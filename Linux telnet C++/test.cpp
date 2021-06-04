#include "telnet.h"
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <curses.h>
#include <errno.h>

using namespace std;
/*
void initial(void)
{ 
	initscr(); 
	cbreak(); 
	nonl(); 
	noecho(); 
	intrflush(stdscr,FALSE); 
	keypad(stdscr,TRUE); 
	refresh(); 
	attrset(A_NORMAL);
} 
*/

void sysquit(void)
{
//	endwin();
	exit(0);
}

int main(int argc, char** argv)
{
	int nRet;
	int nPort;
	char strIP[256];
	char strTitle[256];
	int hSocket;
	char sBuff[2048];
	int iLen;
	char* sPtr;


//	initial();
	if(argc <2)
	{
		printf("\nPlease Enter IP/Domain.........:");
		scanf("%s",strIP);
		if ( strlen(strIP) < 3 )
		{
			cout <<"\nBad IP/Domain\n"; 
			sysquit(); 
		}
	}
	else
	{
		strcpy(strIP,argv[1]);
		if ( strlen(strIP) < 3 )
		{
			cout <<"\nBad IP/Domain\n"; 
			sysquit(); 
		}
	}
	nPort=23;
		
	Telnet	hTelnet(strIP,nPort);
	nRet=hTelnet.Connect();
	if ( nRet )
	{ 
		cout <<"\nUnable To Connect Fail Code[" << nRet << "]\n";
		sysquit(); 
	}
	else
	{
		cout << "Connect to " << strIP << " success!" << endl;
	}
	

    std::string reply;
	int iEchoFlag=1;
	char sSendBuff[2048];
	char sRecvBuff[4096];

	while(true)
	{
		strcpy(sRecvBuff,"");
		while(true)
		{
		  hTelnet >> reply;
		  nRet=reply.length();
//		  	cout << "Recv:" << nRet << " !" << endl;
//		  	sleep(1);
			if ( nRet == 0) {cout << flush;break;};
			cout << reply;
		 	strcat(sRecvBuff,reply.data());
		 	if(strstr(sRecvBuff,"logout")) iEchoFlag=0;
		}
		if(!iEchoFlag) break;

		fd_set rfds;
		struct timeval tv;
		int retval;

		/* Watch stdin (fd 0) to see when it has input. */
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		/* Wait up to five seconds. */
		tv.tv_sec = 0;
		tv.tv_usec = 10000;
		retval = select(1, &rfds, NULL, NULL, &tv);
//		cout << "select " << retval << "tv:" << tv.tv_sec <<'.' << tv.tv_usec << endl;
		/* Don’t rely on the value of tv now! */
	    if (retval == -1)
	    {
	       perror("select()");
	    }
	    else if (retval)
		{
			cin.getline(sSendBuff,2047);
			strcat(sSendBuff,"\n");
//			cout << "send:[" << sSendBuff << "]" << endl;
			hTelnet << sSendBuff;
//			if(strncmp(sSendBuff,"exit",4)==0) iEchoFlag =0;
		}
	}
	cout << "System Quit!" << endl;
	sysquit(); 
	return 0;
}
