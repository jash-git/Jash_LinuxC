//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <QControls.hpp>
#include <QStdCtrls.hpp>
#include <QForms.hpp>
#include <QButtons.hpp>

#include <IniFiles.hpp>		//如果用到inf文件，必须包含此文件
#include <QMenus.hpp>
#include <QTypes.hpp>
#include <QExtCtrls.hpp>

//定义通讯参数
struct CommPara{
        AnsiString PortName;//串口名称
        int BaudRate;		//波特率
        int Parity;         //0表示"无校验",1表示"奇校验",2表示"偶校验"
        int StopBits;		//停止位数
        int DataBits;		//数据位数
        

        };
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TLabel *Label1;
    TLabel *Label2;
    TComboBox *ComboBox1;
    TComboBox *ComboBox2;
    TLabel *Label3;
    TComboBox *ComboBox3;
    TLabel *Label4;
    TComboBox *ComboBox4;
    TLabel *Label5;
    TComboBox *ComboBox5;
    TGroupBox *GroupBox2;
    TMemo *Memo1;
    TGroupBox *GroupBox3;
    TBitBtn *BitBtn1;
    TEdit *Edit1;
    TCheckBox *CheckBox1;
    TBitBtn *BitBtn2;
    TTimer *Timer1;
    TBitBtn *BitBtn3;
    TBitBtn *BitBtn4;
    TCheckBox *CheckBox2;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall ComboBox1Change(TObject *Sender);
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall BitBtn2Click(TObject *Sender);
    void __fastcall ComboBox2Change(TObject *Sender);
    void __fastcall ComboBox3Change(TObject *Sender);
    void __fastcall ComboBox5Change(TObject *Sender);
    void __fastcall ComboBox4Change(TObject *Sender);
    void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall BitBtn3Click(TObject *Sender);
    void __fastcall BitBtn4Click(TObject *Sender);
private:	// User declarations
    TIniFile *IniFileHandle;
public:		// User declarations
    __fastcall TForm1(TComponent* Owner);
public:
    void __fastcall ReadIniFile(AnsiString FileName);//读inf文件
    void __fastcall WriteIniFile(AnsiString FileName);//写inf文件
    void __fastcall InitComm();		//初始化串口
    void __fastcall MyStrToDecArray(AnsiString &s,unsigned char *MySendBuffer,int &MyLength);//字符串转换为十进制并且放入MySendBuffer数组中
    void __fastcall MySendArray(int FileHandle,unsigned char *MySendBuffer,unsigned int MyLength);//发送十六进制数据
    int  __fastcall GetBaudRate(int BaudRate);	//获取波特率
    AnsiString __fastcall MyStringTrim(AnsiString MyStr); //将带空格的字符串转换为没有空格的字符串
    
public:
    int FileHandle;	//定义串口的文件描述符
    CommPara MyCommPara;	//定义串口通讯参数变量
    AnsiString AppDir;		//定义程序当前所处的路径
    AnsiString IniFileName;	//定义inf文件的文件名称
    bool bShutDownFlag;		//定义关闭程序或停止发送数据的布尔变量，bShutDownFlag=true，则说明目前处于关闭程序状态或停止发送数据状态
    bool ReceiveFlag;		//定义停止接收数据的布尔变量，ReceiveFlag=true，则开始接收数据

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 