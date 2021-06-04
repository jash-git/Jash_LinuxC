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
unsigned char Buffer[1024];//定义接收数据缓冲区

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    IniFileName="CommTest.inf";	//初始化inf文件名称
    FileHandle=-1;	//初始化串口文件描述符
    bShutDownFlag=false;
    ReceiveFlag=true;	//使程序刚开始运行时处于接收数据状态
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    AppDir=ExtractFilePath(Application->ExeName);	//得到程序的路径
    ReadIniFile(AppDir+IniFileName);				//从程序的路径下读取inf文件
	
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

    ComboBox3->Items->Add("无校验");
    ComboBox3->Items->Add("奇校验");
    ComboBox3->Items->Add("偶校验");

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
    InitComm();	//打开串口

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)	//串口号发生改变的事件
{
    MyCommPara.PortName=ComboBox1->Items->Strings[ComboBox1->ItemIndex];
    InitComm();	//根据新参数重新设置和打开串口

}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn1Click(TObject *Sender)	//关闭程序
{
    bShutDownFlag=true;
    if(FileHandle!=-1)close(FileHandle);

}
//---------------------------------------------------------------------------
void __fastcall TForm1::ReadIniFile(AnsiString FileName)	//读inf文件
{
    IniFileHandle=new TIniFile(FileName);
    try
    {
        try
        {
            MyCommPara.PortName=IniFileHandle->ReadString("CommSet","PortName","ttyS0");//从inf文件中读取串口号
            MyCommPara.BaudRate=IniFileHandle->ReadInteger("CommSet","BaudRate",9600);	//从inf文件中读取波特率
            MyCommPara.Parity=IniFileHandle->ReadInteger("CommSet","Parity",0);			//从inf文件中读取校验模式
            MyCommPara.StopBits=IniFileHandle->ReadInteger("CommSet","StopBits",1);		//从inf文件中读取停止位数
            MyCommPara.DataBits=IniFileHandle->ReadInteger("CommSet","DataBits",8);		//从inf文件中读取数据位数

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
void __fastcall TForm1::WriteIniFile(AnsiString FileName)	//写inf文件
{
    IniFileHandle=new TIniFile(FileName);
    try
    {
        try
        {
            IniFileHandle->WriteString("CommSet","PortName",MyCommPara.PortName);	//向inf文件中写入串口号
            IniFileHandle->WriteInteger("CommSet","BaudRate",MyCommPara.BaudRate);	//向inf文件中写入波特率
            IniFileHandle->WriteInteger("CommSet","Parity",MyCommPara.Parity);		//向inf文件中写入校验模式
            IniFileHandle->WriteInteger("CommSet","StopBits",MyCommPara.StopBits);	//向inf文件中写入停止位数
            IniFileHandle->WriteInteger("CommSet","DataBits",MyCommPara.DataBits);	//向inf文件中写入数据位数

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

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)	//关闭程序事件
{
    WriteIniFile(AppDir+IniFileName);	//向程序的路径下的inf文件写入通讯参数
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InitComm()		//初始化串口
{
    struct termios CommOptions,OldCommOptions;
    AnsiString PortName=MyCommPara.PortName;
    PortName=AnsiString("//dev//")+PortName;
    FileHandle=open(PortName.c_str(),O_RDWR|O_NOCTTY|O_NDELAY);	//打开串口
    if(FileHandle==-1)
    {
        ShowMessage("CommPort: "+PortName+" open fail!");
    }
    tcgetattr(FileHandle,&OldCommOptions); //保存老的串口信息
    tcgetattr(FileHandle,&CommOptions); //获取当前串口的参数信息
    cfsetispeed(&CommOptions,MyCommPara.BaudRate);
    cfsetospeed(&CommOptions,MyCommPara.BaudRate);    //将波特率设置为9600
    tcsetattr(FileHandle,TCSANOW,&CommOptions);//为串口设置新参数

    switch(MyCommPara.Parity)
    {
    case 0:     //无校验
        CommOptions.c_cflag&=~PARENB;
        CommOptions.c_cflag&=~CSTOPB;
        CommOptions.c_iflag&=~INPCK;
        break;
    case 1:     //奇校验
        CommOptions.c_cflag|=PARENB;
        CommOptions.c_cflag|=PARODD;
        CommOptions.c_cflag&=~CSTOPB;
        CommOptions.c_iflag|=INPCK;

        break;
    case 2:     //偶校验
        CommOptions.c_cflag|=PARENB;
        CommOptions.c_cflag&=~PARODD;
        CommOptions.c_cflag&=~CSTOPB;
        CommOptions.c_iflag|=INPCK;
        break;
    }

    CommOptions.c_cflag&=~CSIZE;	//将数据位数复位

    switch(MyCommPara.DataBits)
    {
    
    case 7:     //数据位为7位数据
        CommOptions.c_cflag|=CS7;
        break;
    case 8:     //数据位为8位数据
        CommOptions.c_cflag|=CS8;
        break;

    }

}
void __fastcall TForm1::BitBtn2Click(TObject *Sender)	//发送控件Edit1中的数据
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

void __fastcall TForm1::ComboBox2Change(TObject *Sender)	//串口波特率发生改变的事件
{
    int TempBaud;
    TempBaud=ComboBox2->Items->Strings[ComboBox2->ItemIndex].ToInt();
    MyCommPara.BaudRate=TempBaud;
    GetBaudRate(TempBaud);
    InitComm();	//根据新参数重新设置和打开串口
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox3Change(TObject *Sender)	//串口校验模式发生改变的事件
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
    InitComm();	//根据新参数重新设置和打开串口
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox5Change(TObject *Sender)	//串口数据位数发生改变的事件
{
    MyCommPara.DataBits=ComboBox5->Items->Strings[ComboBox5->ItemIndex].ToInt();
    InitComm();	//根据新参数重新设置和打开串口
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox4Change(TObject *Sender)	//串口停止位数发生改变的事件
{
    MyCommPara.StopBits=ComboBox4->Items->Strings[ComboBox4->ItemIndex].ToInt();
    InitComm();	//根据新参数重新设置和打开串口
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MySendArray(int FileHandle,unsigned char *MySendBuffer,unsigned int MyLength)	//十六进制发送函数
{
    unsigned int i;

    for(i=0;i<MyLength;i++)
    {
        write(FileHandle,&MySendBuffer[i],sizeof(BYTE));
        Application->ProcessMessages();
        if(bShutDownFlag)return;

    }
}
void __fastcall TForm1::MyStrToDecArray(AnsiString &s,unsigned char *MySendBuffer,int &MyLength)	//字符串转换为十进制并且放入MySendBuffer数组中
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
    TempStr=MyStr.Trim();//首先去掉字符串的开头和结尾的空格
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
      TShiftState Shift)										//控件Edit1发生有键盘按下的事件
{
    if(Key==Key_Return)
    {
        BitBtn2Click(Sender);   
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Timer1Timer(TObject *Sender)	//定时器事件，读取串口的数据
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
                Memo1->Text=Memo1->Text+AnsiString(IntToHex(Buffer[i],2))+" ";//显示在Memo1中
            else
                Memo1->Text=Memo1->Text+AnsiString(char(Buffer[i]))+" ";

        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)	//将Memo1清空
{
    Memo1->Clear();    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)	//接收开始和停止控制
{
    if(ReceiveFlag)
    {
        ReceiveFlag=false;
        BitBtn4->Caption="开始接收";
    }
    else
    {
        ReceiveFlag=true;
        BitBtn4->Caption="停止接收";
    }

}
//---------------------------------------------------------------------------
