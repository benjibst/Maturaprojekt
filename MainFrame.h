#pragma once
#include "MediaFoundationLib.h"
#include <wx/choicdlg.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MainFrame :public wxFrame
{
	int selectedDevice=-1;
	MediaFoundationLib* mfObject;
public:
	MainFrame(const wxString& title, const wxSize& size);
};

