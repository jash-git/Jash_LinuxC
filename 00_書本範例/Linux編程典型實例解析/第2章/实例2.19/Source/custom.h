#include "class.h"
class TForm1 :public TForm
{
public:
   TEdit* Edit1;
   TEdit* Edit2;
   TButton* Button1;
   TButton* Button2;
   void Button1KeyDown(int key);
   TForm1();
};
TForm1::TForm1()
{
   MakeWindow=TRUE;
   Edit1=new TEdit;
   Edit2=new TEdit;
   EditFlag[0]=Edit1;
   EditFlag[1]=Edit2;
   EditFlag[2]=NULL;
   Edit1->TabOrder=1;
   Edit2->TabOrder=2;
   Button1=new TButton;
   Button2=new TButton;
   ButtonFlag[0]=Button1;
   ButtonFlag[1]=Button2;
   ButtonFlag[2]=NULL;
   Button1->TabOrder=3;
   Button2->TabOrder=4;
   OnKeyDown[Button1->TabOrder]=(void (TObject::*)(int key))&TForm1::Button1KeyDown;
   OnKeyDownEnable[Button1->TabOrder]=TRUE;
}
extern TForm1 *Form1=new TForm1();
//---------------------------------------------------------------------------
class TForm2 :public TForm
{
public:
   TEdit* Edit1;
   TButton* Button1;
   void  Button1KeyDown(int key);
   TForm2();
};
TForm2::TForm2()
{
   MakeWindow=TRUE;
   Edit1=new TEdit;
   EditFlag[0]=Edit1;
   EditFlag[1]=NULL;
   Edit1->TabOrder=1;
   Button1=new TButton;
   ButtonFlag[0]=Button1;
   ButtonFlag[1]=NULL;
   Button1->TabOrder=2;
   OnKeyDown[Button1->TabOrder]=(void (TObject::*)(int key))&TForm2::Button1KeyDown;
   OnKeyDownEnable[Button1->TabOrder]=TRUE;
}
extern TForm2 *Form2=new TForm2();
