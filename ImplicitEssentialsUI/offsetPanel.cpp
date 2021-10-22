#include "offsetPanel.h"

wxBEGIN_EVENT_TABLE(offsetPanel, wxPanel)
	EVT_BUTTON(10005, onSetInputClick)
wxEND_EVENT_TABLE()

offsetPanel::offsetPanel(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size)
	: latticePanel(parent, id, pos, size)
{
	mInputSizer = new wxBoxSizer(wxHORIZONTAL);
	mSetInputButton = new wxButton(this, 10005, "Set Input");
	mPathLabel = new wxStaticText(this, wxID_ANY, "");
	mInputSizer->Add(mPathLabel, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	mInputSizer->Add(mSetInputButton, 1);
	auto labelInput = new wxStaticText(this, wxID_ANY, "Input Model");
	mSizer->Insert(0, labelInput, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Insert(1, mInputSizer, 0);
	auto label = new wxStaticText(this, wxID_ANY, "Offset Value");
	mOffsetSize = new wxTextCtrl(this, wxID_ANY, "1", wxDefaultPosition, wxDefaultSize, 0, *mValidator);
	mSizer->Add(label, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(mOffsetSize, 0, wxEXPAND | wxALL, 5);
}

offsetPanel::~offsetPanel()
{
}

void offsetPanel::onSetInputClick(wxCommandEvent & event)
{
	wxFileDialog
		openFileDialog(this, _("Open VDB file"), "", "",
			"VDB files (*.vdb)|*.vdb", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;

	mPathLabel->SetLabelText(openFileDialog.GetFilename());
	mInputPath.assign(openFileDialog.GetPath().ToStdString());
	mInputSizer->Layout();
	event.Skip();
}
