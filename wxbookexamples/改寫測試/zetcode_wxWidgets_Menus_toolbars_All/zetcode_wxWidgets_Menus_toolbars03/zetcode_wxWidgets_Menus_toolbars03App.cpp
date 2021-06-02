/*
http://zetcode.com/gui/wxwidgets/menustoolbars/
A simple toolbar

Our first example will create a simple toolbar.
*/
#include "toolbar.h"
#include <wx/wx.h>

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    Toolbar *toolbar = new Toolbar(wxT("Toolbar"));
    toolbar->Show(true);

    return true;
}
/*
In our example, we create a toolbar and one tool button. Clicking on the toolbar button will terminate the application.

wxToolBar *toolbar = CreateToolBar();
We create a toolbar.

toolbar->AddTool(wxID_EXIT, exit, wxT("Exit application"));
We add a tool to the toolbar.

toolbar->Realize();
After we have added the tools, we call the Realize() method.
*/
