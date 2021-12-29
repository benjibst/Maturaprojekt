#include "MainFrame.h"
MainFrame::MainFrame(const wxString& title, const wxSize& size)
	: wxFrame(0, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER))
{
	ocvProc = new OCVProc();
	serialPort = new SerialPort();
	std::vector<unsigned long> ports = SerialPort::FindAvailableComPorts();
	if (ports.size() != 0&& SelectCOMPort(ports))
		serialPort->OpenPort(selectedCOMPort);

	cameraFound = SelectCameraDialog();
	if (cameraFound)
	{
		InitUI(size, ocvProc->GetCameraResolution());
		ocvProc->SetStreamCanvas(streamContainer);
		ocvProc->StartCameraStream();
	}
}
void MainFrame::btnCaptureClicked(wxCommandEvent& event)
{
	if (ocvProc->IsStreaming())
	{
		ocvProc->StopCameraStream();
		std::vector<cv::Point2f> points = ocvProc->ProcessImage();
		btnCapture->SetLabel(wxString("Restart"));
	}
	else
	{
		ocvProc->StartCameraStream();
		btnCapture->SetLabel(wxString("Capture!"));
	}
}

void MainFrame::btnRotateClicked(wxCommandEvent& event)
{
	ocvProc->rotateStream();
}
void MainFrame::btnMirrorClicked(wxCommandEvent& event)
{
	ocvProc->mirrorStream();
}

bool MainFrame::SelectCameraDialog()
{
	std::string deviceIP;
	int selectedDeviceID;
	std::vector<wchar_t*> deviceNames;

	if (EnterCameraIP(this, deviceIP) == wxID_OK)
		return ocvProc->OpenCamera(deviceIP);

	HRESULT result = Helpers::getVideoDeviceNames(deviceNames);
	if (result != S_OK || deviceNames.size() == 0)
		return false;

	if (SelectString(this, deviceNames, selectedDeviceID) == wxID_OK)
		return ocvProc->OpenCamera(selectedDeviceID);
	return false;
}

wxStandardID MainFrame::EnterCameraIP(wxWindow* parent, std::string& ip)
{
	std::string cameraIP;
	std::ifstream ifile;
	std::ofstream ofile;
	wxTextEntryDialog* enterIP;
	ifile.open("ip.txt");
	if (ifile.is_open())
	{
		std::getline(ifile, cameraIP);
		if (cameraIP.length() == 0)
			cameraIP = defaultIP;
	}
	else
	{
		std::ofstream{ "ip.txt" };
		cameraIP = defaultIP;
	}
	enterIP = new wxTextEntryDialog(parent, wxString(""), wxString("Enter camera IP"), wxString(cameraIP));
	ifile.close();
	wxStandardID dialogResult = (wxStandardID)enterIP->ShowModal();
	ip = std::string(enterIP->GetValue());
	ofile.open("ip.txt");
	if (ofile.is_open())
	{
		ofile.clear();
		ofile << ip;
		ofile.close();
	}
	delete enterIP;
	return dialogResult;
}

wxStandardID MainFrame::SelectString(wxWindow* parent, wxArrayString deviceDisplayNamesAS, int& index)
{
	wxSingleChoiceDialog* selectString = new wxSingleChoiceDialog(parent, wxString(""), wxString("Pick device"), deviceDisplayNamesAS);
	wxStandardID dialogResult = (wxStandardID)selectString->ShowModal();
	index = dialogResult == wxID_OK ? selectString->GetSelection() : -1;
	delete selectString;
	return dialogResult;
}

wxStandardID MainFrame::SelectString(wxWindow* parent, std::vector<wchar_t*> strings, int& index)
{
	wxArrayString deviceDisplayNamesAS(strings.size(), (const wchar_t**)strings.data());
	return SelectString(parent, deviceDisplayNamesAS, index);
}

bool MainFrame::SelectCOMPort(std::vector<unsigned long> ports)
{
	int selectedIndex = 0;
	wxArrayString portNames;
	for (auto i : ports)
	{
		wxString name("COM");
		name += std::to_string(i);
		portNames.Add(name);
	}
	if (SelectString(this, portNames, selectedIndex) == wxID_OK)
	{
		selectedCOMPort = portNames[selectedIndex];
		return true;
	}
	return false;
}

void MainFrame::InitUI(wxSize size, wxSize cameraRes)
{
	//640 ist standard x aufl�sung von droidcam
	int maxXYcameraRes = std::max(cameraRes.x, cameraRes.y);
	SetSize(wxSize(size.x + maxXYcameraRes - 640, size.y + maxXYcameraRes - 640));

	basePanel = new wxPanel(this, wxID_ANY);
	basePanel->SetBackgroundColour(wxColour(0xFFFFFFul));
	streamContainer = new wxPanel(basePanel, wxID_ANY, wxPoint(20, 20), wxSize(maxXYcameraRes, maxXYcameraRes));

	btnCapture = new wxButton(basePanel, wxID_ANY, wxString("Capture!"), wxPoint(40 + maxXYcameraRes, 20), defaultButtonSize);
	btnCapture->Bind(wxEVT_BUTTON, &MainFrame::btnCaptureClicked, this);

	btnRotate = new wxButton(basePanel, wxID_ANY, wxEmptyString,
		wxPoint(40 + maxXYcameraRes, 40 + defaultButtonSize.y), wxSize(defaultButtonSize.y, defaultButtonSize.y));
	btnRotate->Bind(wxEVT_BUTTON, &MainFrame::btnRotateClicked, this);

	btnMirror = new wxButton(basePanel, wxID_ANY, wxEmptyString,
		wxPoint(maxXYcameraRes + defaultButtonSize.x + 80, 40 + defaultButtonSize.y), wxSize(defaultButtonSize.y, defaultButtonSize.y));
	btnMirror->Bind(wxEVT_BUTTON, &MainFrame::btnMirrorClicked, this);
}

bool MainFrame::CameraFound() { return cameraFound; }

MainFrame::~MainFrame()
{
	ocvProc->StopCameraStream();
	delete ocvProc;
}
