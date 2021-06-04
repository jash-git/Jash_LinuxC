//mkdir()創建多級目錄
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int mkdir_r(const char *path)
{
    if (path == NULL)
	{
        return -1;
    }
    char *temp = strdup(path);
    char *pos = temp;

    /* 去掉開頭的 './' 或 '/' */
    if (strncmp(temp, "/", 1) == 0)
	{
		pos += 1;
    }
	else if (strncmp(temp, "./", 2) == 0)
	{
        pos += 2;
    }
    /* 迴圈創建目錄 */
    for ( ; *pos != '\0'; ++ pos)
	{
		if (*pos == '/')
		{
			*pos = '\0';
			mkdir(temp, 0644);
			printf("for %s\n", temp);
			*pos = '/';
		}
	}
	/* 如果最後一級目錄不是以'/'結尾，
		遇到'\0'就中止迴圈，
		不會創建最後一級目錄 
	*/
	if (*(pos - 1) != '/')
	{
		printf("if %s\n", temp);
		mkdir(temp, 0644);
	}
	free(temp);
	return 0;
}

int main()
{
    mkdir_r("./a/b/c/d");
    return 0;
}
