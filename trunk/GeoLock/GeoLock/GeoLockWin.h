#pragma once

/*
if Visual Studio decides to remove timer Interval or complain about it in design mode, here's what it should be at compile
this->timer->Interval = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["updateFreq"])*60*1000);
*/

//connectivity boolean (if IP is obtainable)
bool torUP = false;

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
void getNewIdentity() {
	//load control port from app.config
	int controlPort = (System::Int32::Parse(System::Configuration::ConfigurationManager::AppSettings["controlPort"]));
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
			//telnet command to create new connections (drop old)
			sendBytes = encoder->GetBytes("signal NEWNYM\r\n");
			netStream->Write( sendBytes, 0, sendBytes->Length );
		}
		netStream->Close();
		tcpSocket->Close();
	}
	catch (Exception^ ex) {
		MessageBox::Show("GeoLock was unable to connect to Tor. Please check your configuration.","Error");
	}
}

//function to update IP address via wipmania's API and return either error or stream input
System::String^ updateIP() {
	WebClient^ myWebClient = gcnew WebClient;
	//random number must be appended to URL to ensure IP is current
	//as wipmania's API does not have correct refresh information
	srand(time(NULL));
	int random = rand()%1000 + 1;
	String^ ipURL = "http://api.wipmania.com/?" + random;
	String^ in = "";
	Uri^ siteUri = gcnew Uri(ipURL);
	try {
		//attempt to open connection (via Tor) to wipmania
		Stream^ ipStream = myWebClient->OpenRead(siteUri);
		StreamReader^ sr = gcnew StreamReader(ipStream);
		in = sr->ReadToEnd();
		ipStream->Close();
		torUP = true;
		//return HTML formatted IP string similar to:
		//XXX.XXX.XXX.XXX<br>US
		return in;
	}
	catch (WebException ^ex) {
		//set connectivity boolean and return error message
		MessageBox::Show("Tor is not running or is not properly configured.","Connection Error");
		torUP = false;
	}
	return "ERROR";
}

