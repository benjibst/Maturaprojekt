#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title)
{
	mfObject = new MediaFoundationLib();
	mfObject->GetVideoDevices();
	SetSize(size);
	SetBackgroundColour(wxColour(0xFFFFFFFFul));
}

VOID MainFrame::OnDeviceSelected(int index)
{
	selectedDevice = index;
}
