#pragma once

char *ip,*ct;

System::String ^ char2StringRef( char * p){ 
	System::String ^ str; 
	str = gcnew System::String(p); 
	return str; 
}

System::String ^ getPrettyDate(SYSTEMTIME lt) {
	System::String ^tempD,^tempH,^tempM,^tempS;
	if (lt.wDay < 10) tempD = "0" + lt.wDay; else tempD = "" + lt.wDay;
	if (lt.wHour < 10) tempH = "0" + lt.wHour; else tempH = "" + lt.wHour;
	if (lt.wMinute < 10) tempM = "0" + lt.wMinute; else tempM = "" + lt.wMinute;
	if (lt.wSecond < 10) tempS = "0" + lt.wSecond; else tempS = "" + lt.wSecond;
	return lt.wMonth + "/" + tempD + "/" + lt.wYear + " - " + tempH + ":" + tempM + ":" + tempS;
}

void updateIP() {
	WebClient^ myWebClient = gcnew WebClient;
	Uri^ siteUri = gcnew Uri("http://automation.whatismyip.com/n09230945.asp");
	try {
		Stream^ ipStream = myWebClient->OpenRead(siteUri);
		StreamReader^ sr = gcnew StreamReader(ipStream);
		ip = (char*)(void*)
			Marshal::StringToHGlobalAnsi(sr->ReadToEnd());
		ipStream->Close();
	}
	catch (WebException ^ex) {
		MessageBox::Show("Tor is not running or is not properly configured","Connection Error");
		Application::Exit();
	}
	try {
		String^ ctUriString = "http://api.wipmania.com/" + char2StringRef(ip);
		Uri^ site2Uri = gcnew Uri(ctUriString);
		Stream^ ctStream = myWebClient->OpenRead(site2Uri);
		StreamReader^ sm = gcnew StreamReader(ctStream);
		ct = (char*)(void*)
			Marshal::StringToHGlobalAnsi(sm->ReadToEnd());
		ctStream->Close();
	}
	catch (WebException ^ex) {
		Application::Exit();
	}
}

namespace GeoLock {

	using namespace System;
	using namespace System::Net;
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
			String^ managedExclude = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
			String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
			if (managedExclude->Length > 0) this->excludeList->Text = L"Exclude: " + managedExclude;
			else this->excludeList->Text = L"Exclude: NONE";
			if (managedExit->Length > 0) this->preferNodes->Text = L"Prefer: " + managedExit;
			else this->preferNodes->Text = L"Prefer: NONE";
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
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
	private: System::Windows::Forms::ToolStripMenuItem^  settingsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  excludeExitNodesToolStripMenuItem;
	private: System::Windows::Forms::Label^  excludeList;
	private: System::Windows::Forms::Label^  preferNodes;
	private: System::Windows::Forms::Timer^  timer;
	private: System::Windows::Forms::Label^  timeStamp;
	private: System::Windows::Forms::PictureBox^  flagIcon;

	private: System::ComponentModel::IContainer^  components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->settingsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->excludeExitNodesToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->excludeList = (gcnew System::Windows::Forms::Label());
			this->preferNodes = (gcnew System::Windows::Forms::Label());
			this->timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->timeStamp = (gcnew System::Windows::Forms::Label());
			this->flagIcon = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->flagIcon))->BeginInit();
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
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(92, 22);
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
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->toolStripStatusLabel1, 
				this->toolStripStatusLabel2});
			this->statusStrip1->Location = System::Drawing::Point(0, 109);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(303, 22);
			this->statusStrip1->SizingGrip = false;
			this->statusStrip1->TabIndex = 1;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(22, 17);
			this->toolStripStatusLabel1->Text = L"IP: ";
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Size = System::Drawing::Size(76, 17);
			this->toolStripStatusLabel2->Text = L"Geolocation: ";
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
			this->preferNodes->Location = System::Drawing::Point(12, 56);
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
			this->timeStamp->Location = System::Drawing::Point(12, 80);
			this->timeStamp->Name = L"timeStamp";
			this->timeStamp->Size = System::Drawing::Size(77, 13);
			this->timeStamp->TabIndex = 4;
			this->timeStamp->Text = L"Last Updated: ";
			// 
			// flagIcon
			// 
			this->flagIcon->Location = System::Drawing::Point(282, 114);
			this->flagIcon->Name = L"flagIcon";
			this->flagIcon->Size = System::Drawing::Size(18, 12);
			this->flagIcon->TabIndex = 5;
			this->flagIcon->TabStop = false;
			// 
			// GeoLockWin
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(303, 131);
			this->Controls->Add(this->flagIcon);
			this->Controls->Add(this->timeStamp);
			this->Controls->Add(this->preferNodes);
			this->Controls->Add(this->excludeList);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MaximumSize = System::Drawing::Size(319, 169);
			this->MinimumSize = System::Drawing::Size(319, 169);
			this->Name = L"GeoLockWin";
			this->Text = L"GeoLock";
			this->Load += gcnew System::EventHandler(this, &GeoLockWin::GeoLockWin_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->flagIcon))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
				 Application::Exit();
			 }
	private: System::Void GeoLockWin_Load(System::Object^  sender, System::EventArgs^  e) {
				 System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(GeoLockWin::typeid));
				 updateIP();
				 SYSTEMTIME lt;
				 GetLocalTime(&lt);
				 this->timeStamp->Text = L"Last Updated: " + getPrettyDate(lt);
				 this->toolStripStatusLabel1->Text = L"IP: " + char2StringRef(ip);
				 this->toolStripStatusLabel2->Text = L"Geolocation: " + char2StringRef(ct);
				 this->flagIcon->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(char2StringRef(ct))));
			 }
	private: System::Void excludeExitNodesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 	 ExitNode^ exitNodeDialog = gcnew ExitNode();
				 try {
					 exitNodeDialog->ShowDialog(this);
				 }
				 finally {
					 delete exitNodeDialog;
					 String^ managedExclude = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
					 String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
					 if (managedExclude->Length > 0) this->excludeList->Text = L"Exclude: " + managedExclude;
					 else this->excludeList->Text = L"Exclude: NONE";
					 if (managedExit->Length > 0) this->preferNodes->Text = L"Prefer: " + managedExit;
					 else this->preferNodes->Text = L"Prefer: NONE";
				 }
			 }
};
}