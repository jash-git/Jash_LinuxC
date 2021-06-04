#include "HtmlParser.h"

using namespace liigo;

class ParseAll : public HtmlParser
{
	virtual void onParseAttributes(HtmlNode* pNode)
	{
		parseAttributes(pNode);
	}
};

static void testfile(const char* fileName);
static void testoutput(const char* szHtml);
static void parse_and_dump(const char* szHtml);
static void enumnodes();
static void test_custom_attributes();

void main()
{
	HtmlParser htmlParser;
	MemBuffer  mem;

	parse_and_dump("<a url=xx>����<���Ĥl���j>: </a>"); //�������X�k?
	parse_and_dump("<a value=���Z�y��>"); //'�Z'�rGB18030�s�X�v�T�ѪR?
	parse_and_dump("<a alt=3��120��A�H�� src=�u���ʪ���t����>"); //'�稣��'���rGB18030�s�X�v�T�ѪR?	

	testoutput("abc<![CDATA[<<a/>>]]>xyz"); //���T�ѪRCDATA
	testoutput("<!doctype html>"); //���n�ѪR<!DOCTYPE ...>���ݩʡA��˿�X
	testoutput("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
	testoutput("<hr/><p /><img src='...'/>"); //�۫ʳ�����
	testoutput("<a defer url=liigo.com x='' selected>"); //�ݩʨS����
	testoutput("<a url=\"abc\'def\'\" x=\'hello \"liigo\"\'>"); //�ݩʭ����޸��M��޸��O�M

	parse_and_dump("<script rel=\"next\" href=\"objects.html\">");
	parse_and_dump("...<p>---<a href=url>link</a>...");
	parse_and_dump("<p>---< a   href=url >link</a>");
	parse_and_dump("<a x=a y=b z = \"c <a href=url>\" >"); //�ݩʭȤ޸�����<��>���n�v�T�ѪR
	parse_and_dump("<p>\"�޸������ǰt</p>");
	parse_and_dump("<a x=0> <b y=1> <img z=ok w=false> - </img>");
	parse_and_dump("<color=red>");
	parse_and_dump("<p><!--remarks-->...</p>");
	parse_and_dump("<p><!--**<p></p>**--><x/>...</p>");
	parse_and_dump("<style>..<p.><<.every things here, all in style</style>"); //style���i�����N�奻
	parse_and_dump("<script>..<p.><<.every things here, all in script</script>"); //script���i�����N�奻
	parse_and_dump("<textarea>**<b><style><<...</textarea><p>"); //textarea���i�����N�奻
	parse_and_dump("<a href=\"http://www.163.com\">");  //�T�{�ѪR�ᤣ�O�۫ʳ�����
	parse_and_dump("<a href=\"http://www.163.com\"/>"); //�T�{�ѪR��O�۫ʳ�����
	parse_and_dump("<a\tx=1> <a\nx=1\ny=2> <a\r\nx=1\r\ny=2>"); //�D�Ů���j�Ÿ�
	parse_and_dump("<a x=\"abc\"y=''z>"); //�ݩʭȤ޸��᭱�S���ťդ��j�Ÿ��A�ä��A��

	enumnodes(); //�i�ܹM��`�I����k

	test_custom_attributes();

	//���ոѪR�U�j�������
	/*
	testfile("testfiles\\sina.com.cn.html");
	testfile("testfiles\\163.com.html");
	testfile("testfiles\\qq.com.html");
	testfile("testfiles\\sohu.com.html");
	testfile("testfiles\\baidu.com.html");
	testfile("testfiles\\google.com.html");
	testfile("testfiles\\plus.google.com.explore.html");
	testfile("testfiles\\cnbeta.com.html");
	testfile("testfiles\\taobao.com.html");
	*/
}


static void testfile(const char* fileName)
{
	static ParseAll htmlParser;
	static MemBuffer memOutHtml;
	static MemBuffer memHtml;
	if(memHtml.loadFromFile(fileName))
	{
		MemBuffer outFileName;
		outFileName.appendText(fileName);
		outFileName.appendText(".nodes.txt", -1, true);
		FILE* out = fopen((const char*)outFileName.getData(), "wb+");
		if(out == NULL)
			printf("can\'t open output file %s\n", (const char*)outFileName.getData());

		htmlParser.parseHtml((const char*)memHtml.getData(), true); //htmlParser.parseHtml()
		htmlParser.dumpHtmlNodes(out);
		fclose(out);

		memOutHtml.empty();
		htmlParser.outputHtml(memOutHtml); //htmlParser.outputHtml()
		outFileName.empty();
		outFileName.appendText(fileName);
		outFileName.appendText(".html.txt", -1, true);
		memOutHtml.saveToFile((const char*)outFileName.getData());
	}
	else
	{
		printf("can't open input file %s\n", fileName);
	}
}

static void testoutput(const char* szHtml)
{
	HtmlParser htmlParser;
	MemBuffer  mem;

	htmlParser.parseHtml(szHtml, true);
	htmlParser.outputHtml(mem); mem.appendChar('\0');
	printf("%s\n", (char*)mem.getData());
}

static void parse_and_dump(const char* szHtml)
{
	ParseAll parser;
	parser.parseHtml(szHtml, true);
	parser.dumpHtmlNodes();
}

static void enumnodes()
{
	HtmlParser htmlParser;
	htmlParser.parseHtml("<html><p>...</p>");

	//�Ĥ@�عM��`�I����k: for�j��
	for(int index = 0, count = htmlParser.getHtmlNodeCount(); index < count; index++)
	{
		HtmlNode* pNode = htmlParser.getHtmlNode(index);
		htmlParser.dumpHtmlNode(pNode);
	}

	//�ĤG�عM��`�I����k: while�j��
	//�ѩ�̫ᥲ�M���@���B�~�K�[��NODE_NULL�`�I�A�ҥH���k�i��
	HtmlNode* pNode = htmlParser.getHtmlNode(0);
	while(pNode->type != NODE_NULL)
	{
		htmlParser.dumpHtmlNode(pNode);
		pNode++;
	}

	//�Y�Ϧb�S�����`�`�I�����p�U�Awhile�j��M���k�]�O�i�檺
	htmlParser.parseHtml(NULL);
	pNode = htmlParser.getHtmlNode(0);
	while(pNode->type != NODE_NULL)
	{
		htmlParser.dumpHtmlNode(pNode);
		pNode++;
	}

}

static void test_custom_attributes()
{
	HtmlParser htmlParser;
	htmlParser.parseHtml("<a custom='a=1 b=2 c d e=3 f' a=0>", true);
	HtmlNode* pNode = htmlParser.getHtmlNode(0);
	HtmlParser::parseExtraAttributes(HtmlParser::getAttributeStringValue(pNode, "custom"), pNode, "custom:");
	htmlParser.dumpHtmlNode(pNode);
}

