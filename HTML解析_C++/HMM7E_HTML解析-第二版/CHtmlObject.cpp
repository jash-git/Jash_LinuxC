//////////////////////////////////////////////////////////////////////////////////////////CHtmlObject.cpp//////////////////////////////////////////////////////////////////////////

#pragma once

/*****************************************************************************************************************
created: 2011/12/03
author: hmm7e (hmm7e_z@126.com)

*****************************************************************************************************************/


#include "HtmlObject.h"

//
BOOL CHtmlObject::IsSpace(TCHAR tcLetter)
{
	//�H�U�r���bHTML�аO�س���O�Ů�C
	return (tcLetter == _T(' ') || tcLetter == _T('\r') ||tcLetter == _T('\n') ||tcLetter == _T('\t') );
}

CHtmlObject::CHtmlObject(void)
{
	m_pszSnapshotBuffer = NULL;
	m_nSnapshotBufferLen = 0;
	m_nSnapshotStringLen = 0;
	m_pstHead = NULL;
	m_pstTail = NULL;
}

CHtmlObject::~CHtmlObject(void)
{
	DeleteSnapshot();
}
//
CHtmlObject::tagNode * CHtmlObject::InnerAllocNode()
{
	CHtmlObject::tagNode * pstResult = new CHtmlObject::tagNode;
	if( pstResult )
	{
		::ZeroMemory((LPVOID)pstResult,sizeof(CHtmlObject::tagNode));
	}
	return pstResult;
}
void CHtmlObject::InnerFreeNode(CHtmlObject::tagNode * lpstNode)
{
	if( lpstNode )
		delete lpstNode;
}
void CHtmlObject::InnerLinkNextNode(tagNode * lpstNode)
{
	//�쵲�졧�������I�C
	//1�G�p�G�S�����Y���`�I�A���������O���š����C
	//2�G�p�G�w�g�s���Y���`�I�A����N�쵲�s�`�I�졧�����`�I�A�í��s�O���������`�I���СC
	if( m_pstHead == NULL )
	{
		m_pstHead = lpstNode;
		m_pstTail = lpstNode;
	}
	else
	{
		m_pstTail->s_pstNext = lpstNode;
		m_pstTail = lpstNode;
	}


#ifdef _DEBUG

	if( lpstNode->s_pszKey )
	{
		::OutputDebugString(_T("--"));
		::OutputDebugString(lpstNode->s_pszKey);
		::OutputDebugString(_T("--\r\n"));
	}
	if( lpstNode->s_pszValue )
	{
		::OutputDebugString(_T("--"));
		::OutputDebugString(lpstNode->s_pszValue);
		::OutputDebugString(_T("--\r\n"));
	}

#endif //_DEBUG

}
void CHtmlObject::InnerLinkRightNode(tagNode * lpstTagNode,tagNode * lpstNode)
{
	//�쵲�졧�ݩʡ������Y���`�I�C
	//1�G��{�������ݩʡ����A�쵲���e�s�`�I���U�C
	//2�G���e�`�I�������Y���`�I�O�s�C
	lpstNode->s_pstRight = lpstTagNode->s_pstRight;
	lpstTagNode->s_pstRight = lpstNode;

#ifdef _DEBUG
	if( lpstNode->s_pszKey )
	{
		::OutputDebugString(_T("-->"));
		::OutputDebugString(lpstNode->s_pszKey);
		::OutputDebugString(_T("<--\r\n"));
	}
	if( lpstNode->s_pszValue )
	{
		::OutputDebugString(_T("-->"));
		::OutputDebugString(lpstNode->s_pszValue);
		::OutputDebugString(_T("<--\r\n"));
	}
#endif //_DEBUG
}
void CHtmlObject::InnerCleanupNode()
{
	//�j��M���Ҧ��`�I�C�p�G�s�b���ݩʡ��`�I�@�ֲM���C
	CHtmlObject::tagNode * pstPrev = NULL;
	while( m_pstHead )
	{
		pstPrev = m_pstHead;
		m_pstHead = m_pstHead->s_pstNext;
		//first
		InnerCleanupRightNode(pstPrev);
		//second
		InnerFreeNode(pstPrev);
	}
	m_pstHead = NULL;
	m_pstTail = NULL;
}
void CHtmlObject::InnerCleanupRightNode(CHtmlObject::tagNode * lpstNode)
{
	//�j��M���Ҧ����ݩʡ��`�I�C
	CHtmlObject::tagNode * pstHead = lpstNode->s_pstRight;
	CHtmlObject::tagNode * pstPrev = NULL;
	while( pstHead )
	{
		pstPrev = pstHead;
		pstHead = pstHead->s_pstRight;
		InnerFreeNode(pstPrev);
	}
	pstHead = NULL;
	pstPrev = NULL;
}
//
void CHtmlObject::AutoTakeSnapshot(PBYTE lpszString,UINT nStringLen)
{
	
	if( lpszString && nStringLen > 0)
	{
		//�ھڸ���Y�۰ʧP�_�O�_�ݭn�ഫ��ƨ��e���{�ҨϥΪ��s�X�C
		if( nStringLen >= 2 )
		{
			if( lpszString[0] == 0xFF && lpszString[1] == 0xFE ) // skip 0xFF,0xFE
			{
				TakeSnapshot(lpszString+2,nStringLen-2,CHtmlObject::CHARSET_UNICODE);
			}
			else if( lpszString[0] == 0xEF && lpszString[1] == 0xBB && lpszString[2] == 0xBF )// skip 0xEF,0xBB,0xBF
			{
				TakeSnapshot(lpszString+3,nStringLen-3,CHtmlObject::CHARSET_UTF8);
			}
			else
			{
				TakeSnapshot(lpszString,nStringLen,CHtmlObject::CHARSET_MULTIBYTE);
			}
		}
		else
		{
			TakeSnapshot(lpszString,nStringLen,CHtmlObject::CHARSET_MULTIBYTE);
		}
	}
}
void CHtmlObject::TakeSnapshot(PBYTE lpszString,UINT nStringLen,UINT nFromCharset )
{
	//delete old snapshot
	DeleteSnapshot();

	if( lpszString && nStringLen > 0 )
	{

		//transform to TCHAR

		if( CHtmlHelper::CHARSET_UTF8 == nFromCharset )
		{

#ifdef _UNICODE 

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			m_nSnapshotStringLen = ::MultiByteToWideChar(CP_UTF8,0,(LPCSTR)lpszString,nStringLen,m_pszSnapshotBuffer,m_nSnapshotBufferLen);
#else
			::OutputDebugString(_T("no support"));

#endif //_UNICODE

		}
		else if( CHtmlHelper::CHARSET_UNICODE == nFromCharset )
		{

#ifdef _UNICODE 

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			::memcpy((LPVOID)m_pszSnapshotBuffer,lpszString,nStringLen);

#else

			m_nSnapshotBufferLen = nStringLen/2+1;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			m_nSnapshotStringLen = ::WideCharToMultiByte(CP_ACP,0,(LPWSTR)lpszString,nStringLen,(LPSTR)m_pszSnapshotBuffer,m_nSnapshotBufferLen,NULL,NULL);

#endif //_UNICODE

		}
		else
		{

#ifdef _UNICODE 

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset(m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			m_nSnapshotStringLen = ::MultiByteToWideChar(CP_ACP,0,(LPCSTR)lpszString,nStringLen,m_pszSnapshotBuffer,m_nSnapshotBufferLen);
#else

			m_nSnapshotBufferLen = nStringLen;
			m_pszSnapshotBuffer = new TCHAR[m_nSnapshotBufferLen];

			::memset((LPVOID)m_pszSnapshotBuffer,0,m_nSnapshotBufferLen*sizeof(TCHAR));
			::memcpy((LPVOID)m_pszSnapshotBuffer,lpszString,nStringLen);

#endif //_UNICODE

		}
	}
}
void CHtmlObject::DeleteSnapshot()
{
	//���M���𫬪�C
	InnerCleanupNode();

	if( m_pszSnapshotBuffer )
		delete []m_pszSnapshotBuffer;

	m_pszSnapshotBuffer = NULL;
	m_nSnapshotBufferLen = 0;
	m_nSnapshotStringLen = 0;
}
//
void CHtmlObject::Parse()
{
#ifdef _AFX
	CString strTrace;
	strTrace.Format(_T("CHtmlObject::Parse() --begin-->(%d)\r\n"),::GetTickCount());
	::OutputDebugString(strTrace);
#endif //_AFX

	InnerParse();

#ifdef _AFX
	strTrace.Format(_T("CHtmlObject::Parse() --end-->(%d)\r\n"),::GetTickCount());
	::OutputDebugString(strTrace);
#endif //_AFX
}
//
void CHtmlObject::InnerParse()
{
	LPTSTR pszFind = m_pszSnapshotBuffer;

	//���L�Ҧ����Ů桨
	while( *pszFind != _T('\0') && CHtmlObject::IsSpace(*pszFind) )
	{
		//�U�@�Ӧr��
		pszFind++;
	}
	//����I��'\0'�N�h�X
	do
	{
		// ���O��\0���A�åB�Ĥ@�Ӧr������<���h�m������\0���A�_�h����]�����C
		//�o��g����]�N�b��InnerSplitContent()��^�� ��<���i��w�g�Q�m������\0���C
		if( *pszFind != _T('\0') && *pszFind == _T('<') )
		{
			//�⡧<���m������\0��,���������C
			*pszFind = _T('\0');
			//�U�@�Ӧr���C
			pszFind++;
		}

		// ���O��\0��
		if( *pszFind != _T('\0') )
		{
			//�O�_���`��
			if( *pszFind == _T('!') )
			{
				//�ӽФ@���I�`�C
				tagNode *pstNode = InnerAllocNode();
				//�ѪR�`���A��^���O�`���᭱�����e�C
				pszFind = InnerSplitComment(pstNode,pszFind);
				//�쵲�졧����C�]�U�^
				InnerLinkNextNode(pstNode);
			}
			else
			{
				//�ӽФ@���I�`�C
				tagNode *pstNode = InnerAllocNode();
				//�ѪRtag�A��^���Otag�᭱�����e�C
				pszFind = InnerSplitTag(pstNode,pszFind);
				//�ѪRcontent��^���Ocontent�᭱�����e�C
				pszFind = InnerSplitContent(pstNode,pszFind);
				//�쵲�졧����C�]�U�^
				InnerLinkNextNode(pstNode);
			}
		}

	}while( *pszFind!= _T('\0') );
}
LPTSTR CHtmlObject::InnerSplitComment(CHtmlObject::tagNode * lpstNode,LPTSTR lpszTagString)
{
	LPTSTR pszFind = lpszTagString;
	//���V�`���}�Y(�w�g���L��<���r��)
	lpstNode->s_pszKey = pszFind;
	//�p�G�� <!-- *** -->
	if( ::_tcsnicmp(pszFind+1,_T("--"),2) == 0 )
	{
		//���L�`���аO���Y���A�}�l�d��C
		pszFind += 3;
		//�d���`�������A�õ������[��\0���C
		while( ::_tcsnicmp(pszFind,_T("-->"),3) != 0 )
		{
			//�U�@�Ӧr��
			pszFind++;
		}
		//���O��\0��
		if( *pszFind != _T('\0') )
		{
			//�⡧>���m������\0��,�����`������
			*(pszFind+2) = _T('\0');
			//���V�s���`�I�Τ��e�C
			pszFind += 3;
		}

	}
	//�_�h�� <! *** >
	else
	{
		//�d���`�������A�õ������[��\0���C
		while( *pszFind != _T('\0') && *pszFind != _T('>') )
		{
			//�U�@�Ӧr��
			pszFind++;
		}
		//����O��\0��
		if( *pszFind != _T('\0') )
		{
			//�⡧>���m������\0��,�����`�������C
			*pszFind = _T('\0');
			//���V�s���`�I�Τ��e�C
			pszFind++;
		}
	}

	//���@�ӡ�<��
	while( *pszFind != _T('\0') && *pszFind != _T('<') )
	{
		//�U�@�Ӧr��
		pszFind++;
	}

	return pszFind;
}
LPTSTR CHtmlObject::InnerSplitTag(CHtmlObject::tagNode * lpstNode,LPTSTR lpszTagString)
{
	LPTSTR pszFind = lpszTagString;

	//���V�}�Y(�w�g���L��<���r��)
	lpstNode->s_pszKey = pszFind;
	//�d��tag�����A�õ������[��\0���C
	while( *pszFind != _T('\0') && *pszFind != _T('>') && !CHtmlObject::IsSpace(*pszFind) )
	{
		//�U�@�Ӧr��
		pszFind++;
	}

	//���O��\0��
	if( *pszFind != _T('\0') )
	{
		if( *pszFind == _T('>') )
		{
			//�⡧>���m������\0��,�����`�������C
			*pszFind = _T('\0');
			//���V�s���`�I�Τ��e�C
			pszFind++;
			//��tag�S���ݩ�,����]�����F�C
				
		}
		else
		{
			//�⡧space,\r,\n,\t ���m������\0��,�����`�������C
			*pszFind = _T('\0');
			//���V�s���`�I�Τ��e�C
			pszFind++;

			//�p�G���O�����аO�A��ܦ�tag�����ݩʡ��ٻݭn�ѪR���ݩʡ��C
			if( *lpstNode->s_pszKey != _T('/') )
			{
				//���L�Ҧ����Ů桨,���Ĥ@���ݩʡC
				while( *pszFind != _T('\0') && CHtmlObject::IsSpace(*pszFind) )
				{
					//�U�@�Ӧr��
					pszFind++;
				}
				//�j����R���ݩʡ��C
				while( *pszFind != _T('\0') && *pszFind != _T('<') && *pszFind != _T('>') )
				{
					//�ҡG
					// key="value" key=value
					//���L�Ů�
					while( *pszFind != _T('\0') && CHtmlObject::IsSpace(*pszFind) )
					{
						//�U�@�Ӧr��
						pszFind++;
					}

					//���O��\0��
					if( *pszFind != _T('\0') )
					{
						//�ӽФ@���I�`�C
						tagNode *pstAttributeNode = InnerAllocNode();
						//���V���ݩʡ�Key�C
						pstAttributeNode->s_pszKey = pszFind;

						//�d��key������.
						while( *pszFind != _T('\0') && *pszFind != _T('=') && *pszFind != _T('>') )
						{
							//�U�@�Ӧr��
							pszFind++;
						}
						//���O��\0��
						if( *pszFind != _T('\0') )
						{
							if( *pszFind == _T('>') )
							{
								//�⡧>���m������\0��,���������C
								*pszFind = _T('\0');
								//���V�s���`�I�Τ��e�C
								pszFind++;
								//�쵲�졧����]�k�^�C
								InnerLinkRightNode(lpstNode,pstAttributeNode);
								//�w�g�I�졧>��,�ݭn���X�C
								break;
							}
							else
							{
								//�⡧=���m������\0��,���������C
								*pszFind = _T('\0');
								//���V�s���`�I�Τ��e�C
								pszFind++;
								
								//���O��\0��
								if( *pszFind != _T('\0') )
								{
									if( *pszFind == _T('"') )
									{
										//���L��"��
										pszFind++;
										//���V���ݩʡ�key��Value�C
										pstAttributeNode->s_pszValue = pszFind;

										//�d��Value������.
										while( *pszFind != _T('\0') && *pszFind != _T('\"') && *pszFind != _T('>') )
										{
											//�U�@�Ӧr��
											pszFind++;
										}
										//���O��\0��
										if( *pszFind != _T('\0') )
										{
											//�⡧",> ���m������\0��,���������C
											*pszFind = _T('\0');
											//���V�s���`�I�Τ��e�C
											pszFind++;
										}
									}
									else if( *pszFind == _T('\'') )
									{
										//���L��'��
										pszFind++;
										//���V���ݩʡ�key��Value�C
										pstAttributeNode->s_pszValue = pszFind;

										//�d��Value������.
										while( *pszFind != _T('\0') && *pszFind != _T('\'') && *pszFind != _T('>') )
										{
											//�U�@�Ӧr��
											pszFind++;
										}
										//���O��\0��
										if( *pszFind != _T('\0') )
										{
											//�⡧",<space> ���m������\0��,���������C
											*pszFind = _T('\0');
											//���V�s���`�I�Τ��e�C
											pszFind++;
										}
									}
									else
									{
										//���V���ݩʡ�key��Value�C
										pstAttributeNode->s_pszValue = pszFind;
										//�d��Value������.
										while( *pszFind != _T('\0') && *pszFind != _T(' ') && *pszFind != _T('>') )
										{
											//�U�@�Ӧr��
											pszFind++;
										}
										//���O��\0��
										if( *pszFind != _T('\0') )
										{
											//�⡧ ���m������\0��,���������C
											*pszFind = _T('\0');
											//���V�s���`�I�Τ��e�C
											pszFind++;
										}
									}
									
								}
							}
							
						}
					
						//�쵲�졧����]�k�^�C
						InnerLinkRightNode(lpstNode,pstAttributeNode);
					}
				} 
				
				//���L�o�ӵL�Ϊ��r���C
				if( *pszFind == _T('>') )
				{
					//���V�s���`�I�Τ��e�C
					pszFind++;
				}
			}
		}
	}

	return pszFind;
}
LPTSTR CHtmlObject::InnerSplitContent(CHtmlObject::tagNode * lpstNode,LPTSTR lpszTagString)
{
	LPTSTR pszFind = lpszTagString;
	
	if( ::_tcsnicmp(lpstNode->s_pszKey,_T("script"),6) == 0 )
	{
		pszFind = InnerSplitScript(lpstNode,pszFind);
	}
	else if( ::_tcsnicmp(lpstNode->s_pszKey,_T("style"),5) == 0 )
	{
		pszFind = InnerSplitStyle(lpstNode,pszFind);
	}
	else
	{
		pszFind = InnerSplitText(lpstNode,pszFind);
	}

	return pszFind;
}
LPTSTR CHtmlObject::InnerSplitText(CHtmlObject::tagNode * lpstNode,LPTSTR lpszTagString)
{
	LPTSTR pszFind = lpszTagString;
		
	//���L�Ҧ����Ů桨
	while( *pszFind != _T('\0') && CHtmlObject::IsSpace(*pszFind) )
	{
		//�U�@�Ӧr��
		pszFind++;
	}
	
	//�p�G _T('<') ��ܨS���奻�C
	if( *pszFind != _T('<') )
	{
		//���V�i���奻�C
		lpstNode->s_pszValue = pszFind;
		//�d��奻�����C
		while( *pszFind != _T('\0') && *pszFind != _T('<') && !CHtmlObject::IsSpace(*pszFind) )
		{
			//�U�@�Ӧr��
			pszFind++;
		}
		//���O��\0��
		if( *pszFind != _T('\0') )
		{
			if( *pszFind == _T('<') )
			{
				//�⡧<���m������\0��,���������C
				*pszFind = _T('\0');
				//���V�s���`�I�Τ��e�C
				pszFind++;
			}
			else
			{
				//�⡧space,\r,\n,\t,���m������\0��,���������C
				*pszFind = _T('\0');
				//���V�s���`�I�Τ��e�C
				pszFind++;

				//���@�ӡ�<��
				while( *pszFind != _T('\0') && *pszFind != _T('<') )
				{
					//�U�@�Ӧr��
					pszFind++;
				}
			}
		}
	}

	return pszFind;
}
LPTSTR CHtmlObject::InnerSplitScript(tagNode * lpstNode,LPTSTR lpszTagString)
{
	LPTSTR pszFind = lpszTagString;


#define  SCRIPT_MARK_MAX			1024
	UINT nMarkIndex = 0;
	TCHAR szMark[SCRIPT_MARK_MAX] = {_T('\0')}; //max 1024

	//���L�Ҧ����Ů桨
	while( *pszFind != _T('\0') && CHtmlObject::IsSpace(*pszFind) )
	{
		//�U�@�Ӧr��
		pszFind++;
	}

	if( *pszFind != _T('\0') && *pszFind != _T('<') )
	{
		//���V�i���奻�C
		lpstNode->s_pszValue = pszFind;

		while( *pszFind != _T('\0') )
		{
			//�p�G�r���Q��'�A"���]��h���r��A�o�������p��`���C
			if( szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
			{
				//�p�G�O// abc �h���L�C
				if( ::_tcsnicmp(pszFind,_T("//"),2) == 0   )
				{
					//���L�`�����Y���C
					pszFind +=2;
					//�d��`���������C
					while( *pszFind != _T('\0') && *pszFind != _T('\n') )
					{
						pszFind++;
					}
					//���L�`���������C
					if( *pszFind != _T('\0')  )
						pszFind++;

				}
				//�p�G�O/* abc */�h���L�C
				else if( ::_tcsnicmp(pszFind,_T("/*"),2) == 0   )
				{
					//���L�`�����Y���C
					pszFind +=2;
					//�d��`���������C
					while( ::_tcsnicmp(pszFind,_T("*/"),2) != 0 )
					{
						pszFind++;
					}
					//���L�`���������C
					if( *pszFind != _T('\0')  )
						pszFind +=2;
				}

			}

			if( *pszFind == _T('\\') && 
				( *(pszFind+1) == _T('\\') || 
				*(pszFind+1) == _T('(') || *(pszFind+1) == _T(')') || 
				*(pszFind+1) == _T('[') || *(pszFind+1) == _T(']') || 
				*(pszFind+1) == _T('{') || *(pszFind+1) == _T('}') || 
				*(pszFind+1) == _T('\'') || 
				*(pszFind+1) == _T('\"') ) )
			{
				//��N�r��
				pszFind+=2;

			}
			else if( *pszFind == _T('{') || *pszFind == _T('(') || *pszFind == _T('[') ||  (*pszFind == _T('\'') || *pszFind == _T('\"')) )
			{
				if( szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
				{
					if( nMarkIndex < SCRIPT_MARK_MAX )
					{
						if( nMarkIndex == 0 && szMark[nMarkIndex] == _T('\0')  )
							szMark[nMarkIndex] = *pszFind;
						else
							szMark[++nMarkIndex] = *pszFind;
					}
				}
				else if( szMark[nMarkIndex] == *pszFind )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;

			}
			else if( *pszFind == _T('}') ) 
			{
				if( szMark[nMarkIndex] == _T('{') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;
			}
			else if( *pszFind == _T(')') )
			{
				if( szMark[nMarkIndex] == _T('(') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;
			}
			else if( *pszFind == _T(']') ) 
			{
				if( szMark[nMarkIndex] == _T('[') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;
			}
			else if( *pszFind == _T('<') && szMark[0] == _T('\0') )  //nMarkIndex == 0 &&
			{
				//�⡧<���m������\0��,���������C
				*pszFind = _T('\0');
				//���V�s���`�I�Τ��e�C
				pszFind++;
				break;
			}
			else
			{
				pszFind++;
			}
		}
	}

	return pszFind;
}
LPTSTR CHtmlObject::InnerSplitStyle(CHtmlObject::tagNode * lpstNode,LPTSTR lpszTagString)
{
	LPTSTR pszFind = lpszTagString;

#define  STYLE_MARK_MAX			1024
	UINT nMarkIndex = 0;
	TCHAR szMark[STYLE_MARK_MAX] = {_T('\0')}; //max 1024

	//���L�Ҧ����Ů桨
	while( *pszFind != _T('\0') && CHtmlObject::IsSpace(*pszFind) )
	{
		//�U�@�Ӧr��
		pszFind++;
	}

	if( *pszFind != _T('\0') && *pszFind != _T('<') )
	{
		//���V�i���奻�C
		lpstNode->s_pszValue = pszFind;

		while( *pszFind != _T('\0') )
		{
			//�p�G�r���Q��(�A'�A"���]��h���r��A�o�������p��`���C
			if( szMark[nMarkIndex] != _T('(') && szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
			{
				//�p�G�O/* abc */�h���L�C
				if( ::_tcsnicmp(pszFind,_T("/*"),2) == 0   )
				{
					//���L�`�����Y���A�d��`���������C
					pszFind +=2;
					while( ::_tcsnicmp(pszFind,_T("*/"),2) != 0 )
					{
						pszFind++;
					}
					//���L�`���������C
					if( *pszFind != _T('\0')  )
						pszFind +=2;
				}
			}


			if( *pszFind == _T('{') || *pszFind == _T('(') || *pszFind == _T('[') ||  (*pszFind == _T('\'') || *pszFind == _T('\"')) )
			{
				if( szMark[nMarkIndex] != _T('\'') && szMark[nMarkIndex] != _T('\"') )
				{
					if( nMarkIndex < STYLE_MARK_MAX )
					{
						if( nMarkIndex == 0 && szMark[nMarkIndex] == _T('\0')  )
							szMark[nMarkIndex] = *pszFind;
						else
							szMark[++nMarkIndex] = *pszFind;
					}
				}
				else if( szMark[nMarkIndex] == *pszFind )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;

			}
			else if( *pszFind == _T('}') ) 
			{
				if( szMark[nMarkIndex] == _T('{') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;
			}
			else if( *pszFind == _T(')') )
			{
				if( szMark[nMarkIndex] == _T('(') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;
			}
			else if( *pszFind == _T(']') ) 
			{
				if( szMark[nMarkIndex] == _T('[') )
				{
					if( nMarkIndex >0  )
						szMark[nMarkIndex--] = _T('\0');
					else
						szMark[nMarkIndex] = _T('\0');
				}
				pszFind++;
			}
			else if( *pszFind == _T('<') && szMark[0] == _T('\0') )  //nMarkIndex == 0 &&
			{
				//�⡧<���m������\0��,���������C
				*pszFind = _T('\0');
				//���V�s���`�I�Τ��e�C
				pszFind++;
				break;
			}
			else
			{
				pszFind++;
			}
		}

	}

	return pszFind;
}

//////////////////////////////////////////////////////////////////////////////////////////CHtmlObject.cpp//////////////////////////////////////////////////////////////////////////
