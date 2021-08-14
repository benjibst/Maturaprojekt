#include "Helpers.h"

void Helpers::wxArrStrFromVector(wxArrayString* arrstr, std::vector<wchar_t*> vec)
{
	if (arrstr!=nullptr&&!vec.empty())
	{
		arrstr->Clear();
		for (int i = 0; i < vec.size(); i++)
			arrstr->Add(vec[i]);
	}
}
