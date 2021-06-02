#include <wx/string.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
Console
The following example checks, whether a string contains another string. For this we have a Contains() method.
*/
int main(int argc, char **argv)
{

  wxString str = wxT("The history of my life");

  if (str.Contains(wxT("history"))) {
      wxPuts(wxT("Contains!"));
  }


  if (!str.Contains(wxT("plain"))) {
      wxPuts(wxT("Does not contain!"));
  }

}
