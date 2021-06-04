//main.c
#include "base64.h"

int main(void)
{
    char *src="jash.liao asd700502";
    char *buf;
    char *dec_buf;
	
    base64_encode(src,&buf);
    printf("base64 encode:%s\n",buf);
	
    base64_decode(buf,&dec_buf);
    printf("base64 decode:%s\n",dec_buf);

    free(buf);
    free(dec_buf);
    return 0;
}
