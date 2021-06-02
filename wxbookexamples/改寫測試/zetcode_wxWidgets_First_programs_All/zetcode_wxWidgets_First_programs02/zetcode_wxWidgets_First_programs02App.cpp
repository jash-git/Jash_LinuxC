/*
http://zetcode.com/gui/wxwidgets/firstprograms/
Application icon

In this example, we provide an icon for our application. It became a standard to display a small icon in the upper left corner of the window. The icon is a graphical identity of the program.
*/
//--icon.h
#include <wx/wx.h>

class Icon : public wxFrame
{
public:
    Icon(const wxString& title);

};
//-----------
//--icon.cpp
//#include "icon.h"

Icon::Icon(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
  SetIcon(wxIcon(wxT("web.xpm")));
  Centre();
}
//-----------
//--main.h
//#include <wx/wx.h>

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};
//-----------
//--main.cpp
//#include "main.h"
//#include "icon.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    Icon *icon = new Icon(wxT("Icon"));
    icon->Show(true);

    return true;
}
