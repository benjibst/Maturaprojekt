#include "App.h"

wxIMPLEMENT_APP(App);
bool App::OnInit()
{
	mainFrame = new MainFrame(wxString("Test"), wxSize(900, 900));
	if (mainFrame->CameraFound())
		mainFrame->Show(true);
	else
		mainFrame->Destroy();
	return true;
}