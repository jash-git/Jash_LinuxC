#include <wx/string.h>
#include <wx/utils.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
wxWidgets has several handy utility functions for executing a process, getting a home user directory or getting the OS name.

In the following example, we execute the ls command. For this, we have the wxShell() function (Unix only).
*/
int main(int argc, char **argv)
{

  wxShell(wxT("ls -l"));

}
