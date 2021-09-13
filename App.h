#pragma once
#include "MainFrame.h"

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class App : public wxApp
{
    MainFrame* mainFrame;
public:
    virtual bool OnInit();
};