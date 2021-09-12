#pragma once
#include <wx/wxprec.h>
#include "MainFrame.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class App : public wxApp
{
    MainFrame* mainFrame;
public:
    virtual bool OnInit();
};