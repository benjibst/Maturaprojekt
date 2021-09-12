#include "App.h"

wxIMPLEMENT_APP(App);
bool App::OnInit()
{
	mainFrame = new MainFrame(wxString("Test"), wxSize(900, 700));
	if (mainFrame->cameraFound)
		mainFrame->Show(true);
	else
		mainFrame->Destroy();
	return true;
}