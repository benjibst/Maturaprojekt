#include "MainFrame.h"

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    this->SetClientSize(740, 580);
}

MainFrame::~MainFrame()
{
}
