//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <QControls.hpp>
#include <QStdCtrls.hpp>
#include <QForms.hpp>
#include <QButtons.hpp>

#include <IniFiles.hpp>		//����õ�inf�ļ�������������ļ�
#include <QMenus.hpp>
#include <QTypes.hpp>
#include <QExtCtrls.hpp>

//����ͨѶ����
struct CommPara{
        AnsiString PortName;//��������
        int BaudRate;		//������
        int Parity;         //0��ʾ"��У��",1��ʾ"��У��",2��ʾ"żУ��"
        int StopBits;		//ֹͣλ��
        int DataBits;		//����λ��
        

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
    void __fastcall ReadIniFile(AnsiString FileName);//��inf�ļ�
    void __fastcall WriteIniFile(AnsiString FileName);//дinf�ļ�
    void __fastcall InitComm();		//��ʼ������
    void __fastcall MyStrToDecArray(AnsiString &s,unsigned char *MySendBuffer,int &MyLength);//�ַ���ת��Ϊʮ���Ʋ��ҷ���MySendBuffer������
    void __fastcall MySendArray(int FileHandle,unsigned char *MySendBuffer,unsigned int MyLength);//����ʮ����������
    int  __fastcall GetBaudRate(int BaudRate);	//��ȡ������
    AnsiString __fastcall MyStringTrim(AnsiString MyStr); //�����ո���ַ���ת��Ϊû�пո���ַ���
    
public:
    int FileHandle;	//���崮�ڵ��ļ�������
    CommPara MyCommPara;	//���崮��ͨѶ��������
    AnsiString AppDir;		//�������ǰ������·��
    AnsiString IniFileName;	//����inf�ļ����ļ�����
    bool bShutDownFlag;		//����رճ����ֹͣ�������ݵĲ���������bShutDownFlag=true����˵��Ŀǰ���ڹرճ���״̬��ֹͣ��������״̬
    bool ReceiveFlag;		//����ֹͣ�������ݵĲ���������ReceiveFlag=true����ʼ��������

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
 