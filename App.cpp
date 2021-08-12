#include "App.h"


wxIMPLEMENT_APP(App);
bool App::OnInit()
{
	MainFrame* mainFrame = new MainFrame(wxString("Test"), wxSize(800, 800));
	mainFrame->Show(true);
	return true;
}