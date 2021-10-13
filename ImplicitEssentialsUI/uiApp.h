#pragma once
#include "wx/wx.h"
#include "uiMain.h"
#include <openvdb/openvdb.h>

class uiApp: public wxApp
{
	public:
		uiApp();
		~uiApp();
		virtual bool OnInit();
	private:
		uiMain* main;
};

