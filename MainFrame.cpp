#include "MainFrame.h"
void MainFrame::btnCaptureClicked(wxCommandEvent &event)
{
	ocvProc->StopCameraStream();
	ocvProc->ProcessImage();
}

void MainFrame::btnRotateClicked(wxCommandEvent &event)
{
	ocvProc->rotateStream();
}
void MainFrame::btnMirrorClicked(wxCommandEvent &event)
{
	ocvProc->mirrorStream();
}

MainFrame::MainFrame(const wxString &title, const wxSize &size)
	: wxFrame(0, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER))
{
	InitUI(size);

	ocvProc = new OCVProc();

	std::vector<wchar_t *> deviceNames;
	wxStandardID dialogResult = Helpers::getVideoDeviceNames(deviceNames);
	if (dialogResult == wxID_CANCEL || deviceNames.size() == 0)
	{
		std::string deviceIP;
		if (EnterCameraIP(this, deviceIP) == wxID_OK)
			cameraFound = ocvProc->Init(deviceIP, streamContainer);
		return;
	}
	else
	{
		int selectedDeviceID;
		if (SelectString(this, deviceNames, selectedDeviceID) == wxID_OK)
			cameraFound = ocvProc->Init(selectedDeviceID, streamContainer);
		else
		{
			std::string deviceIP;
			if (EnterCameraIP(this, deviceIP) == wxID_OK)
				cameraFound = ocvProc->Init(deviceIP, streamContainer);
		}
	}
}

wxStandardID MainFrame::EnterCameraIP(wxWindow *parent, std::string &ip)
{
	wxTextEntryDialog *enterIP = new wxTextEntryDialog(parent, wxString(""), wxString("Enter camera IP"), wxString("http://192.168.x.x:4747/video"));
	wxStandardID dialogResult = (wxStandardID)enterIP->ShowModal();
	ip = dialogResult == wxID_OK ? std::string(enterIP->GetValue()) : "";
	delete enterIP;
	return dialogResult;
}

wxStandardID MainFrame::SelectString(wxWindow *parent, std::vector<wchar_t *> strings, int &index)
{
	wxArrayString deviceDisplayNamesAS(strings.size(), (const wchar_t **)strings.data());

	wxSingleChoiceDialog *selectString = new wxSingleChoiceDialog(parent, wxString(""), wxString("Pick device"), deviceDisplayNamesAS);
	wxStandardID dialogResult = (wxStandardID)selectString->ShowModal();
	index = dialogResult == wxID_OK ? selectString->GetSelection() : -1;
	delete selectString;
	return dialogResult;
}

void MainFrame::InitUI(wxSize size)
{
	SetSize(size);

	basePanel = new wxPanel(this, wxID_ANY);
	basePanel->SetBackgroundColour(wxColour(0xFFFFFFul));
	streamContainer = new wxPanel(basePanel, wxID_ANY, wxPoint(20, 20), wxSize(640, 640));

	btnCapture = new wxButton(basePanel, wxID_ANY, wxString("Capture!"), wxPoint(680, 20), defaultButtonSize);
	btnCapture->Bind(wxEVT_BUTTON, &MainFrame::btnCaptureClicked, this);

	btnRotate = new wxButton(basePanel, wxID_ANY, wxEmptyString, 
		wxPoint(680, 40 + defaultButtonSize.y), wxSize(defaultButtonSize.y, defaultButtonSize.y));
	btnRotate->Bind(wxEVT_BUTTON, &MainFrame::btnRotateClicked, this);

	btnMirror = new wxButton(basePanel, wxID_ANY, wxEmptyString, 
		wxPoint(680+defaultButtonSize.x+40, 40+defaultButtonSize.y), wxSize(defaultButtonSize.y, defaultButtonSize.y));
	btnMirror->Bind(wxEVT_BUTTON, &MainFrame::btnMirrorClicked, this);
}

bool MainFrame::CameraFound() { return cameraFound; }

MainFrame::~MainFrame()
{
	ocvProc->StopCameraStream();
	delete ocvProc;
}
