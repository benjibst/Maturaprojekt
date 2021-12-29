#include "App.h"

wxIMPLEMENT_APP(App);
bool App::OnInit()
{
	mainFrame = new MainFrame(wxString("Test"), wxSize(900, 800));
	if (mainFrame->CameraFound())
	{
		mainFrame->Show(true);
		return true;
	}

	return false;
}