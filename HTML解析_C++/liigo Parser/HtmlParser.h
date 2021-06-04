#include "common.h"
//HtmlParser���A�Ω�ѪRHTML�奻
//by liigo, @2010
enum HtmlNodeType
{
	NODE_UNKNOWN = 0,
	NODE_START_TAG,
	NODE_CLOSE_TAG,
	NODE_CONTENT,
	NODE_SOFT_LINE,
};
enum HtmlTagType
{
	TAG_UNKNOWN = 0,
	TAG_A, TAG_DIV, TAG_FONT, TAG_IMG, TAG_P, TAG_SPAN, TAG_BR, TAG_B, TAG_I, TAG_HR, 
	TAG_COLOR, TAG_BGCOLOR, //�D�з�HTML����, �i�H�o�˨ϥ�: <color=red>, ���ĩ� <color color=red>
};
struct HtmlNodeProp
{
	WCHAR* szName;
	WCHAR* szValue;
};
#define MAX_HTML_TAG_LENGTH (15)
struct HtmlNode
{
	HtmlNodeType type;
	HtmlTagType  tagType;
	WCHAR tagName[MAX_HTML_TAG_LENGTH+1];
	WCHAR* text;
	int propCount;
	HtmlNodeProp* props;
};
class HtmlParser
{
	friend class HTMLView;
public:
	HtmlParser() {}
public:
	//html
	void ParseHtml(const WCHAR* szHtml);
	const WCHAR* GetHtml() const { return m_html.GetText(); }
	//nodes
	unsigned int getHtmlNodeCount();
	HtmlNode* getHtmlNodes();
	//props
	const HtmlNodeProp* getNodeProp(const HtmlNode* pNode, const WCHAR* szPropName);
	const WCHAR* getNodePropStringValue(const HtmlNode* pNode, const WCHAR* szPropName, const WCHAR* szDefaultValue = NULL);
	int getNodePropIntValue(const HtmlNode* pNode, const WCHAR* szPropName, int defaultValue = 0);
protected:
	//���\�l���л\, �H�K�ѧO��h���I(�����ѪR�~��), �Ϊ��ѧO��ֵ��I(�����ѪR�t��)
	virtual HtmlTagType getHtmlTagTypeFromName(const WCHAR* szTagName);
public:
	//���\�l���л\, �H�K��n���ѪR�`�I�ݩ�, �Ϊ̰��ܤ��ѪR�`�I�ݩ�(�����ѪR�t��)
	virtual void parseNodeProps(HtmlNode* pNode); //todo: make protected, after testing
private:
	HtmlNode* NewHtmlNode();
	void freeHtmlNodes();
	void dumpHtmlNodes();
private:
	CMem m_HtmlNodes;
	CMString m_html;
};
//�@�Ǥ奻�B�z���
WCHAR* duplicateStr(const WCHAR* pSrc, unsigned int nChar);
void freeDuplicatedStr(WCHAR* p);
unsigned int copyStr(WCHAR* pDest, unsigned int nDest, const WCHAR* pSrc, unsigned int nChar);
