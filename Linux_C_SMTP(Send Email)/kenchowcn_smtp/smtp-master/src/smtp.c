/*
 * 2013-9-10:build by ken <http://kenchowcn.com>
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <netdb.h>
#include "smtp.h"
#include "base64.h"

int stringCut(const unsigned char *pcSrc, const unsigned char *start, const unsigned char *end, unsigned char *pcDest)
{
	unsigned char *posStart = NULL, *posEnd = NULL;
	int len;
	
	if (0 == *pcSrc || NULL == pcDest)
	{
		return -1;
	}

	if (NULL == start)
	{
		posStart = (unsigned char*)pcSrc;
	}
	else
	{
		posStart = strstr(pcSrc, start);
		if (NULL == posStart)
		{
			return -1;
		}
		/* ignore header */
		posStart++;
	}

	if (NULL == end)
	{
		posEnd = posStart+strlen(posStart);
	}
	else
	{
		posEnd = strstr(pcSrc, end);
		if (NULL == posEnd)
		{
			return -1;
		}
	}
	
	len = posEnd - posStart;
	
	strncpy(pcDest, posStart, len);
	
	return len;
}
int connectSmtp(const unsigned char* smtpUrl, const unsigned short smtpPort)
{
    int socketFd = -1;
    struct sockaddr_in smtpAddr = {0};
	struct hostent *host = NULL;
    	
    if(NULL == (host = gethostbyname(smtpUrl)))
    {
        perror("gethostbyname...\n");
        exit(-1);
    }

	memset(&smtpAddr, 0, sizeof(smtpAddr));
    smtpAddr.sin_family = AF_INET;
    smtpAddr.sin_port = htons(smtpPort);
    smtpAddr.sin_addr = *((struct in_addr *)host->h_addr);
    
    socketFd = socket(PF_INET, SOCK_STREAM, 0);
    if(0 > socketFd)
    {
        perror("socket...\n");
        exit(-1);
    }

    if(0 > connect(socketFd, (struct sockaddr *)&smtpAddr, sizeof(struct sockaddr)))
    {
        close(socketFd);
        perror("connect...\n");
        exit(-1);
    }

    return socketFd;
}

int safeRead(int socketFd, unsigned char *readData, int readLen)
{
#ifdef NONEED_SELECT
	return recv(socketFd, readData, readLen, 0);
#else
	fd_set readFds;
	struct timeval tv;
	int ret, len = 0;

	if ((0 >= readLen) || (NULL == readData))
	{
		return -1;
	}

	FD_ZERO(&readFds);
	FD_SET(socketFd, &readFds);

	tv.tv_sec = 5;

	while(1)
	{
		ret = select(socketFd+1, &readFds, NULL, NULL, &tv);
		if (0 > ret)
		{
			perror("select...\n");
			/* reconnect or ... */
		}
		else if (0 == ret)
		{
			continue;
			/* reconnect or continue wait */
		}
		else
		{
			/* got it */
			if(FD_ISSET(socketFd, &readFds))
			{
				/* don't do read twice */
				len = recv(socketFd, readData, readLen, 0);
				/* take data,and leave */
				break;
			}
		}
	}	

	return len;
#endif
}

int safeWrite(int socketFd, const unsigned char *writeData, int writeLen)
{
#ifdef NONEED_SELECT
	return send(socketFd, writeData, writeLen, 0);
#else
	fd_set writeFds;
	struct timeval tv;
	int ret, len = 0;

	if ((0 >= writeLen) || (NULL == writeData))
	{
		return -1;
	}

	FD_ZERO(&writeFds);
	FD_SET(socketFd, &writeFds);

	tv.tv_sec = 5;

	while(1)
	{
		ret = select(socketFd+1, NULL, &writeFds, NULL, &tv);
		if (0 > ret)
		{
			perror("select...\n");
			/* reconnect or ... */
		}
		else if (0 == ret)
		{
			continue;
			/* reconnect or continue wait */
		}
		else
		{
			if(FD_ISSET(socketFd, &writeFds))
			{
				len = send(socketFd, writeData, writeLen, 0);
				break;
			}
		}
	}

	return len;
#endif	
}

/*
 * You can find more detail in here.
 * http://www.ietf.org/rfc/rfc821.txt
 */
int recvStatus(const unsigned char *recvString)
{
    char statusStr[4] = {0};
    
    memset(statusStr, 0, sizeof(statusStr));
    strncpy(statusStr, recvString, 3);
    
    SMTP_Print6("[%s][%d] status = %d\r\n", __FILE__, __LINE__, atoi(statusStr));
    
    switch (atoi(statusStr))
    {
        case 250:
        {
            break;
        }
        case 235:
        {
            break;
        }
        case 354:
        {
            break;
        }
        case 334:
        {
            break;
        }
        case 221:
        {
            break;
        }
        default:
        {
            printf("\r\n# You could be got exception status !\r\n");
            return -1;
        }
    }
    
    return 0;
}

