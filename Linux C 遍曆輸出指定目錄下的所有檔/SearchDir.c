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
    DIR *d; //�n���@�ӱ���X
    struct dirent *file; //readdir��ƪ���^�ȴN�s��b�o�ӵ��c�餤
    struct stat sb;    
    
    if(!(d = opendir(path)))
    {
        printf("error opendir %s!!!\n",path);
        return -1;
    }
    while((file = readdir(d)) != NULL)
    {
        //���e�ؿ�.�A�W�@�ťؿ�..�������ɳ��h���A�קK�����M��ؿ�
        if(strncmp(file->d_name, ".", 1) == 0)
            continue;
        strcpy(filename[len++], file->d_name); //�O�s�M��쪺�ɮצW
        //�P�_���ɬO�_�O�ؿ��A�άO�_�w�j���F�T�h�A�o�اکw�q�u�j���F�T�h�ؿ��A�Ӳ`�N���j�F�A�ٱo�j�X�Ӧh���
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
