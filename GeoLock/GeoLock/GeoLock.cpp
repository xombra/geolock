// GeoLock.cpp : main project file.
#include "stdafx.h"
#include "ExitNode.h"
#include "GeoLockWin.h"

using namespace GeoLock;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Create the main window and run it
	Application::Run(gcnew GeoLockWin());
	return 0;
}
