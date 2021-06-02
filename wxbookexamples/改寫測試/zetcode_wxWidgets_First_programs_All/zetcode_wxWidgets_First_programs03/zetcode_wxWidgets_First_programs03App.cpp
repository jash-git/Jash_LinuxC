/*
http://zetcode.com/gui/wxwidgets/firstprograms/
A simple button

In the following example, we create a button on the frame widget. We will show, how to create a simple event handler.
*/
//--button.h
#include <wx/wx.h>

class Button : public wxFrame
{
public:
    Button(const wxString& title);

    void OnQuit(wxCommandEvent & event);
};
//-----------
//--button.cpp
//#include "button.h"

Button::Button(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(270, 150))
{
  wxPanel *panel = new wxPanel(this, wxID_ANY);

  wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Quit"), wxPoint(20, 20));
  Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(Button::OnQuit));
  button->SetFocus();
  Centre();
}

void Button::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(true);
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
//#include "button.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    Button *btnapp = new Button(wxT("Button"));
    btnapp->Show(true);

    return true;
}
/*
wxPanel *panel = new wxPanel(this, wxID_ANY);
First we create a wxPanel widget. It will be placed inside a wxFrame widget.

wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Quit"), wxPoint(20, 20));
We create a wxButton widget. It is placed on the panel. We use the predefined wxID_EXIT id for the button. It will cause to display a small exit icon on the button. The label of the button is "Quit". The button is positioned manually at x=20, y=20 coordinates. The beginning of the coordinate system is at the upper left hand corner.

Connect(wxID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED,
    wxCommandEventHandler(Button::OnQuit));
If we click on the button, a wxEVT_COMMAND_BUTTON_CLICKED event is generated. We connect the event to the OnQuit() method of the Button class. So when we click on the button, the OnQuit() method is called.

button->SetFocus();
We set the keyboard focus to the button. So if we press the enter key, the button is being clicked.

Close(true);
Inside the OnQuit() method, we call the Close() method. This will terminate our application.
*/
