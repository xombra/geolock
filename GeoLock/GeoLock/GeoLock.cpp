// GeoLock.cpp : main project file.

#include "stdafx.h"
#include "GeoLock.h"

#using <system.dll>

using namespace GeoLock;

char *ip,*ct,*excludeExitNodes,*exitNodes;


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

void updateIP() {
	WebClient^ myWebClient = gcnew WebClient;
	Uri^ siteUri = gcnew Uri("http://automation.whatismyip.com/n09230945.asp");
	Stream^ ipStream = myWebClient->OpenRead(siteUri);
	StreamReader^ sr = gcnew StreamReader(ipStream);
	ip = (char*)(void*)
		Marshal::StringToHGlobalAnsi(sr->ReadToEnd());
	ipStream->Close();
	String^ ctUriString = "http://api.wipmania.com/" + char2StringRef(ip);
	Uri^ site2Uri = gcnew Uri(ctUriString);
	Stream^ ctStream = myWebClient->OpenRead(site2Uri);
	sr = gcnew StreamReader(ctStream);
	ct = (char*)(void*)
		Marshal::StringToHGlobalAnsi(sr->ReadToEnd());
	ctStream->Close();
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

	updateIP();

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
