#include "uiMain.h"
#include "ImplicitEssentialsOpenVDB/Operations.h"

wxBEGIN_EVENT_TABLE(uiMain, wxFrame)
	EVT_COMBOBOX(10001, onComboboxValueChanged)
	EVT_BUTTON(10002, onExecuteClick)
wxEND_EVENT_TABLE()

const long style = wxCAPTION | wxCLOSE_BOX;

uiMain::uiMain() : wxFrame(nullptr, wxID_ANY, "Implicit Modeling Demo", wxDefaultPosition, wxSize(400, 300), style)
{
	wxArrayString strings;
	strings.Add(wxT("Generate"));
	strings.Add(wxT("OffsetFill"));
	strings.Add(wxT("Boolean"));
	mFunctions = new wxComboBox(this, 10001, wxT("Generate"), wxPoint(10, 10), wxSize(150, 25), strings, wxCB_READONLY);
	mSizer = new wxBoxSizer(wxVERTICAL);
	mGeneratePanel = new generatePanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 180));
	mOffsetPanel = new offsetPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 180));
	mBooleanPanel = new booleanPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 180));
	mSizer->Add(mFunctions, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(mGeneratePanel, 0, wxEXPAND | wxALL, 5);
	mSizer->Add(mOffsetPanel, 0, wxEXPAND | wxALL, 5);
	mSizer->Add(mBooleanPanel, 0, wxEXPAND | wxALL, 5);
	mSizer->Hide(2);
	mSizer->Hide(3);
	this->SetSizer(mSizer);
	mSaveButton = new wxButton(this, 10002, "Execute");
	mSizer->Add(mSaveButton, 0, wxEXPAND | wxALL, 5);
}

uiMain::~uiMain()
{ 
}

void uiMain::onComboboxValueChanged(wxCommandEvent & event)
{
	auto currentValue = mFunctions->GetValue();
	if (currentValue == "Generate")
	{
		mSizer->Hide(3);
		mSizer->Hide(2);
		mSizer->Show(1, true);
	}
	else if (currentValue == "OffsetFill")
	{
		mSizer->Hide(3);
		mSizer->Hide(1);
		mSizer->Show(2, true);
	}
	else if (currentValue == "Boolean")
	{
		mSizer->Hide(2);
		mSizer->Hide(1);
		mSizer->Show(3, true);
	}

	mSizer->Layout();
	event.Skip();
}

void uiMain::onExecuteClick(wxCommandEvent & event)
{
	wxFileDialog
		saveFileDialog(this, _("Save VDB file"), "", "",
			"VDB files (*.vdb)|*.vdb", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	auto outPath = saveFileDialog.GetPath().ToStdString();
	if (mFunctions->GetValue() == "Generate")
	{
		float voxelSize(mGeneratePanel->getVoxelSize());
		int gridSize(mGeneratePanel->getGridSize());
		std::string latticeType(mGeneratePanel->getLatticeType());
		Operations::GenerateLattice(voxelSize, gridSize, latticeType, outPath);
	}
	else if (mFunctions->GetValue() == "OffsetFill")
	{
		std::string latticeType(mOffsetPanel->getLatticeType());
		float offset(mOffsetPanel->getOffsetValue());
		std::string inputPath(mOffsetPanel->getLatticeType());
		Operations::OffsetFill(offset, inputPath, latticeType, outPath);
	}
	else
	{

	}

	event.Skip();
}
