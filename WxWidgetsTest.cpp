// wxWidgets "Hello World" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include "MediaFoundationLib.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyApp : public wxApp
{
    MediaFoundationLib* mfObject;
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
private:
    MediaFoundationLib* mfObject;
};

enum
{
    ID_Hello = 1
};

wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    HWND handle = frame->GetHWND();
    mfObject= new MediaFoundationLib();
    mfObject->InitMFPreview(&handle);
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{

}
