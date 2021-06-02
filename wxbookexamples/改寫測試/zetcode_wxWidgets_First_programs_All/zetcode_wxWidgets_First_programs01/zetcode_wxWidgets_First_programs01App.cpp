/*
http://zetcode.com/gui/wxwidgets/firstprograms/
First we create the very basic wxWidgets program.
*/
//--simple.h
#include <wx/wx.h>

class Simple : public wxFrame
{
public:
    Simple(const wxString& title);

};
//-----------
//--simple.cpp
//#include "simple.h"

Simple::Simple(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
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
//#include "simple.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    Simple *simple = new Simple(wxT("Simple"));
    simple->Show(true);

    return true;
}
