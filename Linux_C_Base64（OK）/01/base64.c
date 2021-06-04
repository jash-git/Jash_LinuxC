//base64.c
#include "base64.h"
static char base64_table[255];
void base64_tableinit()
{
	int i,j;
	bzero(base64_table,255);
	for(j=0,i='A';i<='Z';i++) /*��base64�s�X��*/
		base64_table[i]=j++;
	for(i='a';i<='z';i++)
		base64_table[i]=j++;
    for(i='0';i<='9';i++)
		base64_table[i]=j++;
        base64_table['+']=j++;
        base64_table['/']=j++;
        base64_table['=']=j;
}
char* base64_decode(const char *cptr,char **rptr)
{
	char *res;
	int clen,len;
	static int init=0;
	if(cptr==NULL)
		return NULL;
	
	len=strlen(cptr);
	
	if(len%4) /*�s�F�X���r������O4������*/
		return NULL;

	if(!init)
	{
		init=1;
		base64_tableinit();
	}
	
	clen=len/4;
	if((res=(char *)malloc(len-clen))==NULL)
		return NULL;
	
	for(*rptr=res;clen--;)
	{
		
		*res=base64_table[*cptr++]<<2&0xfc;         /*cptr�᤻�첾�ʨ�̰���*/
		*res++|=base64_table[*cptr]>>4;               /*��ۤU�Ӧr���C��줸��res�C���*/
		*res=base64_table[*cptr++]<<4&0xf0;               /*��Rres���|���L�M0*/
		*res++|=base64_table[*cptr]>>2&0x0f;    /*�r���e���첾��C������C�|��*/
		*res=base64_table[*cptr++]<<6;
		if(*cptr!='=')                                                 /*=���G��*/
		*res++|=base64_table[*cptr++];
	}
	return *rptr;

}

char* base64_encode(const char *cptr,char **rptr)
{
	char *res;
	int i,clen,len;
	
	len=strlen(cptr);
	clen=len/3;
	
	if(cptr==NULL||(res=(char *)malloc(clen+3*2+len))==NULL)
		return NULL;

	for(*rptr=res;clen--;)
 	{
		*res++=*cptr >> 2 & 0x3f;          /*��ptr��6���Jres�C6��*/
		*res=*cptr++ << 4 & 0x30;          /*����ptr�̧C2��찪6��M��M0�� ����*/
		*res++|=*cptr >> 4;                  /*��ptr��4�쵹res�C4��*/
		*res=( *cptr++ & 0x0f ) << 2;        /*��ptr�C4�첾�ʨ찪6��*/
		*res++|=*cptr>>6;                  /*��ptr��2�쵹res�C2��*/
		*res++=*cptr++ & 0x3f;
	}

	if(i=len%3)                                       /*�B�z�h�l�r���u����ر��p�h�@�өΪ̨�Ӧr��*/
	{
		if(i==1)                                    /*�ھ�base64�s�X��=��*/
		{
			*res++=*cptr >> 2 & 0x3f;
			*res++=*cptr << 4 & 0x30;
			*res++='=';
			*res++='=';
		}
		else
		{
			*res++=*cptr>>2&0x3f;
			*res=*cptr++<<4&0x30;
			*res++|=*cptr>>4;
			*res++=(*cptr&0x0f)<<2;
			*res++='=';
		}
	}

	*res='=';                                               /*�O�ҳ̫ᵲ�쬰=������]�O�]��base64�ئ���0���s�X*/
	for(res=*rptr;*res!='=';res++)
		*res="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/="[*res];
	
	rptr[0][strlen(*rptr)-1]='\0';                 /*�h���̫�@��=��*/

	return *rptr;

}
