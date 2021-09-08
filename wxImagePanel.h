#pragma once
#include <wx/wxprec.h>
#include <wx/sizer.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class wxImagePanel : public wxPanel
{

public:
	wxBitmap image;
	wxImagePanel(wxFrame* parent);

	void paintEvent(wxPaintEvent& evt);
	void paintNow();

	void render(wxDC& dc);

	// some useful events
	/*
	 void mouseMoved(wxMouseEvent& event);
	 void mouseDown(wxMouseEvent& event);
	 void mouseWheelMoved(wxMouseEvent& event);
	 void mouseReleased(wxMouseEvent& event);
	 void rightClick(wxMouseEvent& event);
	 void mouseLeftWindow(wxMouseEvent& event);
	 void keyPressed(wxKeyEvent& event);
	 void keyReleased(wxKeyEvent& event);
	 */

	DECLARE_EVENT_TABLE()
};

