#pragma once
#include "wx/wx.h"
#include "generatePanel.h"
#include "offsetPanel.h"
#include "booleanPanel.h"

class uiMain : public wxFrame, public wxThreadHelper
{
	public:
		uiMain();
		~uiMain();
	protected:
		virtual wxThread::ExitCode Entry() override;
	private:
		void onThreadUpdate(wxThreadEvent& evt);
		wxDECLARE_EVENT_TABLE();
		void onComboboxValueChanged(wxCommandEvent& event);
		void onExecuteClick(wxCommandEvent& event);
		std::string getSaveWindowLabel();
		std::string getSaveWindowFilter();

		generatePanel* mGeneratePanel;
		offsetPanel* mOffsetPanel;
		booleanPanel* mBooleanPanel;
		wxComboBox* mFunctions;
		wxButton* mButton;
		wxBoxSizer* mSizer;
		wxButton* mSaveButton;
		std::string mOutPath; 
		wxGauge* mProgress;
};

