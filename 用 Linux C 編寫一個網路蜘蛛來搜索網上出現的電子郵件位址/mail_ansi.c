#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>
#include <locale.h>

#define USERAGENT "Wget/1.10.2"
#define ACCEPT "*/*"
#define ACCEPTLANGUAGE "zh-cn,zh;q=0.5"
#define ACCEPTENCODING "gzip,deflate"
#define ACCEPTCHARSET "gb2312,utf-8;q=0.7,*;q=0.7"
#define KEEPALIVE "300"
#define CONNECTION "keep-alive"
#define CONTENTTYPE "application/x-www-form-urlencoded"

#define MAXFILENAME 14
#define DEBUG 1

typedef struct webnode {
        char * host; /* 網頁所在的主機 */
        int port; /* 網路服務器所使用的埠 */
        char * dir; /* 網頁所在的目錄 */
        char * page; /* 網頁檔案名 */
        char * file; /* 本地保存的檔案名 */
        char IsHandled; /* 是否處理過 */
        struct webnode * brother; /* 兄弟節點鏈表指針 */
        struct webnode * child; /* 子節點鏈表指針 */
} WEBNODE;

struct sockaddr_in server_addr;
int sockfd = 0, dsend = 0, totalsend = 0, nbytes = 0, reqn = 0, i = 0, j = 0, ret = 0;
struct hostent *host;
char request[409600] = "", buffer[1024] = "", httpheader[1024] = "";
int FileNumber = 0;
char e[2] = "@/";
WEBNODE * NodeHeader, * NodeTail, * NodeCurr;
char * mapped_mem;

int GetHost(char * , char ** , char ** , int * , char ** ); /**/
void AnalyzePage(WEBNODE *); /**/
void AddInitNode(char *, char *, int, char * ); /**/
void HandleInitNode(WEBNODE *); /**/
void DisplayNode(WEBNODE *); /**/
void HandOneNode(WEBNODE *); /**/
void DoneWithList(int); /**/
void DoOnce(); /**/
void ConnectWeb(void); /**/
void SendRequest(void); /**/
void ReceiveResponse(void); /**/
void GetEmail(char * ); /**/
void GetLink(char * ); /**/
void GetBeforePos(char * , char ** ); /**/
void GetAfterPos(char * , char ** ); /**/
void AddChildNode(WEBNODE * , char * ); /**/
void GetAfterPosWithSlash(char * , char ** ); /**/
void GetMemory(char ** , int ); /**/
int IsExistWeb(WEBNODE * , char * , char * , int , char * ); /**/
void Rstrchr(char * , int , char ** ); /**/
int GetLocalAgent(char * UserAgent, char * Accept, char * AcceptLanguage, char * AcceptEncoding, char * AcceptCharset, char * KeepAlive, char * Connection, char * ContentType); /**/

/**************************************************************
  功能：設置 HTTP 協定頭內容的一些固定值
 ***************************************************************/
int GetLocalAgent(char * UserAgent, char * Accept, char * AcceptLanguage, char * AcceptEncoding, char * AcceptCharset, char * KeepAlive, char * Connection, char * ContentType)
{
        memcpy(UserAgent, USERAGENT, strlen(USERAGENT));
        memcpy(Accept, ACCEPT, strlen(ACCEPT));
        memcpy(AcceptLanguage, ACCEPTLANGUAGE, strlen(ACCEPTLANGUAGE));
        memcpy(AcceptEncoding, ACCEPTENCODING, strlen(ACCEPTENCODING));
        memcpy(AcceptCharset, ACCEPTCHARSET, strlen(ACCEPTCHARSET));
        memcpy(KeepAlive, KEEPALIVE, strlen(KEEPALIVE));
        memcpy(Connection, CONNECTION, strlen(CONNECTION));
        memcpy(ContentType, CONTENTTYPE, strlen(CONTENTTYPE));
        return 0;
}

/**************************************************************
  功能：在字串 s 裏搜索 x 字元，並設置指標 d 指向該位置
 ***************************************************************/
