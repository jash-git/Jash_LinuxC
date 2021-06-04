#include <stdio.h>
#include <time.h>
/*
<script>
<!--
function showtime()
{
today = new Date()
document.CLOCK.ALARM.value = today.toLocaleString()
TimerID = setTimeout ("showtime()",1000)
}
//-->
</script>

<html>
	<head>
		<!--<meta http-equiv="refresh" content="60">-->
		<meta http-equiv="refresh" content="10;url=http://tw.yahoo.com">
	</head>
	<body onLoad="showtime()">
		<form NAME="CLOCK">
			<font COLOR="#000000"><p>²{Šb®Éšè¡G</font><input NAME="ALARM" TYPE="TEXT" SIZE="50"> </p>
		</form>
	</body>
</html>
*/
int main()

{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	//========
	printf("Content-type:text/html\r\n\r\n");
	//========
	printf("<script>\r\n\r\n");
	printf("<!--\r\n\r\n");
	printf("function showtime()\r\n\r\n");
	printf("{\r\n\r\n");
	printf("today = new Date()\r\n\r\n");
	printf("document.CLOCK.ALARM.value = today.toLocaleString()\r\n\r\n");
	printf("TimerID = setTimeout (\"showtime()\",1000)\r\n\r\n");
	printf("}\r\n\r\n");
	printf("//-->\r\n\r\n");
	printf("</script>\r\n\r\n");
	printf("<html>\r\n\r\n");
	printf("	<head>\r\n\r\n");
	printf("		<!--<meta http-equiv=\"refresh\" content=\"60\">-->\r\n\r\n");
	printf("		<meta http-equiv=\"refresh\" content=\"10;url=http://127.0.0.1/cgi-bin/NowTime.cgi\">\r\n\r\n");
	printf("	</head>\r\n\r\n");
	printf("	<body onLoad=\"showtime()\">\r\n\r\n");
	printf("		<form NAME=\"CLOCK\">\r\n\r\n");
	printf("			<font COLOR=\"#000000\"><p>Now Time:</font><input NAME=\"ALARM\" TYPE=\"TEXT\" SIZE=\"50\"> </p>\r\n\r\n");
	printf("		</form>\r\n\r\n");
	printf("		<br/><p>Now Time: %d-%d-%d %d:%d:%d<br/><p>\r\n\r\n",(now->tm_year + 1900),(now->tm_mon + 1),now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
	printf("	</body>\r\n\r\n");
	printf("</html>\r\n\r\n");

	return 0;
}

