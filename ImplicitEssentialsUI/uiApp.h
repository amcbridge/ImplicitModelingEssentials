#pragma once
#include "wx/wx.h"
#include "uiMain.h"

class uiApp: public wxApp
{

	public:
		uiApp();
		~uiApp();
		virtual bool OnInit();
	private:
		uiMain* main;
};

