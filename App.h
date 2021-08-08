#pragma once
#include <wx/wxprec.h>
#include "MediaFoundationLib.h"
#include "MainFrame.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
class App : public wxApp
{
    MediaFoundationLib* mfObject;
public:
    virtual bool OnInit();
};