/***************************************************************
 * Name:      zetcode_wxWidgets_First_programs03App.h
 * Purpose:   Defines Application Class
 * Author:    jash.liao (jash.liao@gmail.com)
 * Created:   2014-12-09
 * Copyright: jash.liao (http://jashliao.pixnet.net/blog)
 * License:
 **************************************************************/

#ifndef ZETCODE_WXWIDGETS_FIRST_PROGRAMS03APP_H
#define ZETCODE_WXWIDGETS_FIRST_PROGRAMS03APP_H

#include <wx/app.h>

class zetcode_wxWidgets_First_programs03App : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // ZETCODE_WXWIDGETS_FIRST_PROGRAMS03APP_H
