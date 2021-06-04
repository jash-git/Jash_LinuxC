//mkdir()�Ыئh�ťؿ�
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

    /* �h���}�Y�� './' �� '/' */
    if (strncmp(temp, "/", 1) == 0)
	{
		pos += 1;
    }
	else if (strncmp(temp, "./", 2) == 0)
	{
        pos += 2;
    }
    /* �j��Ыإؿ� */
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
	/* �p�G�̫�@�ťؿ����O�H'/'�����A
		�J��'\0'�N����j��A
		���|�Ыس̫�@�ťؿ� 
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
