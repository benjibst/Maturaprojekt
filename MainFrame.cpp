#include "MainFrame.h"
void MainFrame::btnCaptureClicked(wxCommandEvent &event)
{
	if (ocvProc->IsStreaming()) 
	{
		ocvProc->StopCameraStream();
		ocvProc->ProcessImage();
		btnCapture->SetLabel(wxString("Restart"));
	}
	else
	{
		ocvProc->StartCameraStream();
		btnCapture->SetLabel(wxString("Capture!"));
	}
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
	ocvProc = new OCVProc();
	cameraFound = SelectCameraDialog();
	if (cameraFound)
	{
		InitUI(size, ocvProc->GetCameraResolution());
		ocvProc->SetStreamCanvas(streamContainer);
		ocvProc->StartCameraStream();
	}
}

bool MainFrame::SelectCameraDialog()
{
	std::string deviceIP;
	int selectedDeviceID;
	std::vector<wchar_t*> deviceNames;
	HRESULT result = Helpers::getVideoDeviceNames(deviceNames);
	if (result != S_OK || deviceNames.size() == 0)
	{
		if (EnterCameraIP(this, deviceIP) == wxID_OK)
			return ocvProc->OpenCamera(deviceIP);
	}
	if (SelectString(this, deviceNames, selectedDeviceID) == wxID_OK)
		return ocvProc->OpenCamera(selectedDeviceID);
	if (EnterCameraIP(this, deviceIP) == wxID_OK)
		return ocvProc->OpenCamera(deviceIP);
	return false;
}

wxStandardID MainFrame::EnterCameraIP(wxWindow *parent, std::string &ip)
{
	std::string lastIP;
	std::ifstream ifile;
	std::ofstream ofile;
	wxTextEntryDialog* enterIP;
	ifile.open("ip.txt");
	if (ifile.is_open())
	{
		std::getline(ifile, lastIP);
		ifile.close();
		enterIP = new wxTextEntryDialog(parent, wxString(""), wxString("Enter camera IP"), wxString(lastIP));
	}
	else
		enterIP = new wxTextEntryDialog(parent, wxString(""), wxString("Enter camera IP"), wxString("http://192.168.x.x:4747/video"));

	wxStandardID dialogResult = (wxStandardID)enterIP->ShowModal();
	ip = dialogResult == wxID_OK ? std::string(enterIP->GetValue()) : "";
	ofile.open("ip.txt");
	if (ofile.is_open())
	{
		ofile.clear();
		ofile<<ip;
		ofile.close();
	}
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

void MainFrame::InitUI(wxSize size,wxSize cameraRes)
{
	//640 ist standard x auflösung von droidcam
	int maxXYcameraRes = std::max(cameraRes.x, cameraRes.y);
	SetSize(wxSize(size.x + maxXYcameraRes-640,size.y+maxXYcameraRes-640));

	basePanel = new wxPanel(this, wxID_ANY);
	basePanel->SetBackgroundColour(wxColour(0xFFFFFFul));
	streamContainer = new wxPanel(basePanel, wxID_ANY, wxPoint(20, 20), wxSize(maxXYcameraRes, maxXYcameraRes));

	btnCapture = new wxButton(basePanel, wxID_ANY, wxString("Capture!"), wxPoint(40+maxXYcameraRes, 20), defaultButtonSize);
	btnCapture->Bind(wxEVT_BUTTON, &MainFrame::btnCaptureClicked, this);

	btnRotate = new wxButton(basePanel, wxID_ANY, wxEmptyString, 
		wxPoint(40+maxXYcameraRes, 40 + defaultButtonSize.y), wxSize(defaultButtonSize.y, defaultButtonSize.y));
	btnRotate->Bind(wxEVT_BUTTON, &MainFrame::btnRotateClicked, this);

	btnMirror = new wxButton(basePanel, wxID_ANY, wxEmptyString, 
		wxPoint(maxXYcameraRes+defaultButtonSize.x+80, 40+defaultButtonSize.y), wxSize(defaultButtonSize.y, defaultButtonSize.y));
	btnMirror->Bind(wxEVT_BUTTON, &MainFrame::btnMirrorClicked, this);
}

bool MainFrame::CameraFound() { return cameraFound; }

MainFrame::~MainFrame()
{
	ocvProc->StopCameraStream();
	delete ocvProc;
}
