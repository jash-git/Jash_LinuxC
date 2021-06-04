#ifndef __HtmlParser_H__
#define __HtmlParser_H__

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

// HtmlParser���A�Ω�ѪRHTML�奻
// http://blog.csdn.net/liigo/article/details/6153829 (���n)
// by liigo, @2010-2012

namespace liigo
{

//MemBuffer: �O����w�İ����A�޲z�w�s�Ϥ��s��s�x�����
#define MEM_DEFAULT_BUFFER_SIZE  256
class MemBuffer
{
public:
	//nBufferSize���w�w�s�Ϫ�l�j�p(�줸�ռ�), ��-1��ܨϥ��q�{��l�j�p(MEM_DEFAULT_BUFFER_SIZE)
	//nBufferSize��0�ɼȤ����t�w�s�ϰO����A�����U�@���g�J��ƮɦA���t
	MemBuffer(size_t nBufferSize = -1);
	MemBuffer(const MemBuffer& other); //�qother����ƻs��ơA�w�s�ϬO�ۦ���t��
	virtual ~MemBuffer(); //�R�c�ɲM�z�w�s������O����A���D�w�gdetach()
	const MemBuffer& operator= (const MemBuffer& other); //�M�Ÿ�ƫ�A��other������ƽƻs�i��

public:
	//�V�w�s�Ͻƻs��ƶ��A�g�J�{����ƪ������A���n�ɷ|�۰��X�R�w�s��
	//��^�s�g�J����ƶ�����}�b�w�s�Ϥ��������q
	size_t appendData(const void* pData, size_t nSize);
	//�b���w����offset�B���J���w����nSize����ơApData��NULL�ɴ��J����Ƨ���0�줸�խȡA�즳��Ʀ۰ʫᲾ�A���n�ɷ|�۰��X�R�w�s��
	void insertData(size_t offset, const void* pData, size_t nSize);
	//�b���w����offset�B�R�����w����size����ơA�즳��Ʀ۰ʫe��
	void deleteData(size_t offset, size_t size);
	//����ƭ���}(�b��ƪ��׬�0�ɪ�^NULL)
	//�bappendXXX()��insertData()��resetSize()��shrink()��exchange()��operator=�եΤ���i��|�ɭP��ƭ���}�o�ͧ���
	inline void* getData() const { return (m_nDataSize == 0 ? NULL : m_pBuffer); }
	//�����w�����B��Ʀ�}�A����offset���p��getDataSize()�A�_�h���O�Ҫ�^����}����
	inline void* getOffsetData(int offset) const { return (m_nDataSize == 0 ? NULL : ((unsigned char*)m_pBuffer + offset)); }
	//���ƾڪ���
	inline size_t getDataSize() const { return m_nDataSize; }
	//���m��ƪ��סA�s���ץi�H�����N�ȡA���n�ɷ|�۰��X�R�w�s�ϡA�s�W�[����Ƨ���0�줸�խ�
	void resetDataSize(size_t size = 0);
	//�M�żƾڡA���ĩ�resetDataSize(0)
	inline void empty() { resetDataSize(0); }
	//���Y�w�s�ϡA�קK���ɶ����Τ��A�ϥΪ��O����A���w�s�Ϥ����w����Ƥ��M����O�d
	void shrink();
	//�M�z�w�s�ϡA����O����
	void clean();
	//���޲z�w�s�ϩM�䤤����ơA�Τ����ۦ�t�d��free()����detach()�᪺���:
	//��^��ƭ���}�A��ƪ��׬�detach()�egetDataSize()��^������
	//detach()�ɫ��w�Ѽ�bShrink��true�i�����קK���O���g�ϥΪ��w�s�ϰO����
	void* detach(bool bShrink = true);
	//�洫��Ӫ���(this & other)�U�ۺ޲z���Ҧ����e�]�]�A��ƩM�w�s�ϡ^
	void exchange(MemBuffer& other);

	//�K�[���������
	inline size_t appendInt(int i) { return appendData(&i, sizeof(i)); }
	inline size_t appendChar(char c) { return appendData(&c, sizeof(c)); }
	//�����p�������ȡ]�ӫDp���V����ơ^�K�[��w�s��
	inline size_t appendPointer(const void* p) { return appendData(&p, sizeof(p)); }
	//��奻���e�K�[��w�s��, len���g�J���줸�ռơ]-1���strlen(szText)�^�AappendZeroChar��ܬO�_�B�~�K�['\0'
	size_t appendText(const char* szText, size_t len = -1, bool appendZeroChar = false);
	//����w�ƶq��0�줸�խȲK�[��w�s��
	size_t appendZeroBytes(int count);

