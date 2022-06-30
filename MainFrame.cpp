#include "MainFrame.h"
MainFrame::MainFrame(const wxString& title, const wxSize& size, OCVProc* ocvObj, MCUConn* serialObj)
	: wxFrame(0, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER))
{
	serialPort = serialObj;
	ocvProc = ocvObj;
	InitUI(size, ocvProc->GetCameraResolution());
	ocvProc->SetStreamCanvas(streamContainer);
}
void MainFrame::btnCaptureClicked(wxCommandEvent& event)
{
	if (ocvProc->IsStreaming())
	{
		ocvProc->StopCameraStream();
		std::vector<cv::Point2f> points = ocvProc->ProcessImage();
		serialPort->SendCoordData(points);
		unsigned char str[10];
		DWORD read;
		serialPort->Read(str, 10);
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

void MainFrame::autoManChanged(wxCommandEvent& event)
{
	if (choiceManAuto->GetSelection() == 0)
		serialPort->SetAutoMode();
	if (choiceManAuto->GetSelection() == 1)
		serialPort->SetManMode();
}

void MainFrame::InitUI(wxSize size, wxSize cameraRes)
{
	//640 ist standard x auflösung von droidcam
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

	wxArrayString choices;
	choices.Add(wxString("Auto"));
	choices.Add(wxString("Manual"));
	choiceManAuto = new wxChoice(basePanel, wxID_ANY, wxPoint(maxXYcameraRes + 40, 80 + defaultButtonSize.y), wxDefaultSize,
		choices);
	choiceManAuto->Bind(wxEVT_CHOICE, &MainFrame::autoManChanged, this);
}

MainFrame::~MainFrame()
{
}
