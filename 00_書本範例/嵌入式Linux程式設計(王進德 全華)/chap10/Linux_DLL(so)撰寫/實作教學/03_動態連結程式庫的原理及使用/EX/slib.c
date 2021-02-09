/*
函數dlopen的第二個參數有兩種選擇：

■RTLD_LAZY：推遲解析DLL中的外部引用，直到DLL被執行

■RTLD_NOW：在返回之前解析所有的外部引用

以下是DLL檔源碼slib.c：
*/
int func(char* msg)

{

    printf("func be Executed!\n");

    printf(msg);

    return 0;

}