	//Ū���ɥ������e�A�p�GkeepExistData=true�N�O�d�w�s�ϭ즳��ơA�_�h�N�M���즳���
	//�Ѽ�appendZeroChar��ܬO�_�B�~�K�[�r��'\0'�A�Ѽ�pReadBytesr�p�G�DNULL�N�g�J�q�ɤ�Ū�����줸�ռ�
	//�b�Ϥ�Ū�g�X��������Ū���ɤ��e�����p�U�A�N��^false�A���w�gŪ����������Ƥ��M�O�d�ApReadBytesr���|�g�JŪ�����줸�ռ�
	bool loadFromFile(const char* szFileName, bool keepExistData = false, bool appendZeroChar = false, size_t* pReadBytes = NULL);
	//�N��ƫO�s���ɡA���ӰѼ�(pBOM,bomLen)�����g�����Y����BOM(Byte Order Mark)
	//�p�G�ɤw�g�s�b�A�N�����л\���즳�ɤ��e
	bool saveToFile(const char* szFileName, const void* pBOM = NULL, size_t bomLen = 0);

private:
	//�n�D�w�s�Ϥ��ܤ֯d�X���׬�size�����ϥΪŶ�
	//��^���ϥΪŶ�������}�A�Y�{����ƪ�����
	void* require(size_t size);

private:
	unsigned char* m_pBuffer; //�w�s�ϭ��a�}
	size_t m_nDataSize, m_nBufferSize; //�ƾڪ��סA�w�s�Ϫ���
};

enum HtmlNodeType
{
	NODE_NULL = 0,  //�@���Ҧ��`�I���׵��аO
	NODE_START_TAG, //�}�l���ҡA�p <a href="liigo.com"> �� <br/>
	NODE_END_TAG,   //�������ҡA�p </a>
	NODE_CONTENT,   //���e: ����}�l���ҩM/�ε������Ҥ��������q�奻
	NODE_REMARKS,   //�`��: <!-- -->
	NODE_UNKNOWN,   //�������`�I����
	_NODE_USER_,    //�Τ�w�q����L�`�I���������j��_NODE_USER_�A�H�T�O���P�W���w�q���`�q�ȭ���
};

enum HtmlTagType
{
	TAG_UNKNOWN = 0, //��ܥ��g�ѧO�����������A�Ѩ�HtmlParser.onIdentifyHtmlTag()
	TAG_SCRIPT, TAG_STYLE, TAG_TEXTAREA, //�X��ѪR�ݭn�����ѧO<script>,<style>�M<textarea>�A�����S�O�B�z
	//�H�U�����Ҧr�����ǱƦC, �ӷ��Ghttp://www.w3.org/TR/html4/index/elements.html (HTML4)
	//and http://www.w3.org/TR/html5/section-index.html#elements-1 (HTML5)
	//���B�ȩw�q�����ȡA���N��ѪR���@�w�ѧO�X���̡A�Ѩ�HtmlParser.onIdentifyHtmlTag()
	TAG_A, TAG_ABBR, TAG_ACRONYM, TAG_ADDRESS, TAG_APPLET, TAG_AREA, TAG_ARTICLE, TAG_ASIDE, TAG_AUDIO, 
	TAG_B, TAG_BASE, TAG_BASEFONT, TAG_BDI, TAG_BDO, TAG_BIG, TAG_BLOCKQUOTE, TAG_BODY, TAG_BR, TAG_BUTTON, 
	TAG_CAPTION, TAG_CENTER, TAG_CITE, TAG_CODE, TAG_COL, TAG_COLGROUP, TAG_COMMAND, 
	TAG_DATALIST, TAG_DD, TAG_DEL, TAG_DETAILS, TAG_DFN, TAG_DIR, TAG_DIV, TAG_DL, TAG_DT, TAG_EM, TAG_EMBED, 
	TAG_FIELDSET, TAG_FIGCAPTION, TAG_FIGURE, TAG_FONT, TAG_FOOTER, TAG_FORM, TAG_FRAME, TAG_FRAMESET, 
	TAG_H1, TAG_H2, TAG_H3, TAG_H4, TAG_H5, TAG_H6, TAG_HEAD, TAG_HEADER, TAG_HGROUP, TAG_HR, TAG_HTML, 
	TAG_I, TAG_IFRAME, TAG_IMG, TAG_INPUT, TAG_INS, TAG_ISINDEX, TAG_KBD, TAG_KEYGEN, 
	TAG_LABEL, TAG_LEGEND, TAG_LI, TAG_LINK, TAG_MAP, TAG_MARK, TAG_MENU, TAG_META, TAG_METER, TAG_NAV, TAG_NOFRAMES, TAG_NOSCRIPT, 
	TAG_OBJECT, TAG_OL, TAG_OPTGROUP, TAG_OPTION, TAG_P, TAG_PARAM, TAG_PRE, TAG_PROGRESS, TAG_Q, TAG_RP, TAG_RT, TAG_RUBY, 
	TAG_S, TAG_SAMP, TAG_SECTION, TAG_SELECT, TAG_SMALL, TAG_SOURCE, TAG_SPAN, TAG_STRIKE, TAG_STRONG, TAG_SUB, TAG_SUMMARY, TAG_SUP, 
	TAG_TABLE, TAG_TBODY, TAG_TD, TAG_TFOOT, TAG_TH, TAG_THEAD, TAG_TIME, TAG_TITLE, TAG_TR, TAG_TRACK, TAG_TT, 
	TAG_U, TAG_UL, TAG_VAR, TAG_VIDEO, TAG_WBR, 
	_TAG_USER_TAG_, //�Τ�w�q����L�������������j��_TAG_USER_TAG_�A�H�T�O���P�W���w�q���`�q�ȭ���
};

enum HtmlNodeFlag
{
	//flags used in HtmlNode.flags
	FLAG_SELF_CLOSING_TAG = 1 << 0, //�O�۫ʳ�����: <br/>
	FLAG_CDATA_BLOCK      = 1 << 1, //�OCDATA�϶�
	FLAG_NEED_FREE_TEXT   = 1 << 2, //��free(HtmlNode.text)

