#include <stdio.h>
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
	printf("<script>\n");
	printf("<!--\n");
	printf("function showtime()\n");
	printf("{\n");
	printf("today = new Date()\n");
	printf("document.CLOCK.ALARM.value = today.toLocaleString()\n");
	printf("TimerID = setTimeout (\"showtime()\",1000)\n");
	printf("}\n");
	printf("//-->\n");
	printf("</script>\n");
	printf("<html>\n");
	printf("	<head>\n");
	printf("		<!--<meta http-equiv=\"refresh\" content=\"60\">-->\n");
	printf("		<meta http-equiv=\"refresh\" content=\"10;url=http://tw.yahoo.com\">\n");
	printf("	</head>\n");
	printf("	<body onLoad=\"showtime()\">\n");
	printf("		<form NAME=\"CLOCK\">\n");
	printf("			<font COLOR=\"#000000\"><p>Now Time:</font><input NAME=\"ALARM\" TYPE=\"TEXT\" SIZE=\"50\"> </p>\n");
	printf("		</form>\n");
	printf("	</body>\n");
	printf("</html>\n");

	return 0;
}

