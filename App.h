#pragma once
#include "MainFrame.h"
#include "SerialPort.h"
#include "OCVProc.h"
#include <string>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class App : public wxApp
{
    MCUConn serialPort;
    OCVProc ocvProc;
    MainFrame* mainFrame;
    bool SelectCOMPort(std::vector<unsigned long> ports);
    bool SelectCameraDialog();
    int SelectString(wxWindow* parent, wxArrayString strings);
    bool EnterCameraIP(wxWindow* parent, std::string& ip);

public:
    virtual bool OnInit();
    ~App();
};