	//flags used in HtmlAttribute.flags
	FLAG_NEED_FREE_NAME   = 1 << 0, //��free(HtmlAttribute.name)
	FLAG_NEED_FREE_VALUE  = 1 << 1, //��free(HtmlAttribute.value)
};

struct HtmlAttribute
{
	char* name;   //�ݩʦW��
	char* value;  //�ݩʭ�
	size_t flags; //bit OR of HtmlNodeFlag
};

#define MAX_HTML_TAG_LENGTH  15 //�`�I�W�٪��̤j�r������,�W�X�N�Q�I�_

struct HtmlNode
{
	HtmlNodeType type;    //�`�I����
	HtmlTagType  tagType; //���������]�ȷ�`�I������NODE_START_TAG��NODE_END_TAG�ɦ��N�q�^
	char tagName[MAX_HTML_TAG_LENGTH+1]; //���ҦW�١]�p<A href="...">���������ҦW�٬�"A"�^
	char* text; //�奻�A����������(tagType)���P�N�q�]���P�A�i�ରNULL
				//�p�Gtype==NODE_START_TAG, text���V���ҦW�٫᭱���ݩʤ奻
				//�p�Gtype==NODE_CONTENT��NODE_REMARKS, text���V���e�Ϊ`���奻
	int attributeCount;    //�ݩʭӼơ]�ȷ��ݩʳQ�ѪR�ᦳ�ġA�Ѩ�HtmlParser.parseAttributes()�A�U�P�^
	MemBuffer* attributes; //�ݩʡ]�W��=�ȡ^�A�Ѩ�HtmlParser.getAttribute()�C�N�ӥi�H�ʺA�K�[�ݩʡC
	size_t flags;    //bit OR of HtmlNodeFlag
	void* pUser;     //user customized, default to NULL
};


//HtmlParser: HTML�奻�ѪR��
class HtmlParser
{
public:
	HtmlParser() {}
	virtual ~HtmlParser() { cleanHtmlNodes(); }

private:
	//disallow copy and assign: only declare, no implementation
	HtmlParser(const HtmlParser&);
	void operator=(const HtmlParser&);

public:
	//�ѪRHTML�A�ѪR���G�O�@�t�C�s��s�x��HtmlNode�`�I�]�i�q�LgetHtmlNode(index)����^
	//�̫ᥲ�M�|�B�~�K�[�@��NODE_NULL�`�I(HtmlNode.type==NODE_NULL)�@���Ҧ��`�I���׵��аO
	//�Ѽ�parseAttributes��true�ɡA����onParseAttributes()���n�ɱN�Q�եΡA�Τᦳ���|�M�w�O�_�ѪR�ݩ�
	//�Ѽ�parseAttributes��false��ܤ��ѪR�����ݩʡA�ƦܳsonParseAttributes()�]���|�ե�
	void parseHtml(const char* szHtml, bool parseAttributes = true);

