#include <wx/string.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
wxString is a class representing a character string.

In the following example, we define three wxStrings. We create one string of these strings using addition operation.
*/
int main(int argc, char **argv)
{
  wxString str1 = wxT("Linux");
  wxString str2 = wxT("Operating");
  wxString str3 = wxT("System");

  wxString str = str1 + wxT(" ") + str2 + wxT(" ") + str3;

  wxPuts(str);
}
