// GeoLock.cpp : main project file.
#include "stdafx.h"
#include "GeoLock.h"

#using <system.dll>

using namespace GeoLock;

void writeRegKey(LPCSTR location,LPCSTR key,char* value) {
	HKEY hKey = 0; char buf[255] = {0};
	DWORD dwType = REG_SZ; DWORD dwBufSize = sizeof(buf);
	/*if location exists*/
	if (RegOpenKeyA(HKEY_CURRENT_USER,location,&hKey) == ERROR_SUCCESS) {
		/*and if key exists*/
		RegSetValueExA(hKey,key,0,dwType, (BYTE*)buf, dwBufSize);
		RegCloseKey(hKey);
	}
}

String^ checkRegKey(LPCSTR location,LPCSTR key) {
	String^ test; char *temp;
	HKEY hKey = 0; char buf[255] = {0};
	DWORD dwType = REG_SZ; DWORD dwBufSize = sizeof(buf);
	/*if location exists*/
	if (RegOpenKeyA(HKEY_CURRENT_USER,location,&hKey) == ERROR_SUCCESS) {
		/*and if key exists*/
		if (RegQueryValueExA(hKey,key,0,&dwType, (BYTE*)buf, &dwBufSize) == ERROR_SUCCESS) {
			temp = &buf[0];
			test = gcnew System::String(temp,0,sizeof(buf));
			for(int i=0;i<sizeof(buf);i++) printf(&buf[i]);
		}
		else MessageBox::Show("A registry key is missing\nYou should reinstall GeoLock","Alert");
		RegCloseKey(hKey);
	}
	else MessageBox::Show("A registry key is missing\nYou should reinstall GeoLock","Alert");
	return test;
}

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	String^ managedExclude = checkRegKey("Software\\GeoLock","excludedExitNodes");
	String^ managedExit = checkRegKey("Software\\GeoLock","exitNodes");
	excludeExitNodes = (char*)(void*)
		Marshal::StringToHGlobalAnsi(managedExclude);
	exitNodes = (char*)(void*)
		Marshal::StringToHGlobalAnsi(managedExit);

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
