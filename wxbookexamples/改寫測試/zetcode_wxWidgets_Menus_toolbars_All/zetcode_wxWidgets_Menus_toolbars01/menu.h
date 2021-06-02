#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <wx/wx.h>
#include <wx/menu.h>

class SimpleMenu : public wxFrame
{
public:
    SimpleMenu(const wxString& title);

    void OnQuit(wxCommandEvent& event);

    wxMenuBar *menubar;
    wxMenu *file;

};


#endif // MENU_H_INCLUDED
