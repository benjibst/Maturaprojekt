#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER))
{
	InitUI(size);



	HDC drawingDC = GetDC(streamContainer->GetHWND());

	ocvProc = new OCVProc();

	std::vector<wchar_t*> deviceNames;
	if (Helpers::getVideoDeviceNames(deviceNames) == wxID_CANCEL || deviceNames.size() == 0)
	{
		std::string deviceIP;
		if (EnterCameraIP(this, deviceIP) == wxID_OK)
			cameraFound = ocvProc->Init(deviceIP, drawingDC);
		return;
	}
	else
	{
		int selectedDeviceID;
		if (SelectString(this, deviceNames, selectedDeviceID) == wxID_OK)
			cameraFound = ocvProc->Init(selectedDeviceID, drawingDC);
		else
		{
			std::string deviceIP;
			if (EnterCameraIP(this, deviceIP) == wxID_OK)
				cameraFound = ocvProc->Init(deviceIP, drawingDC);
		}
	}
}

wxStandardID MainFrame::EnterCameraIP(wxWindow* parent, std::string& ip)
{
	wxTextEntryDialog* enterIP = new wxTextEntryDialog(parent, wxString(""), wxString("Enter camera IP"), wxString("http://192.168.x.x:4747/video"));
	wxStandardID dialogResult = (wxStandardID)enterIP->ShowModal();
	ip = dialogResult == wxID_OK ? std::string(enterIP->GetValue()) : "";
	enterIP->Destroy();
	return dialogResult;
}

wxStandardID MainFrame::SelectString(wxWindow* parent, std::vector<wchar_t*> strings, int& index)
{
	wxArrayString deviceDisplayNamesAS(strings.size(), (const wchar_t**)strings.data());

	wxSingleChoiceDialog* selectString = new wxSingleChoiceDialog(parent, wxString(""), wxString("Pick device"), deviceDisplayNamesAS);
	wxStandardID dialogResult = (wxStandardID)selectString->ShowModal();
	index = dialogResult == wxID_OK ? selectString->GetSelection() : -1;
	selectString->Destroy();
	return dialogResult;
}

void MainFrame::InitUI(wxSize size)
{
	SetSize(size);

	basePanel = new wxPanel(this, wxID_ANY);
	basePanel->SetBackgroundColour(wxColour(0xFFFFFFul));;
	streamContainer = new wxPanel(basePanel, wxID_ANY, wxPoint(20, 20), wxSize(640, 480));
	btnCapture = new wxButton(basePanel, wxID_ANY, wxEmptyString, wxPoint(680, 20));
}

MainFrame::~MainFrame()
{
	ocvProc->StopCameraStream();
	delete ocvProc;
	streamContainer->Destroy();
	btnCapture->Destroy();
	basePanel->Destroy();

}
