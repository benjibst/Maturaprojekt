#pragma once
#include "Helpers.h"
#include "OCVProc.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MainFrame :public wxFrame
{
	int selectedDevice;
	wxPanel* streamContainer;
	wxPanel* basePanel;
	wxButton* btnCapture;
	OCVProc* ocvProc;
	wxStandardID EnterCameraIP(wxWindow* parent,std::string& ip);
	wxStandardID SelectString(wxWindow* parent, std::vector<wchar_t*> strings, int& index);
	void InitUI(wxSize size);
public:
	bool cameraFound = false;
	MainFrame(const wxString& title, const wxSize& size);
	~MainFrame();
};

