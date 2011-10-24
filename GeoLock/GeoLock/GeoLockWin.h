#pragma once

/*
if Visual Studio decides to remove timer Interval or complain about it in design mode, here's what it should be at compile
this->timer->Interval = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["updateFreq"])*60*1000);
*/

//bool to keep track of if IP is acceptable or not
bool acceptance = true;

//function to convert char arrays/strings into managed System::String
System::String ^ char2StringRef( char * p){ 
	System::String ^ str; 
	str = gcnew System::String(p); 
	return str; 
}

//clean up HTML output to obtain only IP address
System::String^ getIP(System::String^ in) {
	int index = in->IndexOf('<');
	return in->Substring(0,index);
}

//clean up HTML output to obtain only country code
System::String^ getCountry(System::String^ in) {
	int index = in->IndexOf('>');
	return in->Substring(index+1,2);
}

//clean up date (add extra 0s, colons, forward slashes)
System::String ^ getPrettyDate(SYSTEMTIME lt) {
	System::String ^tempD,^tempH,^tempM,^tempS;
	if (lt.wDay < 10) tempD = "0" + lt.wDay; else tempD = "" + lt.wDay;
	if (lt.wHour < 10) tempH = "0" + lt.wHour; else tempH = "" + lt.wHour;
	if (lt.wMinute < 10) tempM = "0" + lt.wMinute; else tempM = "" + lt.wMinute;
	if (lt.wSecond < 10) tempS = "0" + lt.wSecond; else tempS = "" + lt.wSecond;
	return lt.wMonth + "/" + tempD + "/" + lt.wYear + " - " + tempH + ":" + tempM + ":" + tempS;
}

//open telnet connection to localhost to tell Tor to update connections
bool getNewIdentity() {
	String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
	bool adv = (advOut == "true");
	//load control port from app.config
	int controlPort = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["controlPort"]));
	if (adv) {
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		Console::WriteLine("[" + getPrettyDate(lt) + "]: Establishing telnet connection to 127.0.0.1:" + controlPort + "...");
	}
	//build connection
	TcpClient^ tcpSocket;
	tcpSocket = gcnew TcpClient();
	IPEndPoint^ serverEP = gcnew IPEndPoint(IPAddress::Parse("127.0.0.1"),controlPort);
	NetworkStream^ netStream;
	try {
		tcpSocket->Connect(serverEP);
		netStream = tcpSocket->GetStream();
		if (netStream->CanWrite) {
			ASCIIEncoding^ encoder = gcnew ASCIIEncoding();
			//telnet command "AUTHENTICATE" - must not be using Tor authentication
			array<Byte>^ sendBytes = encoder->GetBytes("AUTHENTICATE\r\n");
			netStream->Write( sendBytes, 0, sendBytes->Length );
			if (adv) Console::WriteLine("\ttelnet@localhost: AUTHENTICATE");
			//telnet command to create new connections (drop old)
			sendBytes = encoder->GetBytes("signal NEWNYM\r\n");
			netStream->Write( sendBytes, 0, sendBytes->Length );
			if (adv) Console::WriteLine("\ttelnet@localhost: signal NEWNYM");
		}
		if (adv) Console::Write("\tClosing telnet connection...");
		netStream->Close();
		tcpSocket->Close();
		if (adv) Console::WriteLine("Complete\n\tIdentity switch request sent to Tor.");
	}
	catch (Exception^ ex) {
		if (adv) {
			HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			GetConsoleScreenBufferInfo(hstdout,&csbi);
			SetConsoleTextAttribute(hstdout,0x0C);
			Console::WriteLine("FAIL");
			SetConsoleTextAttribute(hstdout,csbi.wAttributes);
		}
		return false;
	}
	return true;
}

