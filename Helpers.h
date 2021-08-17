#pragma once
#include <wx/arrstr.h>
#include <vector>
#include <iostream>
#include <fstream>
class Helpers
{
public:
	static void wxArrStrFromVector(wxArrayString* pArrStr, std::vector<wchar_t*>vector);
	static void ByteArrayToFile(unsigned char** buffer, int buffersize);
};

