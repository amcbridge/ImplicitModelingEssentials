#pragma once
#include "latticePanel.h"
class offsetPanel : public latticePanel
{
	public:
		offsetPanel(wxWindow * 	parent,
			wxWindowID 	id = wxID_ANY,
			const wxPoint & 	pos = wxDefaultPosition,
			const wxSize & 	size = wxDefaultSize);
		~offsetPanel();
		const std::string& getInputPath() const { return mInputPath; }
		float getOffsetValue() const { return std::stof(mOffsetSize->GetValue().ToStdString()); }
		wxDECLARE_EVENT_TABLE();
	private:
		void onSetInputClick(wxCommandEvent& event);

		wxTextCtrl* mOffsetSize;
		wxButton* mSetInputButton;
		std::string mInputPath;
		wxBoxSizer* mInputSizer;
		wxStaticText* mPathLabel;
};