void Rstrchr(char * s, int x, char ** d)
{
        int len = strlen(s) - 1;
        while(len >= 0) {
                if(x == s[len]) {(*d) = s + len; return;}
                len--;
        }
        (*d) = 0;
}

/**************************************************************
  功能：連接一個網站伺服器
 ***************************************************************/
void ConnectWeb(void) { /* connect to web server */
        /* create a socket descriptor */
        if((sockfd=socket(PF_INET,SOCK_STREAM,0))==-1)
        {
                fprintf(stderr,"\tSocket Error:%s\a\n",strerror(errno));
                exit(1);
        }

        /* bind address */
        bzero(&server_addr, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(NodeCurr->port);
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);

        /* connect to the server */
        if(connect(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
        {
                fprintf(stderr, "\tConnect Error:%s\a\n", strerror(errno));
                exit(1);
        }
}

/**************************************************************
  功能：向網站發送 HTTP 請求
 ***************************************************************/
void SendRequest(void) { /* send my http-request to web server */
        dsend = 0;
        totalsend = 0;
        nbytes=strlen(request);
        while(totalsend < nbytes) {
                dsend = write(sockfd, request + totalsend, nbytes - totalsend);
                if(dsend==-1) 
                {
                        fprintf(stderr, "\tsend error!%s\n", strerror(errno));
                        exit(0);
                }
                totalsend+=dsend;
                fprintf(stdout, "\n\tRequest.%d %d bytes send OK!\n", reqn, totalsend);
        }
}

/**************************************************************
  功能：接收網站的 HTTP 返回
 ***************************************************************/
void ReceiveResponse(void) { /* get response from web server */
        fd_set writefds;
        struct timeval tival;
        int retry = 0;
        FILE * localfp = NULL;

        i=0; j = 0;
__ReCeive:
        FD_ZERO(&writefds);
        tival.tv_sec = 10;
        tival.tv_usec = 0;
        if(sockfd > 0) 
                FD_SET(sockfd, &writefds);
        else 
        {
                fprintf(stderr, "\n\tError, socket is negative!\n"); 
                exit(0);
        }

        ret = select(sockfd + 1, &writefds, NULL, NULL, &tival);
        if(ret ==0 ) {
                if(retry++ < 10) goto __ReCeive;
        }
        if(ret <= 0) {
                fprintf(stderr, "\n\tError while receiving!\n");
                exit(0);
        }

        if(FD_ISSET(sockfd, &writefds)) {
                memset(buffer, 0, 1024);
                memset(httpheader, 0, 1024);
                if((localfp = fopen(NodeCurr->file, "w")) == NULL) {
                        if(DEBUG) 
                                fprintf(stderr, "create file '%s' error\n", NodeCurr->file); 
                        return;
                }
                /* receive data from web server */
                while((nbytes=read(sockfd,buffer,1))==1)
                {
                        if(i < 4) { /* 獲取 HTTP 消息頭 */
                                if(buffer[0] == '\r' || buffer[0] == '\n') 
                                        i++;
                                else 
                                        i = 0;
                                memcpy(httpheader + j, buffer, 1); j++;
                        }
                        else { /* 獲取 HTTP 消息體 */
                                fprintf(localfp, "%c", buffer[0]); /* print content on the screen */
                                //fprintf(stdout, "%c", buffer[0]); /* print content on the screen */
                                i++;
                        }
                }
                fclose(localfp);
        }
}

/**************************************************************
  功能：執行一次 HTTP 請求
 ***************************************************************/
void DoOnce() /* send and receive */
{ 
        ConnectWeb(); /* connect to the web server */

        /* send a request */
        SendRequest();

        /* receive a response message from web server */
        ReceiveResponse();

        close(sockfd); /* because HTTP protocol do something one connection, so I can close it after receiving */
}

/**************************************************************
  功能：執行 HTTP 請求
 ***************************************************************/
void DoneWithList(int flag) 
{
        if(flag)
                fprintf(stdout, "\tRequest.%d is:\n%s", ++reqn, request);
        DoOnce();
        if(flag) 
                fprintf(stdout, "\n\tThe following is the response header:\n%s", httpheader);
}

/**************************************************************
  功能：從字串 src 中分析出網站地址和埠，並得到檔和目錄
 ***************************************************************/
int GetHost(char * src, char ** web, char ** file, int * port, char ** dir) {
        char * pA, * pB, * pC;
        int len;

        *port = 0;
        if(!(*src)) return -1;
        pA = src;
        if(!strncmp(pA, "http://", strlen("http://"))) pA = src+strlen("http://");
        /* else if(!strncmp(pA, "https://", strlen("https://"))) pA = src+strlen("https://"); */
        else return 1;
        pB = strchr(pA, '/');
        if(pB) {
                len = strlen(pA) - strlen(pB);
                GetMemory(web, len);
                memcpy((*web), pA, len);
                if(*(pB+1)) {
                        Rstrchr(pB + 1, '/', &pC);
                        if(pC) len = strlen(pB + 1) - strlen(pC);
                        else len = 0;
                        if(len > 0) {
                                GetMemory(dir, len);
                                memcpy((*dir), pB + 1, len);

                                if(pC + 1) {
                                        len = strlen(pC + 1);
                                        GetMemory(file, len);
                                        memcpy((*file), pC + 1, len);
                                }
                                else {
                                        len = 1;
                                        GetMemory(file, len);
                                        memcpy((*file), e, len);
                                }
                        }
                        else {
                                len = 1;
                                GetMemory(dir, len);
                                memcpy((*dir), e + 1, len);

                                len = strlen(pB + 1);
                                GetMemory(file, len);
                                memcpy((*file), pB + 1, len);
                        }
                }
                else {
                        len = 1;
                        GetMemory(dir, len);
                        memcpy((*dir), e + 1, len);

                        len = 1;
                        GetMemory(file, len);
                        memcpy((*file), e, len);
                }
        }
        else {
                len = strlen(pA);
                GetMemory(web, len);
                memcpy((*web), pA, strlen(pA));
                len = 1;
                GetMemory(dir, len);
                memcpy((*dir), e + 1, len);
                len = 1;
                GetMemory(file, len);
                memcpy((*file), e, len);
        }

        pA = strchr((*web), ':');
        if(pA) *port = atoi(pA + 1);
        else *port = 80;

        return 0;
}

/*********************************************************************
 *filename: mailaddrsearch.c
 *purpose: 用 C 語言編寫一個網路蜘蛛來搜索網上出現的電子郵件位址
 *tidied by: zhoulifa(zhoulifa@163.com) 周立發(http://zhoulifa.bokee.com)
 Linux愛好者 Linux知識傳播者 SOHO族 開發者 最擅長C語言
 *date time:2006-08-31 21:00:00
 *Note: 任何人可以任意複製代碼並運用這些文檔，當然包括你的商業用途
 * 但請遵循GPL
 *Thanks to: www.gd-linux.org 廣東省 Linux 公共服務技術支援中心
 *********************************************************************/

int main(int argc, char ** argv)
{
        int WebPort;
        char * WebHost = 0, * PageAddress = 0, * WebDir = 0;
        if(argc < 2)
        {
                if(DEBUG) fprintf(stdout, "Command error, you should input like this:\n\t%s WebPageAddress1 WebPageAddress2 WebPageAddress3 ...", argv[0]);
                exit(0);
        }

        NodeHeader = NodeTail = NodeCurr = 0;
        //setlocale(LC_ALL, "zh_CN.gb2312");

        for(i = 1; i < argc; i++) 
        {
                ret = GetHost(argv[i], &WebHost, &PageAddress, &WebPort, &WebDir); /* Get web page info */
                if(ret) 
                {
                        if (DEBUG) 
                                fprintf(stdout, "GetHost error from '%s'\n", argv[i]);
                        exit(0);
                }
                AddInitNode(WebHost, PageAddress, WebPort, WebDir); /* add this page to chain */
        }
        free(WebHost);
        free(PageAddress);
        free(WebDir);

        if(DEBUG) 
        {
                fprintf(stdout, "\nDisplay.%5d:", FileNumber);
                DisplayNode(NodeHeader); /* display every node */
        }

        HandleInitNode(NodeHeader); /* handle every page */
        return 0;
}

/**************************************************************
  功能：分析網頁
 ***************************************************************/
void AnalyzePage(WEBNODE * node)
{
        int fd;
        int flength = 0;
        fd = open(node->file, O_RDONLY);
        if(fd == -1) goto __AnalyzeDone;
        flength = lseek(fd, 1, SEEK_END);
        write(fd, "\0", 1);
        lseek(fd, 0, SEEK_SET);
        mapped_mem = mmap(0, flength, PROT_READ, MAP_PRIVATE, fd, 0);
        GetEmail(mapped_mem);
        GetLink(mapped_mem);
        close(fd);
        munmap(mapped_mem, flength);
__AnalyzeDone:
        close(fd);
        node->IsHandled = 1;
        remove(node->file);
}

/**************************************************************
  功能：為根節點設置兄弟節點
 ***************************************************************/
void AddInitNode(char * Host, char * Page, int Port, char * Dir)
{
        WEBNODE * NewNode;
        char filename[MAXFILENAME + 1] = "";

        if(NodeHeader == NULL) 
                NewNode = NodeHeader = (WEBNODE *)malloc(sizeof(WEBNODE));
        else 
                NodeTail->brother = NewNode = (WEBNODE *)malloc(sizeof(WEBNODE));
        memset(NewNode, 0, sizeof(WEBNODE));
        NewNode->host = (char *)malloc(strlen(Host) + 1);
        memset(NewNode->host, 0, strlen(Host) + 1);
        NewNode->page = (char *)malloc(strlen(Page) + 1);
        memset(NewNode->page, 0, strlen(Page) + 1);
        NewNode->dir = (char *)malloc(strlen(Dir) + 1);
        memset(NewNode->dir, 0, strlen(Dir) + 1);
        NewNode->file = (char *)malloc(MAXFILENAME + 1);
        memset(NewNode->file, 0, MAXFILENAME + 1);
        strcpy(NewNode->host, Host);
        strcpy(NewNode->page, Page);
        strcpy(NewNode->dir, Dir);
        sprintf(filename, "file%05d.html", FileNumber++);
        strcpy(NewNode->file, filename);
        NewNode->port = Port;
        NewNode->IsHandled = 0;
        NewNode->brother = 0;
        NewNode->child = 0;
        NodeTail = NewNode;
}

/**************************************************************
  功能：處理根節點資訊
 ***************************************************************/
void HandleInitNode(WEBNODE * node)
{
        WEBNODE * CurrentNode = 0;
        CurrentNode = node;
        if(CurrentNode) 
        {
                while(CurrentNode) 
                {
                        if(CurrentNode->IsHandled == 0) 
                        {
                                HandOneNode(CurrentNode);
                                if(DEBUG) 
                                {
                                        fprintf(stdout, "\nDisplay.%5d:", FileNumber);
                                        DisplayNode(NodeHeader); /* display every node */
                                }
                        }
                        CurrentNode = CurrentNode->brother;
                }
                CurrentNode = node;
                while(CurrentNode) 
                {
                        if(CurrentNode->child && CurrentNode->child->IsHandled == 0) 
                        {
                                HandleInitNode(CurrentNode->child);
                        }
                        CurrentNode = CurrentNode->brother;
                }
        }
}

/**************************************************************
  功能：顯示年有節點資訊
 ***************************************************************/
void DisplayNode(WEBNODE * NodeHeader)
{
        WEBNODE * TempNode;
        TempNode = NodeHeader;
        fprintf(stdout, "\n");
        while(TempNode) {
                if(!strcmp(TempNode->dir, "/")) fprintf(stdout, "\t%s:%d%s%s => %s %d\n", TempNode->host, TempNode->port, TempNode->dir, strcmp(TempNode->page, "@")?TempNode->page:"", TempNode->file, TempNode->IsHandled);
                else fprintf(stdout, "\t%s:%d/%s/%s => %s %d\n", TempNode->host, TempNode->port, TempNode->dir, strcmp(TempNode->page, "@")?TempNode->page:"", TempNode->file, TempNode->IsHandled);
                TempNode = TempNode->brother;
        }
        TempNode = NodeHeader;
        while(TempNode) {
                if(TempNode->child) DisplayNode(TempNode->child);
                TempNode = TempNode->brother;
        }
}

/**************************************************************
  功能：處理單個節點資訊
 ***************************************************************/
void HandOneNode(WEBNODE * node)
{
        char UserAgent[1024] = "", Accept[1024] = "", AcceptLanguage[1024] = "", AcceptEncoding[1024] = "", AcceptCharset[1024] = "", KeepAlive[1024] = "", Connection[1024] = "", ContentType[1024] = "";

        NodeCurr = node;
        if((host=gethostbyname(NodeCurr->host))==NULL) /* get ip address by domain */
        {
                if(DEBUG) 
                        fprintf(stderr,"\tGethostname '%s' error, %s\n", NodeCurr->host, strerror(errno));
                exit(1);
        }
        GetLocalAgent(UserAgent, Accept, AcceptLanguage, AcceptEncoding, AcceptCharset, KeepAlive, Connection, ContentType); /* Get client browser information */

        if(strcmp(NodeCurr->dir, "/")) 
                sprintf(request, "GET /%s/%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: %s\r\nConnection: %s\r\n\r\n", NodeCurr->dir, strcmp(NodeCurr->page, "@")?NodeCurr->page:"", NodeCurr->host, UserAgent, Accept, Connection);
        else 
                sprintf(request, "GET %s%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\nAccept: %s\r\nConnection: %s\r\n\r\n", NodeCurr->dir, strcmp(NodeCurr->page, "@")?NodeCurr->page:"", NodeCurr->host, UserAgent, Accept, Connection);
        DoneWithList(1);
        AnalyzePage(NodeCurr);
}

/**************************************************************
  功能：從字串 src 中分析出郵件位址保存到文件
 ***************************************************************/
void GetEmail(char * src)
{
        char * pa, * pb, * pc, *pd;
        char myemail[1024] = "";
        FILE * mailfp = NULL;
        if((mailfp = fopen("email.txt", "a+")) == NULL) return;
        pa = src;
        while((pb = strchr(pa, '@'))) {
                GetBeforePos(pb, &pc);
                GetAfterPos(pb, &pd);
                if(pc && pd && (strlen(pc) > (strlen(pd) + 3))) {
                        memset(myemail, 0, 1024);
                        memcpy(myemail, pc, strlen(pc) - strlen(pd));
                        if(strcmp(NodeCurr->dir, "/")) fprintf(mailfp, "%s\thttp://%s/%s/%s\n", myemail, NodeCurr->host, NodeCurr->dir, strcmp(NodeCurr->page, "@")?NodeCurr->page:"");
                        else fprintf(mailfp, "%s\thttp://%s%s%s\n", myemail, NodeCurr->host, NodeCurr->dir, strcmp(NodeCurr->page, "@")?NodeCurr->page:"");
                        if(*(pd + 1)) pa = pd + 1;
                        else break;
                }
                else if(*(pb + 1)) pa = pb + 1;
                else break;
        }
        fclose(mailfp);
}

/**************************************************************
  功能：從 src 中找出前面的字母、數位等內含，即 email 位址中 @ 的前面部分
 ***************************************************************/
void GetBeforePos(char * src, char ** d)
{
        char * x;
        if(src - 1) x = src - 1;
        else {*d = 0; return ;}
        while(x) {
                if(*x >= 'a' && *x <= 'z') {x--; continue;}
                else if(*x >= 'A' && *x <= 'Z') {x--; continue;}
                else if(*x >= '0' && *x <= '9') {x--; continue;}
                else if(*x == '.' || *x == '-' || *x == '_') {x--; continue;}
                else {break;}
        }
        x++;
        if(x) *d = x;
        else *d = 0;
}

/**************************************************************
  功能：從 src 中找出後面的字母、數位等內含，即 email 位址中 @ 的後面部分
 ***************************************************************/
void GetAfterPos(char * src, char ** d)
{
        char * x;
        if(src + 1) x = src + 1;
        else {*d = 0; return ;}
        while(x) {
                if(*x >= 'a' && *x <= 'z') {x++; continue;}
                else if(*x >= 'A' && *x <= 'Z') {x++; continue;}
                else if(*x >= '0' && *x <= '9') {x++; continue;}
                else if(*x == '.' || *x == '-' || *x == '_') {x++; continue;}
                else {break;}
        }
        if(x) *d = x;
        else *d = 0;
}

/**************************************************************
  功能：從 src 中找出前面的字母、數位等內含，即一個網頁位址中主機名後面的部分
 ***************************************************************/
void GetAfterPosWithSlash(char * src, char ** d)
{
        char * x;
        if(src) x = src;
        else {*d = 0; return ;}
        while(x) {
                if(*x >= 'a' && *x <= 'z') {x++; continue;}
                else if(*x >= 'A' && *x <= 'Z') {x++; continue;}
                else if(*x >= '0' && *x <= '9') {x++; continue;}
                else if(*x == '.' || *x == '-' || *x == '_' || *x == '=') {x++; continue;}
                else if(*x == ':' || *x == '/' || *x == '?' || *x == '&') {x++; continue;}
                else {break;}
        }
        if(x) *d = x;
        else *d = 0;
}

/**************************************************************
  功能：為 myanchor 分配 len 大小的記憶體
 ***************************************************************/
void GetMemory(char ** myanchor, int len)
{
        if(!(*myanchor)) (*myanchor) = (char *)malloc(len + 1);
        else (*myanchor) = (char *)realloc((void *)(*myanchor), len + 1);
        memset((*myanchor), 0, len + 1);
}

/**************************************************************
  功能：從 src 中分析出網頁鏈結，並加入到當前節點的子節點上
 ***************************************************************/
void GetLink(char * src)
{
        char * pa, * pb, * pc;
        char * myanchor = 0;
        int len = 0;

        pa = src;
        do {
                if((pb = strstr(pa, "href='"))) {
                        pc = strchr(pb + 6, '\'');
                        len = strlen(pb + 6) - strlen(pc);
                        GetMemory(&myanchor, len);
                        memcpy(myanchor, pb + 6, len);
                }
                else if((pb = strstr(pa, "href=\""))) {
                        pc = strchr(pb + 6, '"');
                        len = strlen(pb + 6) - strlen(pc);
                        GetMemory(&myanchor, len);
                        memcpy(myanchor, pb + 6, len);
                }
                else if((pb = strstr(pa, "href="))) {
                        GetAfterPosWithSlash(pb + 5, &pc);
                        len = strlen(pb + 5) - strlen(pc);
                        GetMemory(&myanchor, len);
                        memcpy(myanchor, pb + 5, len);
                }
                else {goto __returnLink ;}
                /*
                   if(DEBUG) {
                   if(strcmp(NodeCurr->dir, "/")) fprintf(stdout, "%s\thttp://%s/%s/%s\n", myanchor, NodeCurr->host, NodeCurr->dir, strcmp(NodeCurr->page, "`")?NodeCurr->page:"");
                   else fprintf(stdout, "%s\thttp://%s%s%s\n", myanchor, NodeCurr->host, NodeCurr->dir, strcmp(NodeCurr->page, "`")?NodeCurr->page:"");
                   }
                   */
                if(strlen(myanchor) > 0) AddChildNode(NodeCurr, myanchor);
                if(pc + 1) pa = pc + 1;
        }while(pa);
__returnLink:
        return;
}

/**************************************************************
  功能：為當前節點增加子節點
 ***************************************************************/
void AddChildNode(WEBNODE * node, char * src)
{
        int WebPort, len;
        char * WebHost = 0, * PageAddress = 0, * WebDir = 0, * pC = 0;
        WEBNODE * NewNode;
        char filename[MAXFILENAME + 1] = "";
        char IsFromRoot = 0;

        if(!src) return;
        if(!strncasecmp(src, "mailto:", strlen("mailto:"))) return ;
        if(strstr(src, ".css")) return;
        if(strstr(src, ".xml")) return;
        if(strstr(src, ".ico")) return;
        if(strstr(src, ".jpg")) return;
        if(strstr(src, ".gif")) return;
        if(strstr(src, "javascript:")) return;
        if(strstr(src, "+")) return;

        ret = GetHost(src, &WebHost, &PageAddress, &WebPort, &WebDir);
        if(ret) {
                len = strlen(node->host);
                GetMemory(&WebHost, len);
                strcpy(WebHost, node->host);

                WebPort = node->port;

                IsFromRoot = !strncmp(src, "/", 1);
                if(IsFromRoot && (src + 1)) Rstrchr(src + 1, '/', &pC);
                else if(!IsFromRoot) Rstrchr(src, '/', &pC);
                else pC = 0;

                if(pC) {
                        if(IsFromRoot) len = strlen(src + 1) - strlen(pC);
                        else len = strlen(src) - strlen(pC) + strlen(node->dir) + 1;
                        GetMemory(&WebDir, len);
                        if(IsFromRoot) memcpy(WebDir, src + 1, len);
                        else {memcpy(WebDir, node->dir, strlen(node->dir)); strcat(WebDir, "/"); memcpy(WebDir + strlen(node->dir) + 1, src, strlen(src) - strlen(pC));}

                        if(pC + 1) {
                                len = strlen(pC + 1);
                                GetMemory(&PageAddress, len);
                                strcpy(PageAddress, pC + 1);
                        }
                        else {
                                len = 1;
                                GetMemory(&PageAddress, len);
                                memcpy(PageAddress, e, len);
                        }
                }
                else {
                        if(IsFromRoot) {
                                len = 1;
                                GetMemory(&WebDir, len);
                                memcpy(WebDir, e + 1, len);

                                len = strlen(src + 1);
                                GetMemory(&PageAddress, len);
                                memcpy(PageAddress, src + 1, len);
                        }
                        else {
                                len = strlen(node->dir);
                                GetMemory(&WebDir, len);
                                memcpy(WebDir, node->dir, len);

                                len = strlen(src);
                                GetMemory(&PageAddress, len);
                                memcpy(PageAddress, src, len);
                        }
                }
        }
        ret = IsExistWeb(NodeHeader, WebHost, PageAddress, WebPort, WebDir);
        if(ret) goto __ReturnAdd;

        if(node->child == NULL) NewNode = node->child = (WEBNODE *)malloc(sizeof(WEBNODE));
        else NodeTail->brother = NewNode = (WEBNODE *)malloc(sizeof(WEBNODE));
        memset(NewNode, 0, sizeof(WEBNODE));
        NewNode->host = (char *)malloc(strlen(WebHost) + 1);
        memset(NewNode->host, 0, strlen(WebHost) + 1);
        NewNode->page = (char *)malloc(strlen(PageAddress) + 1);
        memset(NewNode->page, 0, strlen(PageAddress) + 1);
        NewNode->dir = (char *)malloc(strlen(WebDir) + 1);
        memset(NewNode->dir, 0, strlen(WebDir) + 1);
        NewNode->file = (char *)malloc(MAXFILENAME + 1);
        memset(NewNode->file, 0, MAXFILENAME + 1);
        strcpy(NewNode->host, WebHost);
        strcpy(NewNode->page, PageAddress);
        strcpy(NewNode->dir, WebDir);
        sprintf(filename, "file%05d.html", FileNumber++);
        strcpy(NewNode->file, filename);
        NewNode->port = WebPort;
        NewNode->IsHandled = 0;
        NewNode->brother = 0;
        NewNode->child = 0;
        NodeTail = NewNode;
__ReturnAdd:
        free(WebHost); free(PageAddress); free(WebDir);
}

/**************************************************************
  功能：檢查是否已經處理過的網頁
 ***************************************************************/
int IsExistWeb(WEBNODE * node, char * host, char * page, int port, char * dir)
{
        WEBNODE * t;
        t = node;
        while(t) {
                if(!strcmp(t->host, host) && !strcmp(t->page, page) && t->port == port && !strcmp(t->dir, dir)) return 1;
                t = t->brother;
        }
        t = node;
        while(t) {
                if(t->child) {
                        ret = IsExistWeb(t->child, host, page, port, dir);
                        if(ret) return 2;
                }
                t = t->brother;
        }
        return 0;
}

