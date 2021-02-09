/**
 * Filename: mydlopen.c 
 * Author: jash.liao
 */
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

void show_help(char *msg){
        if(msg == NULL){
                printf("Usage:mydlopen dlopen.so say stringtosay\n");
        }else{
                printf("%s\n",msg);
        }
        exit(1);
}
int main(int ac,char ** av){

        if(ac < 3){ 
                show_help(NULL);
        }
        void *handle;
        //void *pfunc;


        int (*pfunc)(char *str);
        char * filename = av[1];
        char * func = av[2];
        char * word = av[3];
        char * error;
        handle = dlopen(filename,RTLD_LAZY);
        if(!handle){
                printf("Error: handle\n");
                return 1;
        }

        pfunc = (int (*)(char *))dlsym(handle,func);
        if((error=dlerror()) != NULL){
                printf("Error: dlsym\n");
                return 2;
        }
        (*pfunc)(word);

        dlclose(handle);
        return 0;

}

