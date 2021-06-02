#include <wx/file.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
wxWidgets has several classes to facilitate working with files. This is low level access to files, as opposed to working with streams.

In the following example, we use the wxFile class to create a new file and write data to it. We also test, whether the file is opened. Note that when we create a file, it automatically stays as opened.
*/
int main(int argc, char **argv)
{

  wxString str = wxT("You make me want to be a better man.\n");

  wxFile file;
  file.Create(wxT("quote"), true);

  if (file.IsOpened())
      wxPuts(wxT("the file is opened"));

  file.Write(str);
  file.Close();

  if (!file.IsOpened())
      wxPuts(wxT("the file is not opened"));
}
