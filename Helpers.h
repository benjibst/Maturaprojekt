#pragma once
#include <wx/arrstr.h>
class Helpers
{
public:
	static void wxArrStrFromVector(wxArrayString* pArrStr, std::vector<wchar_t*>vector);
};

