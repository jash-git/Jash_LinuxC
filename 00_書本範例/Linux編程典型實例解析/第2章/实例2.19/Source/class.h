#include <iostream.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
class TObject
{};
class TControl :public TObject
{
public:
   int Height;
   int Width;
   int Left;
   int Top;
   int TabOrder;
   int TabCount;
   char* Text;
   char* Caption;
   int CurrentFocus;
};
class TWinControl :public TControl
{
public:
   void RefreshFocus(WINDOW *parent,int CurrentFocus);
};
void TWinControl::RefreshFocus(WINDOW *parent,int CurrentFocus)
{
   if(TabOrder==CurrentFocus)
      wmove(parent,Top,Left);
}
//-------------Edit----------------------------
class TEdit :public TWinControl
{
public:
   void   Show(WINDOW *parent,int CurrentFocus);
};
void TEdit::Show(WINDOW *parent,int CurrentFocus)
{
   mvwaddstr(parent,Top,Left,Text);
};
//-------------Button----------------------------
class TButton :public TWinControl
{
public:
   void  Show(WINDOW *parent);
};
void TButton::Show(WINDOW *parent)
{
   mvwaddstr(parent,Top,Left,Caption);
}
//--------------Form-----------------------
class TForm :public TWinControl
{
public:
   WINDOW *Window;
   TEdit *EditFlag[30];
   TButton *ButtonFlag[30];
   bool OnKeyDownEnable[100];
   void (TObject::*OnKeyDown[100])(int key);
   void Show();
   bool MakeWindow;
   void Close();
   bool ExitFlag;
};
void TForm::Show()
{
   int ch;
   ExitFlag=FALSE;
   CurrentFocus=1;
   if(MakeWindow==TRUE)
   {
      Window=newwin(Height,Width,Top,Left);
      keypad(Window,TRUE);
      touchwin(Window);
      MakeWindow=FALSE;
   }
   while(1)
   {
      box(Window,'|','-');
      int flag=0;
      while(1)
      {
         if(EditFlag[flag]==NULL)
         {
            flag=0;
            break;
         }
         EditFlag[flag]->Show(Window,CurrentFocus);
         flag++;
      }
      while(1)
      {
         if(ButtonFlag[flag]==NULL)
         {
            flag=0;
            break;
         }
         ButtonFlag[flag]->Show(Window);
         flag++;
      }
      {
         while(1)
         {
            if(EditFlag[flag]==NULL)
            {
               flag=0;
               break;
            }
            EditFlag[flag]->RefreshFocus(Window,CurrentFocus);
            flag++;
         }
         while(1)
         {
            if(ButtonFlag[flag]==NULL)
            {
               flag=0;
               break;
            }
            ButtonFlag[flag]->RefreshFocus(Window,CurrentFocus);
            flag++;
         }
      }
      wrefresh(Window);
      ch=wgetch(Window);
      if(OnKeyDownEnable[CurrentFocus]==TRUE)
         (this->*OnKeyDown[CurrentFocus])(ch);
      if(ch!=27&&ExitFlag==FALSE)
      {
         if(ch=='\t'||ch==KEY_DOWN)
         {
            CurrentFocus++;
            if (CurrentFocus==TabCount+1)
               CurrentFocus=1;
         }
         if(ch==KEY_UP)
         {
            CurrentFocus--;
            if(CurrentFocus==0)
               CurrentFocus=TabCount;
         }
      }
      else
      {
         break;
      }
   }
}
void TForm::Close()
{
   delwin(Window);
   ExitFlag=TRUE;
}
