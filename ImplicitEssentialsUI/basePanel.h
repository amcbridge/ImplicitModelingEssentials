#pragma once
#include "wx/wx.h"

class basePanel : public wxPanel
{
	public:
		basePanel(wxWindow * 	parent,
			wxWindowID 	id = wxID_ANY,
			const wxPoint & 	pos = wxDefaultPosition,
			const wxSize & 	size = wxDefaultSize);
		~basePanel();
	protected:
		wxTextValidator* mValidator;
		wxBoxSizer* mSizer;
};