//function to verify if country code is acceptable based on user's settings
System::String^ getAcceptState(String^ ip,String^ ct,String^ hostname) {
	//load excluded and preferred nodes from app.config
	String^ managedExcluded = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
	String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
	//load blockedIPs and blockedHosts from app.config
	String^ managedBadIP = System::Configuration::ConfigurationManager::AppSettings["blockedIP"];
	String^ managedBadHost = System::Configuration::ConfigurationManager::AppSettings["blockedHost"];
	//convert comma delimited list into array
	array<String^>^ excludedList = managedExcluded->Split(',');
	array<String^>^ exitList = managedExit->Split(',');
	array<String^>^ badIPList = managedBadIP->Split(',');
	array<String^>^ badHostList = managedBadHost->Split(',');
	//if current country is anything on excluded, immediately fail
	for (int i=0;i<excludedList->Length;i++) if (ct == excludedList[i]) return L"Not Locked";
	//if current IP is blocked, immediately fail
	for (int i=0;i<badIPList->Length;i++) if (ip == badIPList[i]) return L"Not Locked";
	//if current hostname is blocked, immediately fail
	for (int i=0;i<badHostList->Length;i++) if (hostname == badHostList[i]) return L"Not Locked";
	//check preferred nodes (if necessary)
	if (exitList->Length > 1) {
		bool acceptable = false;
		//verify that current country is preferred, if not, immediately fail
		for (int i=0;i<exitList->Length;i++) if (ct == exitList[i]) acceptable = true;
		if (!acceptable) return L"Not Locked";
	}
	//country has been verified as being acceptable, return success
	return L"Locked";
}

namespace GeoLock {

	using namespace System;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::Runtime::InteropServices;

