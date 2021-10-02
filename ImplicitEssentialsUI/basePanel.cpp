#include "basePanel.h"

basePanel::basePanel(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size) 
	: wxPanel(parent, id, pos, size)
{
	mValidator = new wxTextValidator(wxFILTER_INCLUDE_CHAR_LIST);

	wxArrayString list;
	wxString valid_chars(wxT("-.0123456789"));
	size_t len = valid_chars.Length();
	for (size_t i = 0; i < len; i++)
		list.Add(wxString(valid_chars.GetChar(i)));

	mValidator->SetIncludes(list);
	this->SetBackgroundColour(parent->GetBackgroundColour());
	mSizer = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(mSizer);
}

basePanel::~basePanel()
{
	delete mValidator;
}
