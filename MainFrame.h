#pragma once
#include "MediaFoundationLib.h"
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MainFrame :public wxFrame
{
	int selectedDevice;
	MediaFoundationLib* mfObject;
public:
	VOID OnDeviceSelected(int index);
	MainFrame(const wxString& title, const wxSize& size);
};

