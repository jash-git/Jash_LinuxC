#include <iconv.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 1024   //BUFFER_SIZE must >= 2


void usage()
{
	printf("\nBig5ToGB2312 - Convert Big5 encoding file to GB2312 encoding file\n");
	printf("XCyber@sohu.com on August 7, 2008\n");
	printf("  Usage:\n");
	printf("      Big5ToGB2312 [Big5 file(in)]  [GB2312 file(out)]\n\n");
}


int main(int argc, char* argv[])
{
	FILE * pSrcFile = NULL;
	FILE * pDstFile = NULL;

	char szSrcBuf[BUFFER_SIZE];
	char szDstBuf[BUFFER_SIZE];

	size_t nSrc  = 0;
	size_t nDst  = 0;
	size_t nRead = 0;
	size_t nRet  = 0;

	char *pSrcBuf = szSrcBuf;
	char *pDstBuf = szDstBuf;

	iconv_t icv;
	int argument = 1;

	//check input arguments
	if(argc != 3)
	{
		usage();
		return -1;
	}


	pSrcFile = fopen(argv[1],"r");
	if(pSrcFile == NULL)
	{
		printf("can't open source file!\n");
		return -1;
	}

	pDstFile = fopen(argv[2],"w");
	if(pSrcFile == NULL)
	{
		printf("can't open destination file!\n");
		return -1;
	}

	//initialize iconv routine, perform conversion from BIG5 to GB2312
	//TODO: if you want to perfom other type of coversion, e.g. GB2312->BIG5, GB2312->UTF-8 ...
	//just change following two paremeters of iconv_open()
	icv = iconv_open("BIG5","UTF-8");
	if(icv == 0)
	{
		printf("can't initalize iconv routine!\n");
		return -1;
	}

	while(!feof(pSrcFile))
	{
		pSrcBuf = szSrcBuf;
		pDstBuf = szDstBuf;
		nDst = BUFFER_SIZE;

		// read data from source file
		nRead = fread(szSrcBuf + nSrc,sizeof(char),BUFFER_SIZE - nSrc,pSrcFile);
		if(nRead == 0)
			break;

		// the amount of data to be converted should include previous left data and current read data
		nSrc = nSrc + nRead; 

		//perform conversion
		nRet = iconv(icv,(const char**)&pSrcBuf,&nSrc,&pDstBuf,&nDst);

		if(nRet == -1)
		{
			// include all case of errno: E2BIG, EILSEQ, EINVAL
			//     E2BIG: There is not sufficient room at *outbuf.
			//     EILSEQ: An invalid multibyte sequence has been encountered in the input.
			//     EINVAL: An incomplete multibyte sequence has been encountered in the input
			// move the left data to the head of szSrcBuf in other to link it with the next data block
			memmove(szSrcBuf,pSrcBuf,nSrc);
		}

		//wirte data to destination file
		fwrite(szDstBuf,sizeof(char),BUFFER_SIZE - nDst,pDstFile);
		
	}
	iconv_close(icv);
	fclose(pSrcFile);
	fclose(pDstFile);

	printf("conversion complete.\n");

	return 0;
}

