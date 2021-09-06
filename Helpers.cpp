#include "Helpers.h"

void Helpers::wxArrStrFromVector(wxArrayString* arrstr, std::vector<wchar_t*> vec)
{
	if (arrstr != nullptr && !vec.empty())
	{
		arrstr->Clear();
		for (int i = 0; i < vec.size(); i++)
			arrstr->Add(vec[i]);
	}
}

void Helpers::ByteArrayToFile(unsigned char* buffer, int buffersize)
{
	std::ofstream imageFile;
	imageFile.open("D:\\Programme\\Cpp_prog\\wxWidgetsMF\\Images\\image.txt", std::ios::out);
	int count = 0;
	for (int x = 0; x < buffersize; x++)
	{
		count++;
		imageFile << buffer[x] << " ";
		if (count == 640)
		{
			imageFile << "\r\n";
			count = 0;
		}
	}
	imageFile.close();
}