int authEmail(const int socketFd, const unsigned char *mailAddr, const unsigned char *mailPasswd)
{
	int outSize = 0, stringLen;
	char readData[SMTP_MTU] = {0};
	char writeData[SMTP_MTU] = {0};
	char userName[MAX_EMAIL_LEN] = {0};
	char userPasswd[MAX_EMAIL_LEN] = {0};

	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);

	SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);

	/* Send: EHLO */
	safeWrite(socketFd, "EHLO Here\r\n", strlen("EHLO Here\r\n"));

	/* Recv: EHLO */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);

    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

    /* Send: AUTH LOGIN */
	safeWrite(socketFd, "AUTH LOGIN\r\n", strlen("AUTH LOGIN\r\n"));

	/* Recv: AUTH LOGIN */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);

    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	/* Send: username */	
	memset(&userName, 0, MAX_EMAIL_LEN);
	memset(&writeData, 0, SMTP_MTU);
	stringCut((unsigned char*)mailAddr, NULL, "@", userName);

	outSize = BASE64_SIZE(strlen(userName));
	base64_encode(writeData, outSize, userName, strlen(userName));
	strcat(writeData, "\r\n");
	safeWrite(socketFd, writeData, strlen(writeData));
	
	/* Recv: username */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);

    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	/* Send: passwd */	
	memset(&userPasswd, 0, MAX_EMAIL_LEN);
	strcpy(userPasswd, mailPasswd);
	memset(&writeData, 0, SMTP_MTU);
	outSize = BASE64_SIZE(strlen(userPasswd));
	base64_encode(writeData, outSize, userPasswd, strlen(userPasswd));
	strcat(writeData, "\r\n");
    safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: passwd */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);

    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	return 0;
}

int sendEmail(const int socketFd, const unsigned char *fromMail, const unsigned char *toMail,
                 const unsigned char *textMail, const int textLen)
{
	char readData[SMTP_MTU] = {0};
	char writeData[SMTP_MTU] = {0};

	/* Send: MAIL FROM */
	memset(&writeData, 0, SMTP_MTU);
	sprintf(writeData, "MAIL FROM: <%s>\r\n", fromMail);
    safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: MAIL FROM */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);
    
    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	/* Send: RCPT TO */
	memset(&writeData, 0, SMTP_MTU);
	sprintf(writeData, "RCPT TO: <%s>\r\n", toMail);
    safeWrite(socketFd, writeData, strlen(writeData));

	/* Recv: RCPT TO */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);
    
    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);
    
	/* Send: DATA */
	memset(&writeData, 0, SMTP_MTU);
    safeWrite(socketFd, "DATA\r\n", strlen("DATA\r\n"));

	/* Recv: DATA */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);
    
    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	/* Send: MAIL TEXT */
    safeWrite(socketFd, textMail, textLen);

	/* Recv: MAIL TEXT */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);
    
    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	/* Send: QUIT */
	memset(&writeData, 0, SMTP_MTU);
    safeWrite(socketFd, "QUIT\r\n", strlen("QUIT\r\n"));

	/* Recv: QUIT */
	memset(&readData, 0, SMTP_MTU);
    safeRead(socketFd, readData, SMTP_MTU);
    
    SMTP_Print6("[%s][%d]recv: %s\r\n", __FILE__, __LINE__, readData);
    recvStatus(readData);

	return 0;
}

/* subject and body could be null. */
int mailText(unsigned char **mail, const unsigned char *fromMail, const unsigned char *toMail, 
                 const unsigned char *mailSubject, const unsigned char *mailBody)
{
	char *mailText = NULL;
	int mailTextLen = 0;
	char fromName[MAX_EMAIL_LEN] = {0};
	char toName[MAX_EMAIL_LEN] = {0};

	if ((NULL == *mail) || (NULL == fromMail) || (NULL == toMail))
	{
		printf("[%s][%d] \r\n", __FILE__, __LINE__);
		return -1;
	}

	mailTextLen = strlen(fromMail)+strlen(toMail)+strlen(mailSubject)+strlen(mailBody)+500;

	mailText = calloc(mailTextLen, 1);
	if (NULL == mailText)
	{
		perror("malloc...\n");
		return -1;
	}

	memset(&fromName, 0, MAX_EMAIL_LEN);
	stringCut(fromMail, NULL, "@", fromName);

	memset(&toName, 0, MAX_EMAIL_LEN);
	stringCut(toMail, NULL, "@", toName);
	
	snprintf(mailText, mailTextLen, "From: \"%s\"<%s>\r\nTo: \"%s\"<%s>\r\nSubject: %s\r\nMIME-Version:1.0\r\nContent-Type:multipart/mixed;boundary=\"%s\"\r\n\r\n--%s\r\nContent-Type: text/plain; charset=\"gb2312\"\r\n\r\n%s\r\n\r\n--%s\r\n", fromName, fromMail, toName, toMail, mailSubject, TEXT_BOUNDARY, TEXT_BOUNDARY, mailBody, TEXT_BOUNDARY);

	*mail = realloc(*mail, strlen(*mail)+strlen(mailText)+1);
	if (NULL == *mail)
	{
		perror("realloc...\n");
		/* what should I do? */
		return -1;
	}
	
	strcat(*mail, mailText);

	free(mailText);

	/* If I am in danger, let me konw, please */
	return (mailTextLen - strlen(*mail));
}

