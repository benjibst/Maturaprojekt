#include "MainFrame.h"

void MainFrame::btnCaptureClicked(wxCommandEvent& event)
{
	mfObject->captureImage(&CapturedImage);
	Helpers::ByteArrayToFile(CapturedImage.imageData,CapturedImage.bufferSize);
}

MainFrame::MainFrame(const wxString& title, const wxSize& size)
	:wxFrame(NULL, wxID_ANY, title,wxDefaultPosition,wxDefaultSize, wxDEFAULT_FRAME_STYLE&~(wxRESIZE_BORDER))
{
	SetSize(size);
	SetBackgroundColour(wxColour(0xdfe6e9ul));

	basePanel = new wxPanel(this, wxID_ANY);
	basePanel->SetBackgroundColour(wxColour(0xFFFFFFul));;
	mfStreamContainer = new wxPanel(basePanel, wxID_ANY, wxPoint(20, 20), wxSize(640, 480));
	wxButton* btnCapture = new wxButton(basePanel, wxID_ANY,wxEmptyString,wxPoint(680,20));
	btnCapture->Bind(wxEVT_BUTTON, &MainFrame::btnCaptureClicked,this);
	HWND streamHandle = mfStreamContainer->GetHWND();

	mfObject = new MediaFoundationLib();
	mfObject->GetVideoDevices();

	wxArrayString deviceDisplayNamesAS;
	Helpers::wxArrStrFromVector(&deviceDisplayNamesAS, mfObject->GetDeviceNames());

	wxSingleChoiceDialog selectDevice(this,wxString(""), wxString("Pick device"), deviceDisplayNamesAS);
	int dialogResult = selectDevice.ShowModal();
	if (dialogResult == wxID_OK)
	{
		int selectedDevice = selectDevice.GetSelection();
		mfObject->SetSelectedDeviceIndex(selectedDevice);
		
		PreviewInitialized=mfObject->InitMFPreview(&streamHandle);
	}

}

MainFrame::~MainFrame()
{
	if (CapturedImage.imageData != NULL)
		free(CapturedImage.imageData);
	mfObject->Dispose();
	delete mfObject;
}
