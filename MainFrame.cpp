#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title)
{
	HWND thisHandle = this->GetHWND();
	SetSize(size);
	SetBackgroundColour(wxColour(0xFFFFFFFFul));
	mfObject = new MediaFoundationLib();
	mfObject->GetVideoDevices();
	wxArrayString deviceDisplayNamesAS;
	std::vector<wchar_t*> deviceNames= mfObject->GetDeviceNames();
	for (int i = 0; i < deviceNames.size(); i++)
	{
		deviceDisplayNamesAS.Add(deviceNames[i]);
	}
	wxSingleChoiceDialog selectDevice(this,wxString("Pick Device"), wxString("Pick device"), deviceDisplayNamesAS);
	int dialogResult = selectDevice.ShowModal();
	if (dialogResult == wxID_OK)
	{
		int selectedDevice = selectDevice.GetSelection();
		mfObject->SetSelectedDeviceIndex(selectedDevice);
		mfObject->InitMFPreview(&thisHandle);
	}

}
