#include "uiApp.h"

wxIMPLEMENT_APP(uiApp);

uiApp::uiApp()
{
}

uiApp::~uiApp()
{
}

bool uiApp::OnInit()
{
	main = new uiMain();
	main->Show();
	main->CenterOnScreen();
	return true;
}
