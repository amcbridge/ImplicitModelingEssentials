#pragma once
#include "basePanel.h"

class latticePanel : public basePanel
{
	public:
		latticePanel(wxWindow * 	parent,
			wxWindowID 	id = wxID_ANY,
			const wxPoint & 	pos = wxDefaultPosition,
			const wxSize & 	size = wxDefaultSize);
		~latticePanel();
		const std::string getLatticeType() const { return mLatticeType->GetValue().ToStdString(); }
	protected:
		wxComboBox* mLatticeType;
};

