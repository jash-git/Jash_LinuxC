/***************************************************************
 * Name:      wxbook_09_01Main.h
 * Purpose:   Defines Application Frame
 * Author:    jash.liao (jash.liao@gmail.com)
 * Created:   2014-12-07
 * Copyright: jash.liao (http://jashliao.pixnet.net/blog)
 * License:
 **************************************************************/

#ifndef WXBOOK_09_01MAIN_H
#define WXBOOK_09_01MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wxbook_09_01App.h"


#include <wx/button.h>
#include <wx/statline.h>
class wxbook_09_01Dialog: public wxDialog
{
    public:
        wxbook_09_01Dialog(wxDialog *dlg, const wxString& title);
        ~wxbook_09_01Dialog();

    protected:
        enum
        {
            idBtnQuit = 1000,
            idBtnAbout
        };
        wxStaticText* m_staticText1;
        wxButton* BtnAbout;
        wxStaticLine* m_staticline1;
        wxButton* BtnQuit;

    private:
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        DECLARE_EVENT_TABLE()
};

#endif // WXBOOK_09_01MAIN_H
