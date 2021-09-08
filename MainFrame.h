#pragma once
#include "Helpers.h"
#include "OCVProc.h"
#include <wx/choicdlg.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MainFrame :public wxFrame
{
	int selectedDevice=-1;
	wxPanel* streamContainer;
	wxPanel* basePanel;
	OCVProc* ocvProc;
public:
	MainFrame(const wxString& title, const wxSize& size);
};

