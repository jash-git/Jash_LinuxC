/***************************************************************
 * Name:      wxbooke_02_01Main.h
 * Purpose:   Defines Application Frame
 * Author:    jash.liao (jash.liao@gmail.com)
 * Created:   2014-12-05
 * Copyright: jash.liao (http://jashliao.pixnet.net/blog)
 * License:
 **************************************************************/

#ifndef WXBOOKE_02_01MAIN_H
#define WXBOOKE_02_01MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wxbooke_02_01App.h"


#include <wx/button.h>
#include <wx/statline.h>
class wxbooke_02_01Dialog: public wxDialog
{
    public:
        wxbooke_02_01Dialog(wxDialog *dlg, const wxString& title);
        ~wxbooke_02_01Dialog();

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

#endif // WXBOOKE_02_01MAIN_H
