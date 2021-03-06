/*
動態連結程式庫的原理及使用 
2007-09-12 08:51:28 
一.動態連結程式庫的原理及使用
大家對Windows作業系統中的DLL檔一定十分熟悉，其實這種軟體元件化的方法在Linux中也可以實現。其實插件和 DLL 通常是用來無須編寫整個新應用程式而添加功能的極好方法，一般來講，在不更改原有應用程式的情況下，插件為現有應用程式提供新功能。Linux環境下甚至做的更好。

Linux提供4個庫函數、一個頭檔dlfcn.h以及兩個共用庫（靜態庫libdl.a和動態庫libdl.so）支援動態鏈結。

■dlopen：打開動態共用目標檔並將其映射到記憶體中，返回其首位址

■dlsym：返回鎖請求的入口點的指針

■dlerror：返回NULL或者指向描述最近錯誤的字串

■dlclose：關閉動態共用檔

函數dlopen需要在檔系統中查找目標檔並為之創建控制碼。有四種方法指定目標檔的位置：

■絕對路徑

■在環境變數LD_LIBRARY_PATH指定的目錄中

■在/etc/ld.so.cache中指定的庫列表中

■在/usr/lib或者/lib中

下面舉一個例子。

主程序文件hello.c：

*/
#include <stdio.h>

#include <dlfcn.h>

 

void* slib=0;

void (*func)(char*);

const char* hError;



int main(int argc,char* argv[])

{

    slib=dlopen("./slib.so",RTLD_LAZY);

    hError=dlerror();

    if (hError)

    {

        printf("dlopen Error!\n");

        return 1;

    }

    func=dlsym(slib,"func");

    hError=dlerror();

    if (hError)

{

dlclose(slib);

        printf("dlsym Error!\n");

        return 1;

    }

    func("How do you do?\n");

    dlclose(slib);

    hError=dlerror();

    if (hError)

    {

        printf("dlclose Error!\n");

        return 1;

    }

    return 0;

}