//function to verify if country code is acceptable based on user's settings
System::String^ getAcceptState(String^ ct) {
	//load excluded and preferred nodes from app.config
	String^ managedExcluded = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
	String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
	//convert comma delimited list into array
	array<String^>^ excludedList = managedExcluded->Split(',');
	array<String^>^ exitList = managedExit->Split(',');
	//if current country is anything on excluded, immediately fail
	for (int i=0;i<excludedList->Length;i++) if (ct == excludedList[i]) return L"Not Locked";
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

	public ref class GeoLockWin : public System::Windows::Forms::Form
	{
	public:
		GeoLockWin(void)
		{
			InitializeComponent();
			reinitialize();
		}

		//function to check Tor configuration/connectivity and update the status icon thusly
		void updateTorIcon() {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			switch (checkTorrc()) {
			case 0:
				this->torStatusIcon->Text = "Tor OK";
				this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("good")));
				break;
			case 1:
				this->torStatusIcon->Text = "Error: Tor is using a password!";
				this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
				break;
			case 2:
				this->torStatusIcon->Text = "Error: Tor is using cookie authentication!";
				this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
				break;
			case 3:
				this->torStatusIcon->Text = "Error: Tor is not running or is not properly configured.";
				this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("fatal")));
				break;
			case 99:
				this->torStatusIcon->Text = "Error: Unknown";
				this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject("error")));
				break;
			}
		}

		void reinitialize() {
			//load excluded and preferred nodes from app.config
			String^ managedExclude = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
			String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
			String^ stayOnTop = System::Configuration::ConfigurationManager::AppSettings["persist"];
			String^ taskbar = System::Configuration::ConfigurationManager::AppSettings["taskbar"];
			//update their corresponding visual elements
			if (managedExclude->Length > 0) this->excludeList->Text = L"Exclude: " + managedExclude;
			else this->excludeList->Text = L"Exclude: NONE";
			if (managedExit->Length > 0) this->preferNodes->Text = L"Prefer: " + managedExit;
			else this->preferNodes->Text = L"Prefer: NONE";
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
			//if Tor isn't up, return error
			if (!torUP) return 3;
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
		bool updateIPandDisplay() {
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			//call backend updateIP() function and capture HTML formatted IP address
			String^ ipFull = updateIP(); String ^ip,^ct;
			//if IP was obtainable
			if (ipFull != "ERROR") {
				//convert HTML formatted IP and country code to useable elements
				ip = getIP(ipFull);
				ct = getCountry(ipFull);
				//since IP was obtainable, connectivity is assured
				torUP = true;
			}
			else {
				//if IP was not obtainable, display ?'s
				ip = "??.??.??.??";
				ct = "??";
				//assume no connectivity
				torUP = false;
			}
			updateTorIcon();
			//get current system time to display when the IP was last updated
			SYSTEMTIME lt;
			GetLocalTime(&lt);
			this->timeStamp->Text = L"Last Updated: " + getPrettyDate(lt);
			this->toolStripLabel1->Text = L"IP: " + ip;
			this->toolStripLabel2->Text = ct;
			//set flag icon
			String^ acceptState = getAcceptState(ct);
			//attempt to resolve IP address to hostname
			IPHostEntry^ host;
			try {
				host = Dns::GetHostEntry(ip);
				this->toolStripButton1->Text = host->HostName;
			}
			catch (Exception^ ex) {
				this->toolStripButton1->Text = "unknown";
			}
			if (ipFull != "ERROR") this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(ct)));
			this->notifyIcon1->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(ct + "1")));
			this->notifyIcon1->Text = ip + " | " + ct + "\n" + host->HostName;
			//set acceptance icon
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(acceptState)));
			this->toolStripButton2->Text = acceptState;
			//IP address updated and it is acceptable
			if (acceptState == "Locked") return true;
			//or it was updated and is not acceptable
			else return false;
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
	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->forceUpdateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
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
			this->toolStripMenuItem2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->toolStrip1->SuspendLayout();
			this->contextMenuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->BackColor = System::Drawing::SystemColors::Control;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->settingsToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(303, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->forceUpdateToolStripMenuItem, 
				this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// forceUpdateToolStripMenuItem
			// 
			this->forceUpdateToolStripMenuItem->Name = L"forceUpdateToolStripMenuItem";
			this->forceUpdateToolStripMenuItem->ShortcutKeyDisplayString = L"F5";
			this->forceUpdateToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F5;
			this->forceUpdateToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->forceUpdateToolStripMenuItem->Text = L"Force Update";
			this->forceUpdateToolStripMenuItem->ToolTipText = L"Forces a new identity";
			this->forceUpdateToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeoLockWin::forceUpdateToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->ShortcutKeyDisplayString = L"Alt+F4";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeoLockWin::exitToolStripMenuItem_Click);
			// 
			// settingsToolStripMenuItem
			// 
			this->settingsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->excludeExitNodesToolStripMenuItem});
			this->settingsToolStripMenuItem->Name = L"settingsToolStripMenuItem";
			this->settingsToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->settingsToolStripMenuItem->Text = L"Settings";
			// 
			// excludeExitNodesToolStripMenuItem
			// 
			this->excludeExitNodesToolStripMenuItem->Name = L"excludeExitNodesToolStripMenuItem";
			this->excludeExitNodesToolStripMenuItem->Size = System::Drawing::Size(156, 22);
			this->excludeExitNodesToolStripMenuItem->Text = L"Configuration...";
			this->excludeExitNodesToolStripMenuItem->Click += gcnew System::EventHandler(this, &GeoLockWin::excludeExitNodesToolStripMenuItem_Click);
			// 
			// excludeList
			// 
			this->excludeList->AutoEllipsis = true;
			this->excludeList->Location = System::Drawing::Point(12, 32);
			this->excludeList->Name = L"excludeList";
			this->excludeList->Size = System::Drawing::Size(279, 13);
			this->excludeList->TabIndex = 2;
			this->excludeList->Text = L"Exclude: ";
			// 
			// preferNodes
			// 
			this->preferNodes->AutoEllipsis = true;
			this->preferNodes->Location = System::Drawing::Point(12, 58);
			this->preferNodes->Name = L"preferNodes";
			this->preferNodes->Size = System::Drawing::Size(279, 13);
			this->preferNodes->TabIndex = 3;
			this->preferNodes->Text = L"Prefer:";
			// 
			// timer
			// 
			this->timer->Enabled = true;
			this->timer->Interval = 300000;
			this->timer->Tick += gcnew System::EventHandler(this, &GeoLockWin::GeoLockWin_Load);
			// 
			// timeStamp
			// 
			this->timeStamp->AutoSize = true;
			this->timeStamp->Location = System::Drawing::Point(12, 82);
			this->timeStamp->Name = L"timeStamp";
			this->timeStamp->Size = System::Drawing::Size(77, 13);
			this->timeStamp->TabIndex = 4;
			this->timeStamp->Text = L"Last Updated: ";
			// 
			// toolStrip1
			// 
			this->toolStrip1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->toolStrip1->GripStyle = System::Windows::Forms::ToolStripGripStyle::Hidden;
			this->toolStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {this->toolStripLabel1, 
				this->toolStripSeparator1, this->toolStripLabel2, this->toolStripButton1, this->toolStripButton2, this->torStatusIcon});
			this->toolStrip1->Location = System::Drawing::Point(0, 106);
			this->toolStrip1->Name = L"toolStrip1";
			this->toolStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::Professional;
			this->toolStrip1->Size = System::Drawing::Size(303, 25);
			this->toolStrip1->TabIndex = 6;
			this->toolStrip1->Text = L"toolStrip1";
			// 
			// toolStripLabel1
			// 
			this->toolStripLabel1->Name = L"toolStripLabel1";
			this->toolStripLabel1->Size = System::Drawing::Size(22, 22);
			this->toolStripLabel1->Text = L"IP: ";
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(6, 25);
			// 
			// toolStripLabel2
			// 
			this->toolStripLabel2->Name = L"toolStripLabel2";
			this->toolStripLabel2->Size = System::Drawing::Size(0, 22);
			// 
			// toolStripButton1
			// 
			this->toolStripButton1->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton1.Image")));
			this->toolStripButton1->ImageScaling = System::Windows::Forms::ToolStripItemImageScaling::None;
			this->toolStripButton1->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton1->Name = L"toolStripButton1";
			this->toolStripButton1->Size = System::Drawing::Size(23, 22);
			// 
			// toolStripButton2
			// 
			this->toolStripButton2->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->toolStripButton2->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->toolStripButton2->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"toolStripButton2.Image")));
			this->toolStripButton2->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->toolStripButton2->Name = L"toolStripButton2";
			this->toolStripButton2->Size = System::Drawing::Size(23, 22);
			this->toolStripButton2->Text = L"toolStripButton2";
			// 
			// torStatusIcon
			// 
			this->torStatusIcon->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
			this->torStatusIcon->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
			this->torStatusIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"torStatusIcon.Image")));
			this->torStatusIcon->ImageTransparentColor = System::Drawing::Color::Magenta;
			this->torStatusIcon->Name = L"torStatusIcon";
			this->torStatusIcon->Size = System::Drawing::Size(23, 22);
			this->torStatusIcon->Text = L"toolStripButton3";
			// 
			// shapeContainer1
			// 
			this->shapeContainer1->Location = System::Drawing::Point(0, 0);
			this->shapeContainer1->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer1->Name = L"shapeContainer1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(2) {this->lineShape2, 
				this->lineShape1});
			this->shapeContainer1->Size = System::Drawing::Size(303, 131);
			this->shapeContainer1->TabIndex = 7;
			this->shapeContainer1->TabStop = false;
			// 
			// lineShape2
			// 
			this->lineShape2->BorderColor = System::Drawing::SystemColors::ActiveCaption;
			this->lineShape2->Name = L"lineShape2";
			this->lineShape2->X1 = -1;
			this->lineShape2->X2 = 303;
			this->lineShape2->Y1 = 24;
			this->lineShape2->Y2 = 24;
			// 
			// lineShape1
			// 
			this->lineShape1->BorderColor = System::Drawing::SystemColors::ActiveCaption;
			this->lineShape1->Name = L"lineShape1";
			this->lineShape1->X1 = -1;
			this->lineShape1->X2 = 303;
			this->lineShape1->Y1 = 105;
			this->lineShape1->Y2 = 105;
			// 
			// notifyIcon1
			// 
			this->notifyIcon1->ContextMenuStrip = this->contextMenuStrip1;
			this->notifyIcon1->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"notifyIcon1.Icon")));
			this->notifyIcon1->Text = L"GeoLock";
			this->notifyIcon1->Visible = true;
			this->notifyIcon1->DoubleClick += gcnew System::EventHandler(this, &GeoLockWin::notifyIcon1_DoubleClick);
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripMenuItem1, 
				this->toolStripMenuItem2});
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(144, 48);
			// 
			// toolStripMenuItem1
			// 
			this->toolStripMenuItem1->Name = L"toolStripMenuItem1";
			this->toolStripMenuItem1->Size = System::Drawing::Size(143, 22);
			this->toolStripMenuItem1->Text = L"Force Update";
			this->toolStripMenuItem1->Click += gcnew System::EventHandler(this, &GeoLockWin::toolStripMenuItem1_Click);
			// 
			// toolStripMenuItem2
			// 
			this->toolStripMenuItem2->Name = L"toolStripMenuItem2";
			this->toolStripMenuItem2->Size = System::Drawing::Size(143, 22);
			this->toolStripMenuItem2->Text = L"Exit";
			this->toolStripMenuItem2->Click += gcnew System::EventHandler(this, &GeoLockWin::toolStripMenuItem2_Click);
			// 
			// GeoLockWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(303, 131);
			this->Controls->Add(this->toolStrip1);
			this->Controls->Add(this->timeStamp);
			this->Controls->Add(this->preferNodes);
			this->Controls->Add(this->excludeList);
			this->Controls->Add(this->menuStrip1);
			this->Controls->Add(this->shapeContainer1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip1;
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(319, 169);
			this->MinimumSize = System::Drawing::Size(319, 169);
			this->Name = L"GeoLockWin";
			this->Text = L"GeoLock";
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
				 if (forceUpdate == "true") getNewIdentity();
				 //check if IP address is acceptable
				 bool acceptable = updateIPandDisplay();
				 //if not keep switching identities until it is
				 while (!acceptable) {
					 getNewIdentity();
					 acceptable = updateIPandDisplay();
				 }
			 }
	private: System::Void excludeExitNodesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 //launch user settings window
			 	 ExitNode^ exitNodeDialog = gcnew ExitNode();
				 try {
					 exitNodeDialog->ShowDialog(this);
				 }
				 finally {
					 delete exitNodeDialog;
					 reinitialize();
					 //update the IP address and check if it is acceptable and if not, get new identity (this ensures that settings take effect immediately)
					 bool acceptable = updateIPandDisplay();
					 while (!acceptable) {
						 getNewIdentity();
						 acceptable = updateIPandDisplay();
					 }
				 }
			 }
	private: System::Void forceUpdateToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
					//when Force Update is selected or F5 pressed, get a new identity immediately
					getNewIdentity();
					bool acceptable = updateIPandDisplay();
					while (!acceptable) {
						getNewIdentity();
						acceptable = updateIPandDisplay();
					}
		     }
private: System::Void toolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e) {
			 		//when Force Update is selected from context menu, get a new identity immediately
					getNewIdentity();
					bool acceptable = updateIPandDisplay();
					while (!acceptable) {
						getNewIdentity();
						acceptable = updateIPandDisplay();
					}
		 }
private: System::Void toolStripMenuItem2_Click(System::Object^  sender, System::EventArgs^  e) {
			 Application::Exit();
		 }
private: System::Void notifyIcon1_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
			 this->WindowState = System::Windows::Forms::FormWindowState::Normal;
		 }
};
}