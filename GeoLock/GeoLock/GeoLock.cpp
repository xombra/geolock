// GeoLock.cpp : main project file.

#include "stdafx.h"
#include "GeoLock.h"

using namespace GeoLock;

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
	const char* ip = (const char*)(void*)
		Marshal::StringToHGlobalAnsi(sr->ReadToEnd());
	ipStream->Close();

	// Create the main window and run it
	Application::Run(gcnew Form1());
	return 0;
}
