#include "custom.h"
int main(int argc, char* argv[])
{
   initscr();
   touchwin(stdscr);
   wrefresh(stdscr);
   Form1->Height=25;
   Form1->Width=80;
   Form1->Top=0;
   Form1->Left=0;
   Form1->Edit1->Text="Edit1";
   Form1->Edit1->Left=20;
   Form1->Edit1->Top=6;
   Form1->Edit2->Text="Edit2";
   Form1->Edit2->Left=10;
   Form1->Edit2->Top=6;
   Form1->Button1->Caption="Button1";
   Form1->Button1->Left=18;
   Form1->Button1->Top=7;
   Form1->Button2->Caption="Button2";
   Form1->Button2->Left=21;
   Form1->Button2->Top=2;
   Form2->Height=14;
   Form2->Width=40;
   Form2->Top=5;
   Form2->Left=10;
   Form2->Edit1->TabOrder=1;
   Form2->Edit1->Text="Edit1";
   Form2->Edit1->Left=10;
   Form2->Edit1->Top=6;
   Form2->Button1->Caption="Button1";
   Form2->Button1->Left=18;
   Form2->Button1->Top=9;
   Form1->TabCount=4;
   Form2->TabCount=2;
   while(1)
   {
      Form1->Show();
   }
   return 0;
}
void TForm1::Button1KeyDown(int key)
{
   if(key=='\n')
      Form2->Show();
}
void TForm2::Button1KeyDown(int key)
{
   Close();
}