/* static attachmemt size */
int mailAttachment(unsigned char **mail, const unsigned char *filePath)
{
	FILE *fp = NULL;
	int fileSize, base64Size, headerSize, len;
	char *attach = NULL, *base64Attach = NULL, *attachHeader = NULL;
    char fileName[MAX_EMAIL_LEN] = {0};
    const char *contentType = "Content-Type: application/octet-stream";
    const char *contentEncode = "Content-Transfer-Encoding: base64";
    const char *contentDes = "Content-Disposition: attachment";

	fp = fopen(filePath, "rb");
	if (NULL == fp)
	{
		perror("open...");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	if (0 > fileSize)
	{
		perror("ftell...\n");
		return -1;
	}
	
	rewind(fp);

	attach = calloc(fileSize, 1);
	if (NULL == attach)
	{
		perror("malloc...");
		return -1;
	}
    
    headerSize = strlen(contentType)+strlen(contentEncode)+strlen(contentDes)+200;
    attachHeader = calloc(headerSize, 1);
    if (NULL == attach)
	{
		perror("malloc...");
		return -1;
	}
    
    /* attachment header */
    stringCut(filePath, "/", NULL, fileName);
    
    sprintf(attachHeader, "%s;name=\"%s\"\r\n%s\r\n%s;filename=\"%s\"\r\n\r\n", contentType, fileName, contentEncode, contentDes, fileName);
    
	base64Size = BASE64_SIZE(fileSize);
	base64Attach = calloc(base64Size, 1);
	if (NULL == base64Attach)
	{
		perror("malloc...");
		return -1;
	}

	len = fread(attach, sizeof(char), fileSize, fp);

	SMTP_Print6("[%s][%d] %s size = %d, base64Size = %d \r\n",__FILE__, __LINE__, filePath, fileSize, base64Size);

    /* attachment transform to base64 */
	base64_encode(base64Attach, base64Size, attach, fileSize);

	free(attach);

	*mail = realloc(*mail, strlen(*mail)+headerSize+base64Size+1);
	if (NULL == *mail)
	{
		perror("realloc...\n");
		/* what should I do? */
		return -1;
	}

    strcat(*mail, attachHeader);
	strcat(*mail, base64Attach);
    
    free(attachHeader);
	free(base64Attach);

	return fileSize;
}

int mailEnd(unsigned char **mail)
{
	char bodyEnd[200] = {0};
	int len;

	memset(bodyEnd, 0, sizeof(bodyEnd));
	sprintf(bodyEnd, "\r\n--%s--\r\n\r\n.\r\n", TEXT_BOUNDARY);
	
	len = strlen(bodyEnd);

	*mail = realloc(*mail, strlen(*mail)+len+1);
	if (NULL == *mail)
	{
		perror("realloc...\n");
		/* what should I do? */
		return -1;
	}

	strcat(*mail, bodyEnd);

	return 0;
}

int main()
{
	int fd = 0, ret;
	unsigned char *mail = NULL;
	const unsigned char *filePath = "./test.jpg";
	const unsigned char *mailSubject = "test";
	const unsigned char *mailBody = "hello smtp!!";

	const unsigned char *fromMailAddr= "centnet_voip@163.com";
	const unsigned char *mailPasswd= "centnet";
	const unsigned char *toMailAddr= "centnet_voip@163.com";
	const unsigned char *smtpUrl = "smtp.163.com";

	mail = calloc(1, 1);

	/* prepare mail data */
	ret = mailText(&mail, fromMailAddr, toMailAddr, mailSubject, mailBody);
	ret = mailAttachment(&mail, filePath);
	ret = mailEnd(&mail);
    
	SMTP_Print6("\r\n%s \r\n", mail);
    
    printf("\r\nprepare email OK ...\r\n");

	/* send to buddy */
	fd = connectSmtp(smtpUrl, SMTP_PORT);

	printf("connect OK ...\r\n");
	
	ret = authEmail(fd, fromMailAddr, mailPasswd);

	printf("auth OK ...\r\n");
	
	ret = sendEmail(fd, fromMailAddr, toMailAddr, mail, strlen(mail));

	printf("send OK ...\r\n");

	return 0;
}


