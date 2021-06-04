/**********************************************SearchDir.c*****************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
char filename[256][256];
int len = 0;
int trave_dir(char* path, int depth)
{
    DIR *d; //聲明一個控制碼
    struct dirent *file; //readdir函數的返回值就存放在這個結構體中
    struct stat sb;    
    
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    while((file = readdir(d)) != NULL)
    {
        //把當前目錄.，上一級目錄..及隱藏檔都去掉，避免閉環遍曆目錄
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        strcpy(filename[len++], file->d_name); //保存遍曆到的檔案名
        //判斷該檔是否是目錄，及是否已搜索了三層，這裏我定義只搜索了三層目錄，太深就不搜了，省得搜出太多文件
        if(stat(file->d_name, &sb) >= 0 && S_ISDIR(sb.st_mode) && depth <= 3)
        {
            trave_dir(file->d_name, depth + 1);
        }
    }
    closedir(d);
    return 0;
}
int main()
{
    int depth = 1;
    int i;
    trave_dir("/usr/keygoe/ini/", depth);
    for(i = 0; i < len; i++)
    {
        printf("%s\t", filename[i]);
    }
    printf("\n");
    return 0;
}
