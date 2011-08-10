// GeoLock.cpp : main project file.

#include "stdafx.h"
#include "GeoLock.h"

using namespace GeoLock;

char* ip; char* ct;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

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

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
