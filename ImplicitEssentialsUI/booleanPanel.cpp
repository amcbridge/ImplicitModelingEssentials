#include "booleanPanel.h"

wxBEGIN_EVENT_TABLE(booleanPanel, wxPanel)
	EVT_BUTTON(10003, onSetInputAClick)
	EVT_BUTTON(10004, onSetInputBClick)
wxEND_EVENT_TABLE()

booleanPanel::booleanPanel(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size)
	: basePanel(parent, id, pos, size)
{
	auto fileLabel = new wxStaticText(this, wxID_ANY, "Input Files");
	mInputPathsSizer = new wxBoxSizer(wxHORIZONTAL);
	mLabelA = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxSize(50, 25), wxST_ELLIPSIZE_END);
	mLabelB = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxSize(50, 25), wxST_ELLIPSIZE_END);
	mSetInputAButton = new wxButton(this, 10003, "Set Input A");
	mSetInputBButton = new wxButton(this, 10004, "Set Input B");

	mInputPathsSizer->Add(mLabelA, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mInputPathsSizer->Add(mSetInputAButton, 1);
	mInputPathsSizer->Add(mLabelB, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mInputPathsSizer->Add(mSetInputBButton, 1);
	mSizer->Add(fileLabel, 0);
	mSizer->Add(mInputPathsSizer, 0, wxEXPAND | wxALL, 5);
	wxArrayString strings;
	strings.Add(wxT("Union"));
	strings.Add(wxT("Difference"));
	strings.Add(wxT("Intersection"));
	mOperationType = new wxComboBox(this, wxID_ANY, wxT("Union"), wxPoint(10, 10), wxSize(150, 25), strings, wxCB_READONLY);
	mSizer->Add(mOperationType, 0, wxEXPAND | wxALL, 5);
	auto translationLabel = new wxStaticText(this, wxID_ANY, "Translation");
	auto translationSizer = new wxBoxSizer(wxHORIZONTAL);
	auto xLabel = new wxStaticText(this, wxID_ANY, "X");
	auto yLabel = new wxStaticText(this, wxID_ANY, "Y");
	auto zLabel = new wxStaticText(this, wxID_ANY, "Z");
	mTranslateX = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, *mValidator);
	mTranslateY = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, *mValidator);
	mTranslateZ = new wxTextCtrl(this, wxID_ANY, "0", wxDefaultPosition, wxDefaultSize, 0, *mValidator);
	translationSizer->Add(xLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	translationSizer->Add(mTranslateX, 1, wxEXPAND | wxALL, 5);
	translationSizer->Add(yLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	translationSizer->Add(mTranslateY, 1, wxEXPAND | wxALL, 5);
	translationSizer->Add(zLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	translationSizer->Add(mTranslateZ, 1, wxEXPAND | wxALL, 5);
	mSizer->Add(translationLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(translationSizer, 0, wxEXPAND | wxALL, 5);
}

booleanPanel::~booleanPanel()
{
}

void booleanPanel::onSetInputAClick(wxCommandEvent & event)
{
	wxFileDialog
		openFileDialog(this, _("Open VDB file"), "", "",
			"VDB files (*.vdb)|*.vdb", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	mLabelA->SetLabelText(openFileDialog.GetFilename());
	mFilePathA.assign(openFileDialog.GetPath().ToStdString());
	mInputPathsSizer->Layout();

	event.Skip();
}

void booleanPanel::onSetInputBClick(wxCommandEvent & event)
{
	wxFileDialog
		openFileDialog(this, _("Open VDB file"), "", "",
			"VDB files (*.vdb)|*.vdb", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	mLabelB->SetLabelText(openFileDialog.GetFilename());
	mFilePathB.assign(openFileDialog.GetPath().ToStdString());
	mInputPathsSizer->Layout();

	event.Skip();
}
