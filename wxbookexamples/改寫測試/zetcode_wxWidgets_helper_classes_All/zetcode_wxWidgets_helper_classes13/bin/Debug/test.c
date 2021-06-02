#include <wx/textfile.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
The wxTextFile is a simple class which allows to work with text files on line by line basis. It is easier to work with this class than with wxFile class.

In the next example, we will print the number of lines in a file, first and last lines and finally we will read and show the contents of the file.
*/
int main(int argc, char **argv)
{

  wxTextFile file(wxT("test.c"));

  file.Open();

  wxPrintf(wxT("Number of lines: %d\n"), file.GetLineCount());
  wxPrintf(wxT("First line: %s\n"), file.GetFirstLine().c_str());
  wxPrintf(wxT("Last line: %s\n"), file.GetLastLine().c_str());

  wxPuts(wxT("-------------------------------------"));

  wxString s;

  for ( s = file.GetFirstLine(); !file.Eof();
      s = file.GetNextLine() )
  {
       wxPuts(s);
  }

  file.Close();
}
