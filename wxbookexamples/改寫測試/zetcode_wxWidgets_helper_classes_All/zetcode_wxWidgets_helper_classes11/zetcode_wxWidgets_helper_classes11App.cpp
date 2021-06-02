#include <wx/datetime.h>
/*
http://zetcode.com/gui/wxwidgets/helperclasses/
The following example shows, how we can add date spans to our date/time. We add one month to the current time.
*/
int main(int argc, char **argv)
{
  wxDateTime now = wxDateTime::Now();
  wxString date1 = now.Format(wxT("%B %d %Y"));
  wxPuts(date1);

  wxDateSpan span(0, 1);
  wxDateTime then = now.Add(span);

  wxString date2 = then.Format(wxT("%B %d %Y"));
  wxPuts(date2);

}
