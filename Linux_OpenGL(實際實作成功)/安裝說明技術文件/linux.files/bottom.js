document.write('<br><br>');

//document.writeln('<SCRIPT LANGUAGE="JavaScript1.1" SRC="http://banner.adsrevenue.net/banner.php?myID=pop_william4&sid=4154&ct=3"></SCRIPT>');

function alertAndRedirect()
{
//window.open('http://www.moon-soft.com');
}

function alertAndAddFavorite()
{
//document.links[0].style.behavior='url(#default#homepage)';
//document.links[0].setHomePage('http://www.moon-soft.com/');
//window.external.addFavorite('http://www.moon-soft.com/','月光软件站-源程序代码下载网')
}

var page_url = window.location.href;
var page_referer = document.referrer;
var user_referer_id = '';

if ( page_referer.indexOf("baidu") != -1)	{
	user_referer_id = 'baidu';
}
if ( page_referer.indexOf("google") != -1)	{
	user_referer_id = 'google';
}

if (user_referer_id != '')
{
//setTimeout("alertAndRedirect()",8000);
//window.onbeforeunload=alertAndAddFavorite;
//document.write('<iframe name=ads_pop src="http://p.51ads.com/code/popurl.asp?typeid=14&userid=1090"  marginwidth=0 scrolling=no border=0 frameborder=0 width=0 height=0></iframe>');
}

//防止FRAME加载
//if (top.location !== self.location) {
//top.location=self.location;
//}

