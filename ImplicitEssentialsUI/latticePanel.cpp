#include "latticePanel.h"

latticePanel::latticePanel(wxWindow * parent, wxWindowID id, const wxPoint & pos, const wxSize & size)
	: basePanel(parent, id, pos, size)
{
	wxArrayString strings;
	strings.Add(wxT("Primitive"));
	strings.Add(wxT("Gyroid"));
	strings.Add(wxT("Diamond"));
	mLatticeType = new wxComboBox(this, 10001, wxT("Primitive"), wxPoint(10, 10), wxSize(150, 25), strings, wxCB_READONLY);
	auto label = new wxStaticText(this, wxID_ANY, "Lattice Type");
	mSizer->Add(label, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(mLatticeType, 0, wxEXPAND | wxALL, 5);
}

latticePanel::~latticePanel()
{
}
