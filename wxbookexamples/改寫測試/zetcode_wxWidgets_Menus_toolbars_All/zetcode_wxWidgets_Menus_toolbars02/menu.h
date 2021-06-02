#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <wx/wx.h>
#include <wx/menu.h>

class SubMenu : public wxFrame
{
public:
  SubMenu(const wxString& title);

  void OnQuit(wxCommandEvent & event);

  wxMenuBar *menubar;
  wxMenu *file;
  wxMenu *imp;
  wxMenuItem *quit;

};


#endif // MENU_H_INCLUDED
