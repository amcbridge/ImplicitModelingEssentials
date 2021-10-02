#pragma once
#include "wx/wx.h"
#include "generatePanel.h"
#include "offsetPanel.h"
#include "booleanPanel.h"

class uiMain : public wxFrame
{
	public:
		uiMain();
		~uiMain();
		wxDECLARE_EVENT_TABLE();
	private:
		void onComboboxValueChanged(wxCommandEvent& event);
		void onExecuteClick(wxCommandEvent& event);

		generatePanel* mGeneratePanel;
		offsetPanel* mOffsetPanel;
		booleanPanel* mBooleanPanel;
		wxComboBox* mFunctions;
		wxButton* mButton;
		wxBoxSizer* mSizer;
		wxButton* mSaveButton;
};

