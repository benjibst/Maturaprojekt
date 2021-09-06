#pragma once
#include "Helpers.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <wx/choicdlg.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class MainFrame :public wxFrame
{
	HRESULT PreviewInitialized=E_FAIL;
	int selectedDevice=-1;
	wxPanel* mfStreamContainer;
	wxPanel* basePanel;
public:
	MainFrame(const wxString& title, const wxSize& size);
};

