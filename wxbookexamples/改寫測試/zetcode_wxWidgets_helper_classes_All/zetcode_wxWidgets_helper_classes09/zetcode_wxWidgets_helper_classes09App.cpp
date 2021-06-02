#include <wx/datetime.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
In wxWidgets, we have several classes for working with date & time.

The example shows current date or time in various formats.
*/
int main(int argc, char **argv)
{
  wxDateTime now = wxDateTime::Now();

  wxString date1 = now.Format();
  wxString date2 = now.Format(wxT("%X"));
  wxString date3 = now.Format(wxT("%x"));

  wxPuts(date1);
  wxPuts(date2);
  wxPuts(date3);
}
