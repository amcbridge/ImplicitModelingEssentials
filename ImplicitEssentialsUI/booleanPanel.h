#pragma once
#include "basePanel.h"

class booleanPanel : public basePanel
{
	public:
		booleanPanel(wxWindow * 	parent,
			wxWindowID 	id = wxID_ANY,
			const wxPoint & 	pos = wxDefaultPosition,
			const wxSize & 	size = wxDefaultSize);
		~booleanPanel();
		const std::string& getInputAPath() const { return mFilePathA; }
		const std::string& getInputBPath() const { return mFilePathB; }
		const std::string getOperationType() const { return mOperationType->GetValue().ToStdString(); }
		float getXTranslation() const { return std::stof(mTranslateX->GetValue().ToStdString()); }
		float getYTranslation() const { return std::stof(mTranslateY->GetValue().ToStdString()); }
		float getZTranslation() const { return std::stof(mTranslateZ->GetValue().ToStdString()); }
		wxDECLARE_EVENT_TABLE();
	private:
		void onSetInputAClick(wxCommandEvent& event);
		void onSetInputBClick(wxCommandEvent& event);

		wxTextCtrl* mTranslateX;
		wxTextCtrl* mTranslateY;
		wxTextCtrl* mTranslateZ;
		wxButton* mSetInputAButton;
		wxButton* mSetInputBButton;
		wxComboBox* mOperationType;
		std::string mFilePathA;
		std::string mFilePathB;
		wxBoxSizer* mInputPathsSizer;
		wxStaticText* mLabelA;
		wxStaticText* mLabelB;
};

