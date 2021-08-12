#pragma once
#include <wx/wxprec.h>
#include <wx/arrstr.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class PickDeviceDialog : public wxPanel
{
public:
	PickDeviceDialog(wxWindow* Parent, wxArrayString deviceNames);
	PickDeviceDialog(wxWindow* Parent, std::vector<wchar_t*> deviceNames);
	~PickDeviceDialog();
private:
	void OnDeviceSelected(wxCommandEvent& event);
	wxButton* btnSelectDevice;
	wxChoice* choiceDevicesList;
};