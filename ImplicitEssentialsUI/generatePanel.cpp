#include "generatePanel.h"

generatePanel::generatePanel(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size)
	: latticePanel(parent, id, pos, size)
{
	auto voxelLabel = new wxStaticText(this, wxID_ANY, "Voxel Size");
	mVoxelSize = new wxTextCtrl(this, wxID_ANY, wxT("0.1"), wxDefaultPosition, wxDefaultSize, 0, *mValidator);
	auto gridLabel = new wxStaticText(this, wxID_ANY, "Grid Size");
	mGridSize = new wxTextCtrl(this, wxID_ANY, wxT("100"), wxDefaultPosition, wxDefaultSize, 0, *mValidator);
	mSizer->Add(voxelLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(mVoxelSize, 0, wxEXPAND | wxALL, 5);
	mSizer->Add(gridLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(mGridSize, 0, wxEXPAND | wxALL, 5);
}

generatePanel::~generatePanel()
{
}