	public ref class GeoLockWin : public System::Windows::Forms::Form
	{
	public:
		GeoLockWin(void)
		{
			String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
			if (advOut == "true") AllocConsole();
			InitializeComponent();
			reinitialize();
		}

		//function to update IP address via wipmania's API and return either error or stream input
		void updateIP() {
			String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
			bool adv = (advOut == "true");
			//load logging boolean
			WebClient^ myWebClient = gcnew WebClient;
			//random number must be appended to URL to ensure IP is current
			//as wipmania's API does not have correct refresh information
			srand(time(NULL));
			String^ in = "";
			int random = rand()%1000 + 1;
			String^ ipURL = "http://api.wipmania.com/?" + random;
			Uri^ siteUri = gcnew Uri(ipURL);
			try {
				//attempt to open connection (via Tor proxy specified in app.config) to wipmania
				myWebClient->DownloadStringCompleted += gcnew DownloadStringCompletedEventHandler(this,&GeoLock::GeoLockWin::downloadStringEventComplete);
				myWebClient->DownloadStringAsync(siteUri);
			}
			catch (WebException ^ex) {
				if (adv) {
					HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
					CONSOLE_SCREEN_BUFFER_INFO csbi;
					GetConsoleScreenBufferInfo(hstdout,&csbi);
					SetConsoleTextAttribute(hstdout,0x0C);
					Console::WriteLine("FAIL");
					SetConsoleTextAttribute(hstdout,csbi.wAttributes);
				}
			}
		}

		void downloadStringEventComplete(Object^ /*sender*/, DownloadStringCompletedEventArgs^ e) {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
			bool adv = (advOut == "true");
			String^ logging = System::Configuration::ConfigurationManager::AppSettings["logging"];
			bool log = (logging == "true");
			if (e->Error == nullptr) {
				String^ ipFull = dynamic_cast<String^>(e->Result);
				String ^ip,^ct;
				//if IP was obtainable
				if (ipFull != "") {
					//convert HTML formatted IP and country code to useable elements
					ip = getIP(ipFull);
					ct = getCountry(ipFull);
					if (adv) {
						Console::WriteLine("Complete");
						Console::WriteLine("\tIP: " + ip);
						Console::WriteLine("\tGeo: " + ct);
					}
				}
				else {
					//if IP was not obtainable, display ?'s
					ip = "??.??.??.??";
					ct = "??";
					if (adv) {
						HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
						CONSOLE_SCREEN_BUFFER_INFO csbi;
						GetConsoleScreenBufferInfo(hstdout,&csbi);
						SetConsoleTextAttribute(hstdout,0x0C);
						Console::WriteLine("FAIL");
						SetConsoleTextAttribute(hstdout,csbi.wAttributes);
					}
				}
				//update IP and Country
				this->toolStripLabel1->Text = L"IP: " + ip;
				this->toolStripLabel2->Text = ct;
				//set notification icon and tooltip
				this->notifyIcon1->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject("$this.Icon")));
				this->notifyIcon1->Text = ip + " | " + ct;
				//load hostLookup boolean
				String^ host = System::Configuration::ConfigurationManager::AppSettings["hostLookup"];
				//attempt to resolve IP address to hostname if specified
				String^ hostString = "";
				if (host == "true") {
					try {
						IPHostEntry^ host;
						host = Dns::GetHostEntry(ip);
						this->toolStripButton1->Text = host->HostName;
						if (adv) Console::WriteLine("\tHost: " + host->HostName);
						this->notifyIcon1->Text += "\n" + host->HostName;
						hostString = host->HostName;
					}
					catch (Exception^ ex) {
						this->toolStripButton1->Text = "???";
						if (adv) Console::WriteLine("\tHost: unknown");
						this->notifyIcon1->Text += "\n???";
					}
				}
				else {
					this->toolStripButton1->Text = "---";
				}
				//get acceptance state
				String^ acceptState = getAcceptState(ip,ct,hostString);
				//set flag icon
				if (ipFull != "") this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(ct)));
				//set acceptance icon and tooltip
				this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(acceptState)));
				this->toolStripButton2->Text = acceptState;
				if (log) {
					StreamWriter^ pwriter = gcnew StreamWriter("Identity_Log.txt",true);
					SYSTEMTIME lt;
					GetLocalTime(&lt);
					pwriter->WriteLine("[IP_Update @" + getPrettyDate(lt) + "] - " + ip + "|" + ct);
					pwriter->Close();
				}
				updateTorIcon(true);
				HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO csbi;
				GetConsoleScreenBufferInfo(hstdout,&csbi);
				if ((acceptState) == "Locked") {
					//IP address updated and it is acceptable
					if (adv) {
						Console::Write("\tState: ");
						SetConsoleTextAttribute(hstdout,0x0A);
						Console::WriteLine("GOOD");
						SetConsoleTextAttribute(hstdout,csbi.wAttributes);
					}
				}
				else {
					//or it was updated and is not acceptable
					if (adv) Console::Write("\tState: ");
					SetConsoleTextAttribute(hstdout,0x0C);
					Console::WriteLine("BAD");
					SetConsoleTextAttribute(hstdout,csbi.wAttributes);
					if(!getNewIdentity()) {
						this->torStatusIcon->Text = resources->GetString(L"Comm");
						this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
					}
					updateIPandDisplay();
				}
			}
			else {
				updateTorIcon(false);
				if (adv) {
					HANDLE hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
					CONSOLE_SCREEN_BUFFER_INFO csbi;
					GetConsoleScreenBufferInfo(hstdout,&csbi);
					SetConsoleTextAttribute(hstdout,0x0C);
					Console::WriteLine("FAIL");
					SetConsoleTextAttribute(hstdout,csbi.wAttributes);
				}
			}
			this->progressBar1->Visible = false;
		}

		//function to check Tor configuration/connectivity and update the status icon thusly
		void updateTorIcon(bool up) {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			if (!up) {
				this->torStatusIcon->Text = resources->GetString(L"Err");
				this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("fatal")));
			}
			else {
				switch (checkTorrc()) {
				case 0:
					this->torStatusIcon->Text = resources->GetString(L"TorOK");
					this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("good")));
					break;
				case 1:
					this->torStatusIcon->Text = resources->GetString(L"Torpw");
					this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
					break;
				case 2:
					this->torStatusIcon->Text = resources->GetString(L"Torcookie");
					this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
					break;
				case 99:
					this->torStatusIcon->Text = resources->GetString(L"Unknown");
					this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
					break;
				}
			}
		}

		void reinitialize() {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			//load excluded and preferred nodes from app.config
			String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
			bool adv = (advOut == "true");
			String^ managedExclude = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
			String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
			String^ managedBadIP = System::Configuration::ConfigurationManager::AppSettings["blockedIP"];
			String^ managedBadHost = System::Configuration::ConfigurationManager::AppSettings["blockedHost"];
			String^ stayOnTop = System::Configuration::ConfigurationManager::AppSettings["persist"];
			String^ taskbar = System::Configuration::ConfigurationManager::AppSettings["taskbar"];
			//update their corresponding visual elements
			if (adv) {
				SYSTEMTIME lt;
				GetLocalTime(&lt);
				Console::Write("[" + getPrettyDate(lt) + "]: Loading excluded nodes...");
			}
			if (managedExclude->Length > 0) {
				resources->ApplyResources(this->excludeList, L"excludeList");
				this->excludeList->Text += " " + managedExclude;
			}
			else {
				resources->ApplyResources(this->excludeList, L"excludeList");
				this->excludeList->Text += " -";
			}
			if (adv) Console::WriteLine("Complete");
			if (adv) {
				SYSTEMTIME lt;
				GetLocalTime(&lt);
				Console::Write("[" + getPrettyDate(lt) + "]: Loading preferred nodes...");
			}
			if (managedExit->Length > 0) {
				resources->ApplyResources(this->preferNodes, L"preferNodes");
				this->preferNodes->Text += " " + managedExit;
			}
			else {
				resources->ApplyResources(this->preferNodes, L"preferNodes");
				this->preferNodes->Text += " -";
			}
			if (adv) Console::WriteLine("Complete");
			if (adv) {
				SYSTEMTIME lt;
				GetLocalTime(&lt);
				Console::Write("[" + getPrettyDate(lt) + "]: Loading Blocked IPs...");
			}
			if (managedBadIP->Length > 0) {
				resources->ApplyResources(this->blockedIP, L"blockedIP");
				this->blockedIP->Text += " " + managedBadIP;
			}
			else {
				resources->ApplyResources(this->blockedIP, L"blockedIP");
				this->blockedIP->Text += " -";
			}
			if (adv) Console::WriteLine("Complete");
			if (adv) {
				SYSTEMTIME lt;
				GetLocalTime(&lt);
				Console::Write("[" + getPrettyDate(lt) + "]: Loading Blocked hostnames...");
			}
			if (managedBadHost->Length > 0) {
				resources->ApplyResources(this->blockedHost, L"blockedHost");
				this->blockedHost->Text += " " + managedBadHost;
			}
			else {
				resources->ApplyResources(this->blockedHost, L"blockedHost");
				this->blockedHost->Text += " -";
			}
			if (adv) Console::WriteLine("Complete");
			if (stayOnTop == "true") this->TopMost = true;
			else this->TopMost = false;
			if (taskbar == "true") this->notifyIcon1->Visible = true;
			else this->notifyIcon1->Visible = false;
			int op = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["opacity"]));
			double opacity = op/100.0;
			this->Opacity = opacity;
		}

		//backend function to check Tor configuration/connectivity 
		int checkTorrc() {
			//load %appdata% environment variable
			char* pPath = std::getenv("appdata");
			String^ appdata = char2StringRef(pPath) + "\\Vidalia\\torrc";
			StreamReader^ sr = gcnew StreamReader(appdata);
			try {
				try {
					String^ line;
					//check "torrc" for password or cookie authentication (which would cause "signal NEWNYM" to fail)
					while (line = sr->ReadLine()) {
						if (line->StartsWith("HashedControlPassword")) return 1;
						if (line->StartsWith("CookieAuthentication 1")) return 2;
					}
				}
				finally {
					if (sr) delete (IDisposable^)sr;
				}
			}
			catch (Exception^ ex) {
				//return unknown error
				return 99;
			}
			//return status of OK
			return 0;
		}

		//function to call backend updateIP() and update the visual elements accordingly
		void updateIPandDisplay() {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
			bool adv = (advOut == "true");
			if (adv) {
				SYSTEMTIME lt;
				GetLocalTime(&lt);
				Console::Write("[" + getPrettyDate(lt) + "]: Attempting to Update IP...");
			}
			//call backend updateIP() function and capture HTML formatted IP address
			//set progress bar to visible so user knows their is background process running
			this->progressBar1->Visible = true;
			updateIP();
			//get current system time to display when the IP was last updated
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			resources->ApplyResources(this->timeStamp, L"timeStamp");
			this->timeStamp->Text += getPrettyDate(lt);
		}

	protected:
		~GeoLockWin()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  excludeExitNodesToolStripMenuItem;
	private: System::Windows::Forms::Label^  excludeList;
	private: System::Windows::Forms::Label^  preferNodes;
	private: System::Windows::Forms::Timer^  timer;
	private: System::Windows::Forms::Label^  timeStamp;
	private: System::Windows::Forms::ToolStrip^  toolStrip1;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel1;
	private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;
	private: System::Windows::Forms::ToolStripLabel^  toolStripLabel2;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton1;
	private: System::Windows::Forms::ToolStripButton^  toolStripButton2;
	private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer1;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape1;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape2;
	private: System::Windows::Forms::ToolStripMenuItem^  forceUpdateToolStripMenuItem;
	private: System::Windows::Forms::ToolStripButton^  torStatusIcon;
	private: System::Windows::Forms::NotifyIcon^  notifyIcon1;
	private: System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem1;
	private: System::Windows::Forms::ToolStripMenuItem^  toolStripMenuItem2;
	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  blockedIP;
	private: System::Windows::Forms::Label^  blockedHost;
	private: System::Windows::Forms::ToolStripMenuItem^  addCurrentIP;
	private: System::Windows::Forms::ToolStripMenuItem^  addCurrentHost;
	private: System::Windows::Forms::ToolStripMenuItem^  blockCurrentIPn;
	private: System::Windows::Forms::ToolStripMenuItem^  blockCurrentHostn;
	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->forceUpdateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addCurrentIP = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->addCurrentHost = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->excludeExitNodesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->excludeList = (gcnew System::Windows::Forms::Label());
			this->preferNodes = (gcnew System::Windows::Forms::Label());
			this->timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->timeStamp = (gcnew System::Windows::Forms::Label());
			this->toolStrip1 = (gcnew System::Windows::Forms::ToolStrip());
			this->toolStripLabel1 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->toolStripLabel2 = (gcnew System::Windows::Forms::ToolStripLabel());
			this->toolStripButton1 = (gcnew System::Windows::Forms::ToolStripButton());
			this->toolStripButton2 = (gcnew System::Windows::Forms::ToolStripButton());
			this->torStatusIcon = (gcnew System::Windows::Forms::ToolStripButton());
			this->shapeContainer1 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->notifyIcon1 = (gcnew System::Windows::Forms::NotifyIcon(this->components));
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->toolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blockCurrentIPn = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->blockCurrentHostn = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->blockedIP = (gcnew System::Windows::Forms::Label());
			this->blockedHost = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			resources->ApplyResources(this->menuStrip1, L"menuStrip1");
			this->menuStrip1->BackColor = System::Drawing::SystemColors::Control;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->settingsToolStripMenuItem});
			this->menuStrip1->Name = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			resources->ApplyResources(this->fileToolStripMenuItem, L"fileToolStripMenuItem");
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->forceUpdateToolStripMenuItem, 
				this->addCurrentIP, this->addCurrentHost, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			// 
			// forceUpdateToolStripMenuItem
			// 
			resources->ApplyResources(this->forceUpdateToolStripMenuItem, L"forceUpdateToolStripMenuItem");
			this->forceUpdateToolStripMenuItem->Name = L"forceUpdateToolStripMenuItem";
			this->forceUpdateToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeoLockWin::forceUpdateToolStripMenuItem_Click);
			// 
			// addCurrentIP
			// 
			resources->ApplyResources(this->addCurrentIP, L"addCurrentIP");
			this->addCurrentIP->Name = L"addCurrentIP";
			this->addCurrentIP->Click += gcnew System::EventHandler(this, &GeoLockWin::addCurrentIP_Click);
			// 
			// addCurrentHost
			// 
			resources->ApplyResources(this->addCurrentHost, L"addCurrentHost");
			this->addCurrentHost->Name = L"addCurrentHost";
			this->addCurrentHost->Click += gcnew System::EventHandler(this, &GeoLockWin::addCurrentHost_Click);
			// 
			// exitToolStripMenuItem
			// 
			resources->ApplyResources(this->exitToolStripMenuItem, L"exitToolStripMenuItem");
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeoLockWin::exitToolStripMenuItem_Click);
			// 
			// settingsToolStripMenuItem
			// 
			resources->ApplyResources(this->settingsToolStripMenuItem, L"settingsToolStripMenuItem");
			this->settingsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->excludeExitNodesToolStripMenuItem});
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			// 
			// excludeExitNodesToolStripMenuItem
			// 
			resources->ApplyResources(this->excludeExitNodesToolStripMenuItem, L"excludeExitNodesToolStripMenuItem");
			this->excludeExitNodesToolStripMenuItem->Name = L"excludeExitNodesToolStripMenuItem";
			this->excludeExitNodesToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeoLockWin::excludeExitNodesToolStripMenuItem_Click);
			// 
			// excludeList
			// 
			resources->ApplyResources(this->excludeList, L"excludeList");
			this->excludeList->AutoEllipsis = true;
			this->excludeList->Name = L"excludeList";
			// 
			// preferNodes
			// 
			resources->ApplyResources(this->preferNodes, L"preferNodes");
			this->preferNodes->AutoEllipsis = true;
			this->preferNodes->Name = L"preferNodes";
			// 
			// timer
			// 
			this->timer->Enabled = true;
			this->timer->Interval = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["updateFreq"])*60*1000);
			this->timer->Tick += gcnew System::EventHandler(this, &GeoLockWin::GeoLockWin_Load);
			// 
			// timeStamp
			// 
			resources->ApplyResources(this->timeStamp, L"timeStamp");
			this->timeStamp->Name = L"timeStamp";
			// 
			// toolStrip1
			// 
			resources->ApplyResources(this->toolStrip1, L"toolStrip1");
			this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->toolStripLabel1, 
				this->toolStripSeparator1, this->toolStripLabel2, this->toolStripButton1, this->toolStripButton2, this->torStatusIcon});
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			// 
			// toolStripLabel1
			// 
			resources->ApplyResources(this->toolStripLabel1, L"toolStripLabel1");
			this->toolStripLabel1->Name = L"toolStripLabel1";
			// 
			// toolStripSeparator1
			// 
			resources->ApplyResources(this->toolStripSeparator1, L"toolStripSeparator1");
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			// 
			// toolStripLabel2
			// 
			resources->ApplyResources(this->toolStripLabel2, L"toolStripLabel2");
			this->toolStripLabel2->Name = L"toolStripLabel2";
			// 
			// toolStripButton1
			// 
			resources->ApplyResources(this->toolStripButton1, L"toolStripButton1");
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Name = L"toolStripButton1";
			// 
			// toolStripButton2
			// 
			resources->ApplyResources(this->toolStripButton2, L"toolStripButton2");
			this->toolStripButton2->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton2->Name = L"toolStripButton2";
			// 
			// torStatusIcon
			// 
			resources->ApplyResources(this->torStatusIcon, L"torStatusIcon");
			this->torStatusIcon->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->torStatusIcon->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->torStatusIcon->Name = L"torStatusIcon";
			// 
			// shapeContainer1
			// 
			resources->ApplyResources(this->shapeContainer1, L"shapeContainer1");
			this->shapeContainer1->Name = L"shapeContainer1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(2) {this->lineShape2, 
				this->lineShape1});
			this->shapeContainer1->TabStop = false;
			// 
			// lineShape2
			// 
			resources->ApplyResources(this->lineShape2, L"lineShape2");
			this->lineShape2->Name = L"lineShape2";
			// 
			// lineShape1
			// 
			resources->ApplyResources(this->lineShape1, L"lineShape1");
			this->lineShape1->Name = L"lineShape1";
			// 
			// notifyIcon1
			// 
			resources->ApplyResources(this->notifyIcon1, L"notifyIcon1");
			this->notifyIcon1->ContextMenuStrip = this->contextMenuStrip1;
			this->notifyIcon1->DoubleClick += gcnew System::EventHandler(this, &GeoLockWin::notifyIcon1_DoubleClick);
			// 
			// contextMenuStrip1
			// 
			resources->ApplyResources(this->contextMenuStrip1, L"contextMenuStrip1");
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->toolStripMenuItem1, 
				this->blockCurrentIPn, this->blockCurrentHostn, this->toolStripMenuItem2});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			// 
			// toolStripMenuItem1
			// 
			resources->ApplyResources(this->toolStripMenuItem1, L"toolStripMenuItem1");
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &GeoLockWin::forceUpdateToolStripMenuItem_Click);
			// 
			// blockCurrentIPn
			// 
			resources->ApplyResources(this->blockCurrentIPn, L"blockCurrentIPn");
			this->blockCurrentIPn->Name = L"blockCurrentIPn";
			this->blockCurrentIPn->Click += gcnew System::EventHandler(this, &GeoLockWin::addCurrentIP_Click);
			// 
			// blockCurrentHostn
			// 
			resources->ApplyResources(this->blockCurrentHostn, L"blockCurrentHostn");
			this->blockCurrentHostn->Name = L"blockCurrentHostn";
			this->blockCurrentHostn->Click += gcnew System::EventHandler(this, &GeoLockWin::addCurrentHost_Click);
			// 
			// toolStripMenuItem2
			// 
			resources->ApplyResources(this->toolStripMenuItem2, L"toolStripMenuItem2");
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &GeoLockWin::exitToolStripMenuItem_Click);
			// 
			// progressBar1
			// 
			resources->ApplyResources(this->progressBar1, L"progressBar1");
			this->progressBar1->BackColor = System::Drawing::SystemColors::Control;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Marquee;
			// 
			// blockedIP
			// 
			resources->ApplyResources(this->blockedIP, L"blockedIP");
			this->blockedIP->AutoEllipsis = true;
			this->blockedIP->Name = L"blockedIP";
			// 
			// blockedHost
			// 
			resources->ApplyResources(this->blockedHost, L"blockedHost");
			this->blockedHost->AutoEllipsis = true;
			this->blockedHost->Name = L"blockedHost";
			// 
			// GeoLockWin
			// 
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->preferNodes);
			this->Controls->Add(this->blockedHost);
			this->Controls->Add(this->blockedIP);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->timeStamp);
			this->Controls->Add(this->excludeList);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->shapeContainer1);
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->Name = L"GeoLockWin";
			this->Load += gcnew System::EventHandler(this, &GeoLockWin::GeoLockWin_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->toolStrip1->ResumeLayout(false);
			this->toolStrip1->PerformLayout();
			this->contextMenuStrip1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 Application::Exit();
			 }
	private: System::Void GeoLockWin_Load(System::Object^  sender, System::EventArgs^  e) {
				 //on application startup and timer expiration, force new identity if specified in user settings
				 String^ forceUpdate = System::Configuration::ConfigurationManager::AppSettings["forceUpdate"];
				 if (forceUpdate == "true") {
					 if(!getNewIdentity()) {
						 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
						 this->torStatusIcon->Text = resources->GetString(L"Comm");
						 this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
					 }
				 }
				 updateIPandDisplay();
			 }
	private: System::Void excludeExitNodesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //launch user settings window
			 	 ExitNode^ exitNodeDialog = gcnew ExitNode();
				 try {
					 exitNodeDialog->ShowDialog(this);
				 }
				 finally {
					 delete exitNodeDialog;
					 //update display elements
					 reinitialize();
					 //update timer tick if necessary
					 this->timer->Interval = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["updateFreq"])*60*1000);
					 //update the IP address and check if it is acceptable and if not, get new identity (this ensures that settings take effect immediately)
					 updateIPandDisplay();
				 }
			 }
	private: System::Void forceUpdateToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				//when Force Update is selected or F5 pressed, get a new identity immediately
				if(!getNewIdentity()) {
					System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
					this->torStatusIcon->Text = resources->GetString(L"Comm");
					this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
				}
				updateIPandDisplay();
		     }
	private: System::Void notifyIcon1_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
			    //restore window from minimized state when notification icon is double clicked
				this->WindowState = System::Windows::Forms::FormWindowState::Normal;
			}
	private: System::Void addCurrentIP_Click(System::Object^  sender, System::EventArgs^  e) {
				System::Configuration::Configuration ^config = 
					System::Configuration::ConfigurationManager::OpenExeConfiguration(System::Configuration::ConfigurationUserLevel::None);
				String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
				bool adv = (advOut == "true");
				if (adv) {
					SYSTEMTIME lt;
					GetLocalTime(&lt);
					Console::Write("[" + getPrettyDate(lt) + "]: Blocking current IP address...");
				}
				String^ managedBadIP = System::Configuration::ConfigurationManager::AppSettings["blockedIP"];
				if (managedBadIP->Length > 0) managedBadIP += "," + (this->toolStripLabel1->Text)->Substring(3)->Trim();
				else  managedBadIP += (this->toolStripLabel1->Text)->Substring(3)->Trim();
				//write blockedIPs
				config->AppSettings->Settings->Remove("blockedIP");
				config->AppSettings->Settings->Add("blockedIP",managedBadIP);
				config->Save(System::Configuration::ConfigurationSaveMode::Modified);
				System::Configuration::ConfigurationManager::RefreshSection("appSettings");
				if (adv) {
					Console::WriteLine("Complete");
				}
				reinitialize();
				updateIPandDisplay();
			}
	private: System::Void addCurrentHost_Click(System::Object^  sender, System::EventArgs^  e) {
				System::Configuration::Configuration ^config = 
					System::Configuration::ConfigurationManager::OpenExeConfiguration(System::Configuration::ConfigurationUserLevel::None);
				String^ advOut = System::Configuration::ConfigurationManager::AppSettings["advancedOutput"];
				bool adv = (advOut == "true");
				if (adv) {
					SYSTEMTIME lt;
					GetLocalTime(&lt);
					Console::Write("[" + getPrettyDate(lt) + "]: Blocking current hostname...");
				}
				String^ managedBadHost = System::Configuration::ConfigurationManager::AppSettings["blockedHost"];
				if (managedBadHost->Length > 0) managedBadHost += "," + this->toolStripButton1->Text;
				else managedBadHost += this->toolStripButton1->Text;
				//write blockedHosts
				config->AppSettings->Settings->Remove("blockedHost");
				config->AppSettings->Settings->Add("blockedHost",managedBadHost);
				config->Save(System::Configuration::ConfigurationSaveMode::Modified);
				System::Configuration::ConfigurationManager::RefreshSection("appSettings");
				if (adv) {
					Console::WriteLine("Complete");
				}
				reinitialize();
				updateIPandDisplay();
			}
};
}