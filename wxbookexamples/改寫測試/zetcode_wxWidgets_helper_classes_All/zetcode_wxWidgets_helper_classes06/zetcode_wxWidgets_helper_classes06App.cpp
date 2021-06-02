#include <wx/string.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
The MakeLower() and MakeUpper() methods make characters lower case and upper case.
*/
int main(int argc, char **argv)
{
  wxString str = wxT("The history of my life");

  wxPuts(str.MakeLower());
  wxPuts(str.MakeUpper());
}
