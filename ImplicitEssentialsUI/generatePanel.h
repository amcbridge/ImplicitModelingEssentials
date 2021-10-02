#pragma once
#include "wx/wx.h"
#include "latticePanel.h"

class generatePanel : public latticePanel
{
	public:
		generatePanel(wxWindow * 	parent,
			wxWindowID 	id = wxID_ANY,
			const wxPoint & 	pos = wxDefaultPosition,
			const wxSize & 	size = wxDefaultSize);
		~generatePanel();
		float getVoxelSize() const { return std::stof(mVoxelSize->GetValue().ToStdString()); }
		int getGridSize() const { return std::stoi(mGridSize->GetValue().ToStdString()); }
	private: 
		wxTextCtrl* mVoxelSize;
		wxTextCtrl* mGridSize;
};