	//���`�I�Ӽơ]���]�A�̫�@���B�~�K�[��NODE_NULL�`�I�^
	int getHtmlNodeCount();
	//�����w���޳B���`�I�A���ޥ����X�k: 0 <= index <= getHtmlNodeCount()
	//�䤤�̫�@�Ӹ`�I�]�Yindex==getHtmlNodeCount()�B�^���B�~�K�[��NODE_NULL�`�I
	HtmlNode* getHtmlNode(int index);

	static bool cloneHtmlNode(const HtmlNode* pSrcNode, HtmlNode* pDestNode); //�ݨϥ�cleanHtmlNode()�M�z
	static void cleanHtmlNode(HtmlNode* pNode); //�u�M�z�`�I���ʺA���t�����e�A�O�dtype,tagType,tagName,flags,pUser
	void cleanHtmlNodes(); //�M�z�Ҧ��`�I������Ҧ��ΰO����

	//attributes
	static const HtmlAttribute* getAttribute(const HtmlNode* pNode, size_t index); //must: 0 <= index < pNode->attributeCount
	static const HtmlAttribute* getAttribute(const HtmlNode* pNode, const char* szAttributeName); //return NULL if attribute not exist
	static const char* getAttributeStringValue(const HtmlNode* pNode, const char* szAttributeName, const char* szDefaultValue = NULL);
	static int getAttributeIntValue(const HtmlNode* pNode, const char* szAttributeName, int defaultValue = 0);
	static void parseAttributes(HtmlNode* pNode); //�ѪRpNode->text�ح����ݩ�, �q�`�bonParseAttributes()���ϥ�
	static void parseExtraAttributes(const char* szAttributesText, HtmlNode* pTargetNode, const char* szNamePrefix = NULL); //�ѪR�奻szAttributesText�ت��ݩʨæs�JpTargetNode�]�O�d�즳�ݩʡ^, �Ѽ�szNamePrefix�Ω���w�B�~�K�[���ݩʦW�٭��X�]�H���s�ѪR���ݩʻP�즳�ݩʦW�ٽĬ�^
	//output
	void outputHtml(MemBuffer& buffer, bool keepBufferData = false);
	static void outputHtmlNode(MemBuffer& buffer, const HtmlNode* pNode);
	void dumpHtmlNodes(FILE* f = stdout); //for debug or test
	static void dumpHtmlNode(const HtmlNode* pNode, int nodeIndex = -1, FILE* f = stdout);

protected:
	//���\�l���л\, �H�K�ѧO��h����(�����ѪR�~��), �Ϊ��ѧO��ּ���(�����ѪR�t��)
	//�q�{���ѧO�A��HTML�򥻵��c�M��T�������X�Ӷ}�l����: A,IMG,META,BODY,TITLE,FRAME,IFRAME
	//onIdentifyHtmlTag()����onParseAttributes()�Q�ե�
	virtual HtmlTagType onIdentifyHtmlTag(const char* szTagName, HtmlNodeType nodeType);
	//���\�l���л\, �H�K��n���ѪR�`�I�ݩ�, �Ϊ̳����ѪR�Ʀܰ��ܤ��ѪR�`�I�ݩ�(�����ѪR�t��)
	//�i�H�ھڼ��ҦW��(pNode->tagName)�μ�������(pNode->tagType)�P�_�O�_�ݭn�ѪR�ݩʡ]parseAttributes()�^
	//�q�{�ȸѪR"�w�ѧO�X��������"�������ݩʡ]�YpNode->tagType != TAG_UNKNOWN�^
	virtual void onParseAttributes(HtmlNode* pNode);
	//���\�l���л\, �b�Y�`�I�ѪR������Q�ե�, �p�G��^false�h�ߨ谱��ѪRHTML
	//�o�ؤ]�\�O�@�ӫ���ɾ���l��pNode->pUser
	virtual bool onNodeReady(HtmlNode* pNode) { return true; }

private:
	HtmlNode* appendHtmlNode();

private:
	MemBuffer m_HtmlNodes;
};

} //namespace liigo

#endif //__HtmlParser_H__

