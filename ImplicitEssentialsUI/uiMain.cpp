#include "uiMain.h"
#include "ImplicitEssentialsOpenVDB/Operations.h"

wxBEGIN_EVENT_TABLE(uiMain, wxFrame)
	EVT_COMBOBOX(10001, onComboboxValueChanged)
	EVT_BUTTON(10002, onExecuteClick)
wxEND_EVENT_TABLE()

const long style = wxCAPTION | wxCLOSE_BOX;
const std::string windowTitle = "Implicit Modeling Demo";

uiMain::uiMain() : wxFrame(nullptr, wxID_ANY, windowTitle, wxDefaultPosition, wxSize(400, 300), style)
{
	wxArrayString strings;
	strings.Add(wxT("Generate"));
	strings.Add(wxT("OffsetFill"));
	strings.Add(wxT("Boolean"));
	mFunctions = new wxComboBox(this, 10001, wxT("Generate"), wxPoint(10, 10), wxSize(150, 25), strings, wxCB_READONLY);
	mSizer = new wxBoxSizer(wxVERTICAL);
	mGeneratePanel = new generatePanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 170));
	mOffsetPanel = new offsetPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 170));
	mBooleanPanel = new booleanPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 170));
	mSizer->Add(mFunctions, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 5);
	mSizer->Add(mGeneratePanel, 0, wxEXPAND | wxALL, 5);
	mSizer->Add(mOffsetPanel, 0, wxEXPAND | wxALL, 5);
	mSizer->Add(mBooleanPanel, 0, wxEXPAND | wxALL, 5);
	mSizer->Hide(2);
	mSizer->Hide(3);
	this->SetSizer(mSizer);
	mProgress = new wxGauge(this, wxID_ANY, 1, wxDefaultPosition, wxSize(150, 15), wxGA_HORIZONTAL);
	mSizer->Add(mProgress, 0, wxEXPAND | wxRIGHT | wxLEFT | wxBOTTOM, 5);
	mSaveButton = new wxButton(this, 10002, "Execute");
	mSizer->Add(mSaveButton, 0, wxEXPAND | wxALL, 5);
	Bind(wxEVT_THREAD, (wxObjectEventFunction)&uiMain::onThreadUpdate, this);
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
	wxFileDialog saveFileDialog(this, uiMain::getSaveWindowLabel(), "", "",
			uiMain::getSaveWindowFilter(), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;

	mOutPath.assign(saveFileDialog.GetPath().ToStdString());
	
	if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR)
	{
		wxMessageBox("Could not create the worker thread!", windowTitle, wxICON_ERROR);
		return;
	}

	if (GetThread()->Run() != wxTHREAD_NO_ERROR)
	{
		wxMessageBox("Could not run the worker thread!", windowTitle, wxICON_ERROR);
		return;
	}

	mProgress->Pulse();
	mSaveButton->Disable();
	event.Skip();
}

wxThread::ExitCode uiMain::Entry()
{
	try
	{
		if (mFunctions->GetValue() == "Generate")
		{
			float voxelSize(mGeneratePanel->getVoxelSize());
			int gridSize(mGeneratePanel->getGridSize());
			std::string latticeType(mGeneratePanel->getLatticeType());
			Operations::GenerateLattice(voxelSize, gridSize, latticeType, mOutPath);
		}
		else if (mFunctions->GetValue() == "OffsetFill")
		{
			std::string latticeType(mOffsetPanel->getLatticeType());
			float offset(mOffsetPanel->getOffsetValue());
			std::string inputPath(mOffsetPanel->getInputPath());
			Operations::OffsetFill(offset, inputPath, latticeType, mOutPath);
		}
		else
		{
			std::string inputAPath(mBooleanPanel->getInputAPath());
			std::string inputBPath(mBooleanPanel->getInputBPath());
			std::string type(mBooleanPanel->getOperationType());
			float t[]{ mBooleanPanel->getXTranslation(), mBooleanPanel->getYTranslation(), mBooleanPanel->getZTranslation() };
			Operations::Boolean(inputAPath, inputBPath, type, t, mOutPath);
		}
	}
	catch (const std::exception& e)
	{
		auto evt = new wxThreadEvent(wxEVT_THREAD);
		evt->SetInt(1);
		evt->SetString(e.what());
		QueueEvent(evt);
		return (wxThread::ExitCode)1;
	}

	auto evt = new wxThreadEvent(wxEVT_THREAD);
	evt->SetInt(0);
	QueueEvent(evt);
	return (wxThread::ExitCode)0;
}

void uiMain::onThreadUpdate(wxThreadEvent& evt)
{
	mProgress->SetValue(0);

	if (evt.GetInt() == 0)
	{
		wxMessageBox("Completed!", windowTitle, wxICON_INFORMATION);
	}
	else
	{
		wxMessageBox(evt.GetString(), windowTitle, wxICON_ERROR);
	}

	mSaveButton->Enable();
}

std::string uiMain::getSaveWindowLabel()
{
	return mFunctions->GetValue() == "OffsetFill" ? "Save OBJ file" : "Save VDB file";
}

std::string uiMain::getSaveWindowFilter()
{
	return mFunctions->GetValue() == "OffsetFill" ? "OBJ files (*.obj)|*.obj" : "VDB files (*.vdb)|*.vdb";
}
