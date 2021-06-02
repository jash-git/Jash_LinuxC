#ifndef TOOLBAR_H_INCLUDED
#define TOOLBAR_H_INCLUDED
#include <wx/wx.h>

class Toolbar : public wxFrame
{
public:
    Toolbar(const wxString& title);

    void OnQuit(wxCommandEvent& event);


};


#endif // TOOLBAR_H_INCLUDED
