/*
http://zetcode.com/gui/wxwidgets/menustoolbars/
Simple menu example

Creating a menubar in wxWidgets is very simple.
*/
#include <wx/wx.h>
#include "menu.h"

class MyApp : public wxApp
{
  public:
    virtual bool OnInit();
};


IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    SimpleMenu *menu = new SimpleMenu(wxT("Simple Menu"));
    menu->Show(true);

    return true;
}
/*
menubar = new wxMenuBar;
First we create a menubar object.

file = new wxMenu;
Next we create a menu object.

file->Append(wxID_EXIT, wxT("&Quit"));
We append a menu item into the menu object. The first parameter is the id of the menu item. The second parameter is the name of the menu item. Here we did not create a wxMenuItem explicitly. It was created by the Append() method behind the scenes. Later on, we will create a wxMenuItem manually.

menubar->Append(file, wxT("&File"));
SetMenuBar(menubar);
After that, we append a menu into the menubar. The & character creates an accelerator key. The character that follows the & is underlined. This way the menu is accessible via the Alt+F shortcut. In the end, we call the SetMenuBar() method. This method belongs to the wxFrame widget. It sets up the menubar.
*/
