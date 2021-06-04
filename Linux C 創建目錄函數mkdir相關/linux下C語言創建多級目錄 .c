//linux下C語言創建多級目錄 
//資料來源：http://sharp2wing.iteye.com/blog/1280802
int CreateDir(const char *sPathName)  
{  
	char	DirName[256];  
	strcpy(DirName,   sPathName);  
	int	i;
	int len=strlen(DirName);  
	if(DirName[len-1]!='/')
		strcat(DirName,"/");     
	
	len=strlen(DirName);  
	for(i=1;   i<len;   i++)  
	{  
		if(DirName[i]=='/')  
		{  
			DirName[i]   =   0;  
			if(access(DirName,NULL)!=0)  
			{  
				if(mkdir(DirName,0755)==-1)  
				{   
					perror("mkdir   error");   
					return   -1;   
				}  
			}  
			DirName[i]   =   '/';  
		}  
	}  
   
	return   0;  
} 
