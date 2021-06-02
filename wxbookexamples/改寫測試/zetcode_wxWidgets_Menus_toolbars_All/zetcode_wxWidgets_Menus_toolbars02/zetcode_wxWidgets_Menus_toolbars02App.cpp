/*
http://zetcode.com/gui/wxwidgets/menustoolbars/
Submenus

Each menu can also have a submenu. This way we can group similar commands into groups. For example we can place commands that hide or show various toolbars like personal bar, address bar, status bar, or navigation bar into a submenu called toolbars. Within a menu, we can separate commands with a separator. It is a simple line. It is common practice to separate commands like new, open, save from commands like print, print preview with a single separator. In our example we will see, how we can create submenus and menu separators.
*/
#include "menu.h"
#include <wx/wx.h>

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    SubMenu *smenu = new SubMenu(wxT("Submenu"));
    smenu->Show(true);

    return true;
}
/*
We created one submenu in a file menu. It is an import submenu, which can be seen in Opera web browser.

file->AppendSeparator();
A menu separator line is created calling an AppendSeparator() method.

imp = new wxMenu;
imp->Append(wxID_ANY, wxT("Import newsfeed list..."));
imp->Append(wxID_ANY, wxT("Import bookmarks..."));
imp->Append(wxID_ANY, wxT("Import mail..."));

file->AppendSubMenu(imp, wxT("I&mport"));
A submenu is created like a normal menu. It is appended with a AppendSubMenu() method.
*/
