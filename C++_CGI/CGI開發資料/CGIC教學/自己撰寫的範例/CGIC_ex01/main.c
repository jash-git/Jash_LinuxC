#include <stdio.h>
#include "cgic.h"
#include <string.h>
#include <stdlib.h>
int cgiMain()
{
	cgiHeaderContentType("text/html");
	fprintf(cgiOut, "<HTML><HEAD>/n");
	fprintf(cgiOut, "<TITLE>My First CGI</TITLE></HEAD>/n");
	fprintf(cgiOut, "<BODY><H1>Hello CGIC</H1></BODY>/n");
	fprintf(cgiOut, "</HTML>/n");
	return 0;
}
