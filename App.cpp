#include "App.h"

wxIMPLEMENT_APP(App);
bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame();
    HWND handle = mainFrame->GetHWND();
    mfObject = new MediaFoundationLib();
    mfObject->InitMFPreview(&handle);
    mainFrame->Show(true);
    return true;
}