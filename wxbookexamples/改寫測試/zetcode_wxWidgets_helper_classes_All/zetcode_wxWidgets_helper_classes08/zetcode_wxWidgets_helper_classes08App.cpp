#include <wx/string.h>
#include <wx/utils.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
Next we will we will get the home user directory, os name, user name, host name, and total free memory.
*/
int main(int argc, char **argv)
{
  wxPuts(wxGetHomeDir());
  wxPuts(wxGetOsDescription());
  wxPuts(wxGetUserName());
  wxPuts(wxGetFullHostName());

  long mem = wxGetFreeMemory().ToLong();

  wxPrintf(wxT("Memory: %ld\n"), mem);
}
