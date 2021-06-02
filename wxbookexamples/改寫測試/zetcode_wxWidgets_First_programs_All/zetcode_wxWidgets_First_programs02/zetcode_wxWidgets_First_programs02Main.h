/***************************************************************
 * Name:      zetcode_wxWidgets_First_programs02Main.h
 * Purpose:   Defines Application Frame
 * Author:    jash.liao (jash.liao@gmail.com)
 * Created:   2014-12-09
 * Copyright: jash.liao (http://jashliao.pixnet.net/blog)
 * License:
 **************************************************************/

#ifndef ZETCODE_WXWIDGETS_FIRST_PROGRAMS02MAIN_H
#define ZETCODE_WXWIDGETS_FIRST_PROGRAMS02MAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "zetcode_wxWidgets_First_programs02App.h"


#include <wx/button.h>
#include <wx/statline.h>
class zetcode_wxWidgets_First_programs02Dialog: public wxDialog
{
    public:
        zetcode_wxWidgets_First_programs02Dialog(wxDialog *dlg, const wxString& title);
        ~zetcode_wxWidgets_First_programs02Dialog();

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

#endif // ZETCODE_WXWIDGETS_FIRST_PROGRAMS02MAIN_H
