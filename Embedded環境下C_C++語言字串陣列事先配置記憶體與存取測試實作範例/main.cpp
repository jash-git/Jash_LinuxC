#include <iostream>

using namespace std;
char *Embedded_C_String_Arry[]
{
  "Embedded",//0
  "���ҤU",
  "C/C++�y��",
  "�r��}�C",
  "�ƥ��t�m�O����",
  "�P�s������",//5
  "��@�d��",
};
int String_Arry_count=7;//��ʶ�g
int main()
{
    int i;
    char **get_C_String_Arry;
    get_C_String_Arry=(char **)Embedded_C_String_Arry;

    for(i=0;i<String_Arry_count;i++)
    {
        cout << get_C_String_Arry[i];
    }
    cout << endl;
    return 0;
}
