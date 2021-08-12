#include "PickDeviceDialog.h"

PickDeviceDialog::PickDeviceDialog(wxWindow* Parent, wxArrayString deviceNames)
	: wxPanel(Parent, wxID_ANY)
{
	choiceDevicesList = new wxChoice(this, wxID_ANY, wxPoint(200, 200), wxDefaultSize, deviceNames);
	btnSelectDevice = new wxButton(this, wxID_ANY, wxString(""), wxPoint(50, 50), wxSize(100, 100));
	btnSelectDevice->Bind(wxEVT_BUTTON, &PickDeviceDialog::OnDeviceSelected,this);
}

PickDeviceDialog::PickDeviceDialog(wxWindow* Parent, std::vector<wchar_t*> deviceNames)
	: wxPanel(Parent, wxID_ANY)
{
	wxArrayString deviceDisplayNames;
	for (int i = 0; i < deviceNames.size(); i++)
	{
		deviceDisplayNames.Add(deviceNames[i]);
	}
	choiceDevicesList = new wxChoice(this, wxID_ANY, wxPoint(200, 200), wxDefaultSize, deviceDisplayNames);
	btnSelectDevice = new wxButton(this, wxID_ANY, wxString(""), wxPoint(50, 50), wxSize(100, 100));
	btnSelectDevice->Bind(wxEVT_BUTTON, &PickDeviceDialog::OnDeviceSelected,this);
}

VOID PickDeviceDialog::OnDeviceSelected(wxCommandEvent& event)
{
	int a = 4;
}

PickDeviceDialog::~PickDeviceDialog()
{
}
