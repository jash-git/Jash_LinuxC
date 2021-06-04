//---------------------------------------------------------------------------

#include <clx.h>
#pragma hdrstop

#include "Unit1.h"
#include <fcntl.h>
#include <termios.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.xfm"
TForm1 *Form1;
unsigned char Buffer[1024];//����������ݻ�����

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    IniFileName="CommTest.inf";	//��ʼ��inf�ļ�����
    FileHandle=-1;	//��ʼ�������ļ�������
    bShutDownFlag=false;
    ReceiveFlag=true;	//ʹ����տ�ʼ����ʱ���ڽ�������״̬
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    AppDir=ExtractFilePath(Application->ExeName);	//�õ������·��
    ReadIniFile(AppDir+IniFileName);				//�ӳ����·���¶�ȡinf�ļ�
	
    ComboBox1->Items->Add("ttyS0");
    ComboBox1->Items->Add("ttyS1");
    ComboBox1->Items->Add("ttyS2");
    ComboBox1->Items->Add("ttyS3");

    ComboBox2->Items->Add("1200");
    ComboBox2->Items->Add("2400");
    ComboBox2->Items->Add("4800");
    ComboBox2->Items->Add("9600");
    ComboBox2->Items->Add("19200");
    ComboBox2->Items->Add("38400");
    ComboBox2->Items->Add("57600");
    ComboBox2->Items->Add("115200");

    ComboBox3->Items->Add("��У��");
    ComboBox3->Items->Add("��У��");
    ComboBox3->Items->Add("żУ��");

    ComboBox4->Items->Add("1");
    ComboBox4->Items->Add("2");

    ComboBox5->Items->Add("7");
    ComboBox5->Items->Add("8");

    int Index=ComboBox1->Items->IndexOf(MyCommPara.PortName);
    ComboBox1->ItemIndex=Index;
    switch(MyCommPara.BaudRate)
    {
    case B1200:
        ComboBox2->ItemIndex=0;
        break;
    case B2400:
        ComboBox2->ItemIndex=1;
        break;
    case B4800:
        ComboBox2->ItemIndex=2;
        break;
    case B9600:
        ComboBox2->ItemIndex=3;
        break;
    case B19200:
        ComboBox2->ItemIndex=4;
        break;
    case B38400:
        ComboBox2->ItemIndex=5;
        break;
    case B57600:
        ComboBox2->ItemIndex=6;
        break;
    case B115200:
        ComboBox2->ItemIndex=7;
        break;


    }
    
    switch(MyCommPara.Parity)
    {
    case 0:
        ComboBox3->ItemIndex=0;
        break;
    case 1:
        ComboBox3->ItemIndex=1;
        break;
    case 2:
        ComboBox3->ItemIndex=2;
        break;
    }
    Index=ComboBox4->Items->IndexOf(AnsiString(MyCommPara.StopBits));
    ComboBox4->ItemIndex=Index;
    Index=ComboBox5->Items->IndexOf(AnsiString(MyCommPara.DataBits));
    ComboBox5->ItemIndex=Index;
    InitComm();	//�򿪴���

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)	//���ںŷ����ı���¼�
{
    MyCommPara.PortName=ComboBox1->Items->Strings[ComboBox1->ItemIndex];
    InitComm();	//�����²����������úʹ򿪴���

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)	//�رճ���
{
    bShutDownFlag=true;
    if(FileHandle!=-1)close(FileHandle);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ReadIniFile(AnsiString FileName)	//��inf�ļ�
{
    IniFileHandle=new TIniFile(FileName);
    try
    {
        try
        {
            MyCommPara.PortName=IniFileHandle->ReadString("CommSet","PortName","ttyS0");//��inf�ļ��ж�ȡ���ں�
            MyCommPara.BaudRate=IniFileHandle->ReadInteger("CommSet","BaudRate",9600);	//��inf�ļ��ж�ȡ������
            MyCommPara.Parity=IniFileHandle->ReadInteger("CommSet","Parity",0);			//��inf�ļ��ж�ȡУ��ģʽ
            MyCommPara.StopBits=IniFileHandle->ReadInteger("CommSet","StopBits",1);		//��inf�ļ��ж�ȡֹͣλ��
            MyCommPara.DataBits=IniFileHandle->ReadInteger("CommSet","DataBits",8);		//��inf�ļ��ж�ȡ����λ��

        }
        catch(...)
        {

        }

    }
    __finally
    {
        if(IniFileHandle!=NULL)delete IniFileHandle;
    }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::WriteIniFile(AnsiString FileName)	//дinf�ļ�
{
    IniFileHandle=new TIniFile(FileName);
    try
    {
        try
        {
            IniFileHandle->WriteString("CommSet","PortName",MyCommPara.PortName);	//��inf�ļ���д�봮�ں�
            IniFileHandle->WriteInteger("CommSet","BaudRate",MyCommPara.BaudRate);	//��inf�ļ���д�벨����
            IniFileHandle->WriteInteger("CommSet","Parity",MyCommPara.Parity);		//��inf�ļ���д��У��ģʽ
            IniFileHandle->WriteInteger("CommSet","StopBits",MyCommPara.StopBits);	//��inf�ļ���д��ֹͣλ��
            IniFileHandle->WriteInteger("CommSet","DataBits",MyCommPara.DataBits);	//��inf�ļ���д������λ��

        }
        catch(...)
        {

        }

    }
    __finally
    {
        if(IniFileHandle!=NULL)delete IniFileHandle;
    }
}

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)	//�رճ����¼�
{
    WriteIniFile(AppDir+IniFileName);	//������·���µ�inf�ļ�д��ͨѶ����
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InitComm()		//��ʼ������
{
    struct termios CommOptions,OldCommOptions;
    AnsiString PortName=MyCommPara.PortName;
    PortName=AnsiString("//dev//")+PortName;
    FileHandle=open(PortName.c_str(),O_RDWR|O_NOCTTY|O_NDELAY);	//�򿪴���
    if(FileHandle==-1)
    {
        ShowMessage("CommPort: "+PortName+" open fail!");
    }
    tcgetattr(FileHandle,&OldCommOptions); //�����ϵĴ�����Ϣ
    tcgetattr(FileHandle,&CommOptions); //��ȡ��ǰ���ڵĲ�����Ϣ
    cfsetispeed(&CommOptions,MyCommPara.BaudRate);
    cfsetospeed(&CommOptions,MyCommPara.BaudRate);    //������������Ϊ9600
    tcsetattr(FileHandle,TCSANOW,&CommOptions);//Ϊ���������²���

    switch(MyCommPara.Parity)
    {
    case 0:     //��У��
        CommOptions.c_cflag&=~PARENB;
        CommOptions.c_cflag&=~CSTOPB;
        CommOptions.c_iflag&=~INPCK;
        break;
    case 1:     //��У��
        CommOptions.c_cflag|=PARENB;
        CommOptions.c_cflag|=PARODD;
        CommOptions.c_cflag&=~CSTOPB;
        CommOptions.c_iflag|=INPCK;

        break;
    case 2:     //żУ��
        CommOptions.c_cflag|=PARENB;
        CommOptions.c_cflag&=~PARODD;
        CommOptions.c_cflag&=~CSTOPB;
        CommOptions.c_iflag|=INPCK;
        break;
    }

    CommOptions.c_cflag&=~CSIZE;	//������λ����λ

    switch(MyCommPara.DataBits)
    {
    
    case 7:     //����λΪ7λ����
        CommOptions.c_cflag|=CS7;
        break;
    case 8:     //����λΪ8λ����
        CommOptions.c_cflag|=CS8;
        break;

    }

}
void __fastcall TForm1::BitBtn2Click(TObject *Sender)	//���Ϳؼ�Edit1�е�����
{
    unsigned char *SendBuffer;
    int StrLength,SendLength;
    String TempStr;
    AnsiString MySendStr;
    if(FileHandle!=-1)
    {
        if(!CheckBox1->Checked)
        {
            write(FileHandle,Edit1->Text.data(),sizeof(Edit1->Text.Length()));
        }
        else
        {
            StrLength=Edit1->Text.Length();
            SendBuffer=new unsigned char[StrLength];
            TempStr=Edit1->Text; //
            try
            {
                MySendStr=MyStringTrim(TempStr);
                MyStrToDecArray(MySendStr,SendBuffer,SendLength);
                MySendArray(FileHandle,SendBuffer,SendLength);
            }
            __finally
            {
                delete [] SendBuffer;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)	//���ڲ����ʷ����ı���¼�
{
    int TempBaud;
    TempBaud=ComboBox2->Items->Strings[ComboBox2->ItemIndex].ToInt();
    MyCommPara.BaudRate=TempBaud;
    GetBaudRate(TempBaud);
    InitComm();	//�����²����������úʹ򿪴���
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox3Change(TObject *Sender)	//����У��ģʽ�����ı���¼�
{
    switch(ComboBox3->ItemIndex)
    {
    case 0:
        MyCommPara.Parity=0;
        break;
    case 1:
        MyCommPara.Parity=1;
        break;
    case 2:
        MyCommPara.Parity=2;
        break;
    }
    InitComm();	//�����²����������úʹ򿪴���
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox5Change(TObject *Sender)	//��������λ�������ı���¼�
{
    MyCommPara.DataBits=ComboBox5->Items->Strings[ComboBox5->ItemIndex].ToInt();
    InitComm();	//�����²����������úʹ򿪴���
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox4Change(TObject *Sender)	//����ֹͣλ�������ı���¼�
{
    MyCommPara.StopBits=ComboBox4->Items->Strings[ComboBox4->ItemIndex].ToInt();
    InitComm();	//�����²����������úʹ򿪴���
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MySendArray(int FileHandle,unsigned char *MySendBuffer,unsigned int MyLength)	//ʮ�����Ʒ��ͺ���
{
    unsigned int i;

    for(i=0;i<MyLength;i++)
    {
        write(FileHandle,&MySendBuffer[i],sizeof(BYTE));
        Application->ProcessMessages();
        if(bShutDownFlag)return;

    }
}
void __fastcall TForm1::MyStrToDecArray(AnsiString &s,unsigned char *MySendBuffer,int &MyLength)	//�ַ���ת��Ϊʮ���Ʋ��ҷ���MySendBuffer������
{
    int MyStringLength;
    int i,j;
    unsigned char BinChar;
    AnsiString Temps,Temps1;

    BinChar=0;
    for(i=1,j=0;i<s.Length();++i,j++)
    {
        Temps=s.SubString(i,2);
        Temps1=Temps.SubString(1,1);
        if(Temps1=="A"||Temps1=="a")BinChar=10*16;
        else
        if(Temps1=="B"||Temps1=="b")BinChar=11*16;
        else
        if(Temps1=="C"||Temps1=="c")BinChar=12*16;
        else
        if(Temps1=="D"||Temps1=="d")BinChar=13*16;
        else
        if(Temps1=="E"||Temps1=="e")BinChar=14*16;
        else
        if(Temps1=="F"||Temps1=="f")BinChar=15*16;
        else
        BinChar=Temps1.ToInt()*16;

        Temps1=Temps.SubString(2,1);
        if(Temps1=="A"||Temps1=="a")BinChar+=10;
        else
        if(Temps1=="B"||Temps1=="b")BinChar+=11;
        else
        if(Temps1=="C"||Temps1=="c")BinChar+=12;
        else
        if(Temps1=="D"||Temps1=="d")BinChar+=13;
        else
        if(Temps1=="E"||Temps1=="e")BinChar+=14;
        else
        if(Temps1=="F"||Temps1=="f")BinChar+=15;
        else
        BinChar+=Temps1.ToInt();

        MySendBuffer[j]=BinChar;
        ++i;
     }
    MyLength=j;
}
int __fastcall TForm1::GetBaudRate(int BaudRate)
{

    switch(BaudRate)
    {
    case 1200:
        MyCommPara.BaudRate=B1200;
        break;
    case 2400:
        MyCommPara.BaudRate=B2400;
        break;
    case 4800:
        MyCommPara.BaudRate=B4800;
        break;
    case 9600:
        MyCommPara.BaudRate=B9600;
        break;
    case 19200:
        MyCommPara.BaudRate=B19200;
        break;
    case 38400:
        MyCommPara.BaudRate=B38400;
        break;
    case 57600:
        MyCommPara.BaudRate=B57600;
        break;
    case 115200:
        MyCommPara.BaudRate=B115200;
        break;


    }
    return BaudRate;
}
AnsiString __fastcall TForm1::MyStringTrim(AnsiString MyStr)
{
    AnsiString TempStr,ReturnStr;
    int Index;
    TempStr=MyStr.Trim();//����ȥ���ַ����Ŀ�ͷ�ͽ�β�Ŀո�
    if(TempStr=="")return ("");
    Index=TempStr.AnsiPos(" ");
    ReturnStr="";
    while(Index)
    {
        ReturnStr+=TempStr.SubString(1,2);
        TempStr.Delete(1,Index);
        TempStr=TempStr.Trim();
        Index=TempStr.AnsiPos(" ");

    }
    if(TempStr!="")
        ReturnStr+=TempStr.SubString(1,2);
    return ReturnStr;
    

}
void __fastcall TForm1::Edit1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)										//�ؼ�Edit1�����м��̰��µ��¼�
{
    if(Key==Key_Return)
    {
        BitBtn2Click(Sender);   
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)	//��ʱ���¼�����ȡ���ڵ�����
{
    //
    int ReadByte=0;
    if(ReceiveFlag==false)return;
    ReadByte=read(FileHandle,Buffer,1024);
    if(ReadByte)
    {
        for(int i=0;i<ReadByte;i++)
        {
            if(CheckBox2->Checked)
                Memo1->Text=Memo1->Text+AnsiString(IntToHex(Buffer[i],2))+" ";//��ʾ��Memo1��
            else
                Memo1->Text=Memo1->Text+AnsiString(char(Buffer[i]))+" ";

        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)	//��Memo1���
{
    Memo1->Clear();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)	//���տ�ʼ��ֹͣ����
{
    if(ReceiveFlag)
    {
        ReceiveFlag=false;
        BitBtn4->Caption="��ʼ����";
    }
    else
    {
        ReceiveFlag=true;
        BitBtn4->Caption="ֹͣ����";
    }

}
//---------------------------------------------------------------------------
