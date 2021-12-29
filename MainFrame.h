#pragma once
#include "Helpers.h"
#include "OCVProc.h"
#include "SerialPort.h"
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
	std::string selectedCOMPort;
	wxSize defaultButtonSize = wxSize(60,30);
	wxPanel *streamContainer;
	wxPanel *basePanel;
	wxButton *btnCapture;
	wxButton *btnRotate;
	wxButton *btnMirror;
	wxCheckListBox* selectPoints;
	wxButton* sendPoints;
	OCVProc *ocvProc;
	SerialPort* serialPort;
	wxStandardID EnterCameraIP(wxWindow *parent, std::string &ip);
	wxStandardID SelectString(wxWindow* parent, wxArrayString deviceDisplayNamesAS, int& index);
	wxStandardID SelectString(wxWindow *parent, std::vector<wchar_t *> strings, int &index);
	bool SelectCOMPort(std::vector<unsigned long> ports);
	void InitUI(wxSize size,wxSize cameraRes);
	void btnCaptureClicked(wxCommandEvent &event);
	void btnRotateClicked(wxCommandEvent &event);
	void btnMirrorClicked(wxCommandEvent &event);
	bool cameraFound = false;
	bool mcuConnected = false;
public:
	bool CameraFound();
	MainFrame(const wxString &title, const wxSize &size);
	bool SelectCameraDialog();
	~MainFrame();
};
