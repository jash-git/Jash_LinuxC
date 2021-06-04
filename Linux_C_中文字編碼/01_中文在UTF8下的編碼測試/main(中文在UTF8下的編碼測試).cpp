#include <iostream>
#include <bitset>
#include <cstdio>
using namespace std;
int main(){
    string s="林忠億";
    char a[s.length()+1];
    for(int i=0;i<s.length();i++){
        a[i]=s[i];
    }
    a[s.length()]='\0';
    cout<<a<<endl;
    for(int i=0;i<s.length();i++){
        printf("%x\t",a[i]);
        bitset<16> x=a[i];
        cout<<x<<endl;
    }
}
