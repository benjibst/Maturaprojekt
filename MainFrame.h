#pragma once
#include "Helpers.h"
#include "OCVProc.h"
#include <algorithm>
#include <iostream>
#include <fstream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MainFrame : public wxFrame
{
	int selectedDevice;
	wxSize defaultButtonSize = wxSize(60,30);
	wxPanel *streamContainer;
	wxPanel *basePanel;
	wxButton *btnCapture;
	wxButton *btnRotate;
	wxButton *btnMirror;
	OCVProc *ocvProc;
	wxStandardID EnterCameraIP(wxWindow *parent, std::string &ip);
	wxStandardID SelectString(wxWindow *parent, std::vector<wchar_t *> strings, int &index);
	void InitUI(wxSize size,wxSize cameraRes);
	void btnCaptureClicked(wxCommandEvent &event);
	void btnRotateClicked(wxCommandEvent &event);
	void btnMirrorClicked(wxCommandEvent &event);
	bool cameraFound = false;

public:
	bool CameraFound();
	MainFrame(const wxString &title, const wxSize &size);
	bool SelectCameraDialog();
	~MainFrame();
};
