/*
http://zetcode.com/gui/wxwidgets/firstprograms/
Widgets communicate

It is important to know, how widgets can communicate in application. Follow the next example.
*/
#include "Communicate.h"
//--main.h
#include <wx/wx.h>

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};
//-----------
//--main.cpp
//#include "main.h"
//#include "Communicate.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    Communicate *communicate = new Communicate(wxT("Widgets communicate"));
    communicate->Show(true);

    return true;
}
/*
In our example we have two panels. A left and right panel. The left panel has two buttons. The right panel has one static text. The buttons change the number displayed in the static text. The question is, how do we grab the pointer to the static text?

m_parent = parent;
Here we save the pointer to the parent widget of the LeftPanel. It is a wxPanel widget.

Communicate *comm = (Communicate *) m_parent->GetParent();
comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count));
These two lines are the most important lines of the example. It is shown, how we get access to the static text widget, which is placed on a different panel. First we get the parent of the both left and right panels. This parent widget has a pointer to the right panel. And the right panel has a pointer to the static text.
*/
