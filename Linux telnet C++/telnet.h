// Definition of the Telnet class

#ifndef Telnet_class
#define Telnet_class

#include "Socket.h"

enum _verb
{
  verb_sb   = 250,
  verb_will = 251,
  verb_wont = 252,
  verb_do   = 253, 
  verb_dont = 254
};

enum _state
{
  state_data,   //we expect a data byte
  state_code,   //we expect a code
  state_option  //we expect an option
};
enum _option
{
  TOPT_BIN = 0,   // Binary Transmission
  TOPT_ECHO = 1,  // Echo
  TOPT_RECN = 2,  // Reconnection
  TOPT_SUPP = 3,  // Suppress Go Ahead
  TOPT_APRX = 4,  // Approx Message Size Negotiation
  TOPT_STAT = 5,  // Status
  TOPT_TIM = 6,   // Timing Mark
  TOPT_REM = 7,   // Remote Controlled Trans and Echo
  TOPT_OLW = 8,   // Output Line Width
  TOPT_OPS = 9,   // Output Page Size
  TOPT_OCRD = 10, // Output Carriage-Return Disposition
  TOPT_OHT = 11,  // Output Horizontal Tabstops
  TOPT_OHTD = 12, // Output Horizontal Tab Disposition
  TOPT_OFD = 13,  // Output Formfeed Disposition
  TOPT_OVT = 14,  // Output Vertical Tabstops
  TOPT_OVTD = 15, // Output Vertical Tab Disposition
  TOPT_OLD = 16,  // Output Linefeed Disposition
  TOPT_EXT = 17,  // Extended ASCII
  TOPT_LOGO = 18, // Logout
  TOPT_BYTE = 19, // Byte Macro
  TOPT_DATA = 20, // Data Entry Terminal
  TOPT_SUP = 21,  // SUPDUP
  TOPT_SUPO = 22, // SUPDUP Output
  TOPT_SNDL = 23, // Send Location
  TOPT_TERM = 24, // Terminal Type
  TOPT_EOR = 25,  // End of Record
  TOPT_TACACS = 26, // TACACS User Identification
  TOPT_OM = 27,   // Output Marking
  TOPT_TLN = 28,  // Terminal Location Number
  TOPT_3270 = 29, // Telnet 3270 Regime
  TOPT_X3 = 30,  // X.3 PAD
  TOPT_NAWS = 31, // Negotiate About Window Size
  TOPT_TS = 32,   // Terminal Speed
  TOPT_RFC = 33,  // Remote Flow Control
  TOPT_LINE = 34, // Linemode
  TOPT_XDL = 35,  // X Display Location
  TOPT_ENVIR = 36,// Telnet Environment Option
  TOPT_AUTH = 37, // Telnet Authentication Option
  TOPT_NENVIR = 39,// Telnet Environment Option
  TOPT_EXTOP = 255, // Extended-Options-List
  TOPT_ERROR = 256  // Magic number
};

void ansi_set_screen_attribute(char* buffer);

class Telnet : private Socket
{
  public:
	Telnet ( std::string host, int port );
	~Telnet(){};

	const Telnet& operator << ( const std::string& ) const;
	const Telnet& operator >> ( std::string& ) const;
	int	SendCmd(const std::string&, std::string&);
	int	SendCmd(const std::string&);
	int	RecvReply(std::string&);
	int Connect(void);
	const int TelentProtcol(unsigned char code) const;
	void Disconnect(void);

	// protocol function
	void yesreply(_verb verb,_option option) const;
	void noreply(_verb verb,_option option) const;
	void askfor(_verb verb,_option option) const;
	void termreply(unsigned char data) const;

  private:
/*
	typedef void(*LPCODEPROC) (char*);
	typedef void(*LPOPTIONPROC) (_verb,_option);
	typedef int(*LPDATAPROC) (unsigned char data);
	
	struct ANSI_CODE 
	{
	  unsigned char cmd;
	  LPCODEPROC proc;
	} codec[NUM_CODEC];
*/
	string	sHost;
	int		iPort;
  	int	iRetryTime;
  	std::string sUserName;
  	std::string sPassWord;

};


#endif
