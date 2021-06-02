#include <wx/dir.h>
#include <wx/filefn.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
The wxDir class allows us to enumerate files and directories.

In the following example, we will print all files and directories available in the current working directory.
*/
int main(int argc, char **argv)
{

  wxDir dir(wxGetCwd());

  wxString file;

  bool cont = dir.GetFirst(&file, wxEmptyString,
      wxDIR_FILES | wxDIR_DIRS);

  while (cont) {
      wxPuts(file);
      cont = dir.GetNext(&file);
  }
}
