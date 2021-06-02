#include <wx/string.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
The Len() method returns the number of characters in the string.
*/
int main(int argc, char **argv)
{
  wxString str = wxT("The history of my life");
  wxPrintf(wxT("The string has %d characters\n"), str.Len());
}
