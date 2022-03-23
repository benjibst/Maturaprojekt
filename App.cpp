#include "App.h"

wxIMPLEMENT_APP(App);
bool App::OnInit()
{
	
	std::vector<unsigned long> portIDS = MCUConn::FindAvailableComPorts();
	SelectCOMPort(portIDS);
		

	if (!SelectCameraDialog())
		return false;

	mainFrame = new MainFrame(wxString("Test"), wxSize(900, 800), &ocvProc, &serialPort);
	mainFrame->Show(true);
	ocvProc.StartCameraStream();
	return true;
}
bool App::SelectCOMPort(std::vector<unsigned long> ports)
{
	int selectedIndex = 0;
	wxArrayString portNames;
	for (auto i : ports)
	{
		wxString name("COM");
		name += std::to_string(i);
		portNames.Add(name);
	}
	if (SelectString(nullptr, portNames, selectedIndex))
	{
		bool open =serialPort.OpenPort(ports[selectedIndex]);
		return true;
	}
	return false;
}

bool App::SelectCameraDialog()
{
	std::string deviceIP;
	int selectedDeviceID;
	std::vector<wchar_t*> deviceNames;

	if (EnterCameraIP(nullptr, deviceIP))
		return ocvProc.OpenCamera(deviceIP);

	HRESULT result = Helpers::getVideoDeviceNames(deviceNames);
	if (result != S_OK || deviceNames.size() == 0)
		return false;



	if (SelectString(nullptr, Helpers::wxArrStrFromVector(deviceNames), selectedDeviceID))
		return ocvProc.OpenCamera(selectedDeviceID);
	return false;
}

bool App::SelectString(wxWindow* parent, wxArrayString strings, int& index)
{
	wxSingleChoiceDialog selectString(parent, wxString(""), wxString("Pick device"), strings);
	wxStandardID dialogResult = (wxStandardID)selectString.ShowModal();
	index = dialogResult == wxID_OK ? selectString.GetSelection() : -1;
	return dialogResult == wxID_OK;
}

bool App::EnterCameraIP(wxWindow* parent, std::string& ip)
{
	std::string cameraIP;
	std::ifstream ifile;
	std::ofstream ofile;

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
	ifile.close();
	wxTextEntryDialog enterIP(parent, wxString(""), wxString("Enter camera IP"), wxString(cameraIP));
	wxStandardID dialogResult = (wxStandardID)enterIP.ShowModal();
	ip = std::string(enterIP.GetValue());
	ofile.open("ip.txt");
	if (ofile.is_open())
	{
		ofile.clear();
		ofile << ip;
		ofile.close();
	}
	return dialogResult == wxID_OK;
}

App::~App()
{

}