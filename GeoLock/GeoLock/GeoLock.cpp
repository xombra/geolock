// GeoLock.cpp : main project file.
#include "stdafx.h"
#include "GeoLock.h"

#using <system.dll>

using namespace GeoLock;

void writeRegKey(LPCSTR location,LPCSTR key,char* value) {
	HKEY hKey = 0; char* buf = value;
	DWORD dwType = REG_SZ;
	/*if location exists*/
	if (RegOpenKeyA(HKEY_CURRENT_USER,location,&hKey) == ERROR_SUCCESS) {
		/*and if key exists*/
		RegSetValueExA(hKey,key,0,dwType, (BYTE*)buf, 1024);
		RegCloseKey(hKey);
	}
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
