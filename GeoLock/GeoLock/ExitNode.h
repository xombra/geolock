#pragma once

namespace GeoLock {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class ExitNode : public System::Windows::Forms::Form
	{
	public:
		ExitNode(void)
		{
			InitializeComponent();
			String^ managedExcluded = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
			String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
			String^ updateFreq = System::Configuration::ConfigurationManager::AppSettings["updateFreq"];
			String^ controlPortI = System::Configuration::ConfigurationManager::AppSettings["controlPort"];

			array<String^>^ excludedList = managedExcluded->Split(',');
			if (excludedList[0] != "") {
				for(int i=0;i<excludedList->Length;i++) {
					int index = this->excludedNodes->FindString(excludedList[i]);
					if (index != -1) this->excludedNodes->SetItemChecked(index,true);
				}
			}
			array<String^>^ exitList = managedExit->Split(',');
			if (exitList[0] != "") {
				for(int i=0;i<exitList->Length;i++) {
					int index = this->preferredNodes->FindString(exitList[i]);
					if (index != -1) this->preferredNodes->SetItemChecked(index,true);
				}
			}
			this->updateIn->Text = updateFreq;
			this->controlPort->Text = controlPortI;
		}

	protected:
		~ExitNode()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::CheckedListBox^  excludedNodes;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabPage1;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::Label^  label1;
	private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer1;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::CheckedListBox^  preferredNodes;
	private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer2;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape2;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  okButton;
	private: System::Windows::Forms::Button^  cancelButton;
	private: System::Windows::Forms::Button^  clearAll;
	private: System::Windows::Forms::Button^  selectAll;
	private: System::Windows::Forms::Button^  SelectAllPre;
	private: System::Windows::Forms::Button^  ClearAllPre;
	private: System::Windows::Forms::TabPage^  tabPage3;
	private: System::Windows::Forms::Label^  label5;
	private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer3;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape3;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  updateIn;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape4;
	private: System::Windows::Forms::TextBox^  controlPort;

	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Button^  defaultButton;
	private: System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ExitNode::typeid));
			this->excludedNodes = (gcnew System::Windows::Forms::CheckedListBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabPage1 = (gcnew System::Windows::Forms::TabPage());
			this->clearAll = (gcnew System::Windows::Forms::Button());
			this->selectAll = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->shapeContainer1 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->tabPage2 = (gcnew System::Windows::Forms::TabPage());
			this->ClearAllPre = (gcnew System::Windows::Forms::Button());
			this->SelectAllPre = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->preferredNodes = (gcnew System::Windows::Forms::CheckedListBox());
			this->shapeContainer2 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->defaultButton = (gcnew System::Windows::Forms::Button());
			this->controlPort = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->updateIn = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->shapeContainer3 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->okButton = (gcnew System::Windows::Forms::Button());
			this->cancelButton = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			this->SuspendLayout();
			// 
			// excludedNodes
			// 
			this->excludedNodes->CheckOnClick = true;
			this->excludedNodes->FormattingEnabled = true;
			this->excludedNodes->Items->AddRange(gcnew cli::array< System::Object^  >(240) {L"AC", L"AD", L"AE", L"AF", L"AG", L"AI", L"AL", 
				L"AM", L"AN", L"AO", L"AP", L"AQ", L"AR", L"AS", L"AT", L"AU", L"AW", L"AX", L"AZ", L"BA", L"BB", L"BD", L"BE", L"BF", L"BG", 
				L"BH", L"BI", L"BJ", L"BM", L"BN", L"BO", L"BR", L"BS", L"BT", L"BV", L"BW", L"BY", L"BZ", L"CA", L"CD", L"CF", L"CG", L"CH", 
				L"CI", L"CK", L"CL", L"CM", L"CN", L"CO", L"CR", L"CU", L"CV", L"CX", L"CY", L"CZ", L"DE", L"DJ", L"DK", L"DM", L"DO", L"DZ", 
				L"EC", L"EE", L"EG", L"ER", L"ES", L"ET", L"EU", L"FI", L"FJ", L"FK", L"FM", L"FO", L"FR", L"GA", L"GB", L"GD", L"GE", L"GF", 
				L"GH", L"GI", L"GL", L"GM", L"GN", L"GP", L"GQ", L"GR", L"GT", L"GU", L"GW", L"GY", L"HK", L"HM", L"HN", L"HR", L"HT", L"HU", 
				L"ID", L"IE", L"IL", L"IM", L"IN", L"IO", L"IQ", L"IR", L"IS", L"IT", L"JE", L"JM", L"JO", L"JP", L"KE", L"KG", L"KH", L"KI", 
				L"KM", L"KN", L"KP", L"KR", L"KW", L"KY", L"KZ", L"LA", L"LB", L"LC", L"LI", L"LK", L"LR", L"LS", L"LT", L"LU", L"LV", L"LY", 
				L"MA", L"MC", L"MD", L"ME", L"MG", L"MH", L"MK", L"ML", L"MM", L"MN", L"MO", L"MP", L"MQ", L"MR", L"MS", L"MT", L"MU", L"MV", 
				L"MW", L"MX", L"MY", L"MZ", L"NA", L"NC", L"NE", L"NF", L"NG", L"NI", L"NL", L"NO", L"NP", L"NR", L"NU", L"NZ", L"OM", L"PA", 
				L"PE", L"PF", L"PG", L"PH", L"PK", L"PL", L"PM", L"PR", L"PS", L"PT", L"PW", L"PY", L"QA", L"RE", L"RO", L"RS", L"RU", L"RW", 
				L"SA", L"SB", L"SC", L"SD", L"SE", L"SG", L"SI", L"SK", L"SL", L"SM", L"SN", L"SO", L"SR", L"ST", L"SV", L"SY", L"SZ", L"TC", 
				L"TD", L"TG", L"TH", L"TJ", L"TK", L"TL", L"TM", L"TN", L"TO", L"TR", L"TT", L"TV", L"TW", L"TZ", L"UA", L"UG", L"UM", L"US", 
				L"UY", L"UZ", L"VA", L"VC", L"VE", L"VG", L"VI", L"VN", L"VU", L"WF", L"WS", L"XX", L"YE", L"YT", L"ZA", L"ZM", L"ZW"});
			this->excludedNodes->Location = System::Drawing::Point(21, 26);
			this->excludedNodes->Name = L"excludedNodes";
			this->excludedNodes->Size = System::Drawing::Size(65, 244);
			this->excludedNodes->TabIndex = 0;
			// 
			// tabControl1
			// 
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Location = System::Drawing::Point(6, 7);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(276, 310);
			this->tabControl1->TabIndex = 4;
			// 
			// tabPage1
			// 
			this->tabPage1->Controls->Add(this->clearAll);
			this->tabPage1->Controls->Add(this->selectAll);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->excludedNodes);
			this->tabPage1->Controls->Add(this->shapeContainer1);
			this->tabPage1->Location = System::Drawing::Point(4, 22);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->Padding = System::Windows::Forms::Padding(3);
			this->tabPage1->Size = System::Drawing::Size(268, 284);
			this->tabPage1->TabIndex = 0;
			this->tabPage1->Text = L"Excluded Nodes";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// clearAll
			// 
			this->clearAll->Location = System::Drawing::Point(107, 229);
			this->clearAll->Name = L"clearAll";
			this->clearAll->Size = System::Drawing::Size(75, 23);
			this->clearAll->TabIndex = 5;
			this->clearAll->Text = L"Clear All";
			this->clearAll->UseVisualStyleBackColor = true;
			this->clearAll->Click += gcnew System::EventHandler(this, &ExitNode::clearAll_Click);
			// 
			// selectAll
			// 
			this->selectAll->Location = System::Drawing::Point(107, 200);
			this->selectAll->Name = L"selectAll";
			this->selectAll->Size = System::Drawing::Size(75, 23);
			this->selectAll->TabIndex = 4;
			this->selectAll->Text = L"Select All";
			this->selectAll->UseVisualStyleBackColor = true;
			this->selectAll->Click += gcnew System::EventHandler(this, &ExitNode::selectAll_Click);
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(107, 26);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(143, 244);
			this->label2->TabIndex = 3;
			this->label2->Text = resources->GetString(L"label2.Text");
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 7);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(98, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Excluded Countries";
			// 
			// shapeContainer1
			// 
			this->shapeContainer1->Location = System::Drawing::Point(3, 3);
			this->shapeContainer1->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer1->Name = L"shapeContainer1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(1) {this->lineShape1});
			this->shapeContainer1->Size = System::Drawing::Size(262, 278);
			this->shapeContainer1->TabIndex = 2;
			this->shapeContainer1->TabStop = false;
			// 
			// lineShape1
			// 
			this->lineShape1->BorderColor = System::Drawing::SystemColors::Highlight;
			this->lineShape1->Name = L"lineShape1";
			this->lineShape1->X1 = 104;
			this->lineShape1->X2 = 258;
			this->lineShape1->Y1 = 11;
			this->lineShape1->Y2 = 11;
			// 
			// tabPage2
			// 
			this->tabPage2->Controls->Add(this->ClearAllPre);
			this->tabPage2->Controls->Add(this->SelectAllPre);
			this->tabPage2->Controls->Add(this->label4);
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->preferredNodes);
			this->tabPage2->Controls->Add(this->shapeContainer2);
			this->tabPage2->Location = System::Drawing::Point(4, 22);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->Padding = System::Windows::Forms::Padding(3);
			this->tabPage2->Size = System::Drawing::Size(268, 284);
			this->tabPage2->TabIndex = 1;
			this->tabPage2->Text = L"Preferred Nodes";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// ClearAllPre
			// 
			this->ClearAllPre->Enabled = false;
			this->ClearAllPre->Location = System::Drawing::Point(107, 229);
			this->ClearAllPre->Name = L"ClearAllPre";
			this->ClearAllPre->Size = System::Drawing::Size(75, 23);
			this->ClearAllPre->TabIndex = 6;
			this->ClearAllPre->Text = L"Clear All";
			this->ClearAllPre->UseVisualStyleBackColor = true;
			this->ClearAllPre->Click += gcnew System::EventHandler(this, &ExitNode::ClearAllPre_Click);
			// 
			// SelectAllPre
			// 
			this->SelectAllPre->Enabled = false;
			this->SelectAllPre->Location = System::Drawing::Point(107, 200);
			this->SelectAllPre->Name = L"SelectAllPre";
			this->SelectAllPre->Size = System::Drawing::Size(75, 23);
			this->SelectAllPre->TabIndex = 5;
			this->SelectAllPre->Text = L"Select All";
			this->SelectAllPre->UseVisualStyleBackColor = true;
			this->SelectAllPre->Click += gcnew System::EventHandler(this, &ExitNode::SelectAllPre_Click);
			// 
			// label4
			// 
			this->label4->Location = System::Drawing::Point(107, 26);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(143, 167);
			this->label4->TabIndex = 4;
			this->label4->Text = resources->GetString(L"label4.Text");
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(7, 7);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(97, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Preferred Countries";
			// 
			// preferredNodes
			// 
			this->preferredNodes->CheckOnClick = true;
			this->preferredNodes->Enabled = false;
			this->preferredNodes->FormattingEnabled = true;
			this->preferredNodes->Items->AddRange(gcnew cli::array< System::Object^  >(240) {L"AC", L"AD", L"AE", L"AF", L"AG", L"AI", 
				L"AL", L"AM", L"AN", L"AO", L"AP", L"AQ", L"AR", L"AS", L"AT", L"AU", L"AW", L"AX", L"AZ", L"BA", L"BB", L"BD", L"BE", L"BF", 
				L"BG", L"BH", L"BI", L"BJ", L"BM", L"BN", L"BO", L"BR", L"BS", L"BT", L"BV", L"BW", L"BY", L"BZ", L"CA", L"CD", L"CF", L"CG", 
				L"CH", L"CI", L"CK", L"CL", L"CM", L"CN", L"CO", L"CR", L"CU", L"CV", L"CX", L"CY", L"CZ", L"DE", L"DJ", L"DK", L"DM", L"DO", 
				L"DZ", L"EC", L"EE", L"EG", L"ER", L"ES", L"ET", L"EU", L"FI", L"FJ", L"FK", L"FM", L"FO", L"FR", L"GA", L"GB", L"GD", L"GE", 
				L"GF", L"GH", L"GI", L"GL", L"GM", L"GN", L"GP", L"GQ", L"GR", L"GT", L"GU", L"GW", L"GY", L"HK", L"HM", L"HN", L"HR", L"HT", 
				L"HU", L"ID", L"IE", L"IL", L"IM", L"IN", L"IO", L"IQ", L"IR", L"IS", L"IT", L"JE", L"JM", L"JO", L"JP", L"KE", L"KG", L"KH", 
				L"KI", L"KM", L"KN", L"KP", L"KR", L"KW", L"KY", L"KZ", L"LA", L"LB", L"LC", L"LI", L"LK", L"LR", L"LS", L"LT", L"LU", L"LV", 
				L"LY", L"MA", L"MC", L"MD", L"ME", L"MG", L"MH", L"MK", L"ML", L"MM", L"MN", L"MO", L"MP", L"MQ", L"MR", L"MS", L"MT", L"MU", 
				L"MV", L"MW", L"MX", L"MY", L"MZ", L"NA", L"NC", L"NE", L"NF", L"NG", L"NI", L"NL", L"NO", L"NP", L"NR", L"NU", L"NZ", L"OM", 
				L"PA", L"PE", L"PF", L"PG", L"PH", L"PK", L"PL", L"PM", L"PR", L"PS", L"PT", L"PW", L"PY", L"QA", L"RE", L"RO", L"RS", L"RU", 
				L"RW", L"SA", L"SB", L"SC", L"SD", L"SE", L"SG", L"SI", L"SK", L"SL", L"SM", L"SN", L"SO", L"SR", L"ST", L"SV", L"SY", L"SZ", 
				L"TC", L"TD", L"TG", L"TH", L"TJ", L"TK", L"TL", L"TM", L"TN", L"TO", L"TR", L"TT", L"TV", L"TW", L"TZ", L"UA", L"UG", L"UM", 
				L"US", L"UY", L"UZ", L"VA", L"VC", L"VE", L"VG", L"VI", L"VN", L"VU", L"WF", L"WS", L"XX", L"YE", L"YT", L"ZA", L"ZM", L"ZW"});
			this->preferredNodes->Location = System::Drawing::Point(21, 26);
			this->preferredNodes->Name = L"preferredNodes";
			this->preferredNodes->Size = System::Drawing::Size(65, 244);
			this->preferredNodes->TabIndex = 0;
			// 
			// shapeContainer2
			// 
			this->shapeContainer2->Location = System::Drawing::Point(3, 3);
			this->shapeContainer2->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer2->Name = L"shapeContainer2";
			this->shapeContainer2->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(1) {this->lineShape2});
			this->shapeContainer2->Size = System::Drawing::Size(262, 278);
			this->shapeContainer2->TabIndex = 2;
			this->shapeContainer2->TabStop = false;
			// 
			// lineShape2
			// 
			this->lineShape2->BorderColor = System::Drawing::SystemColors::Highlight;
			this->lineShape2->Name = L"lineShape2";
			this->lineShape2->X1 = 104;
			this->lineShape2->X2 = 258;
			this->lineShape2->Y1 = 11;
			this->lineShape2->Y2 = 11;
			// 
			// tabPage3
			// 
			this->tabPage3->Controls->Add(this->defaultButton);
			this->tabPage3->Controls->Add(this->controlPort);
			this->tabPage3->Controls->Add(this->label10);
			this->tabPage3->Controls->Add(this->label9);
			this->tabPage3->Controls->Add(this->label8);
			this->tabPage3->Controls->Add(this->label7);
			this->tabPage3->Controls->Add(this->updateIn);
			this->tabPage3->Controls->Add(this->label6);
			this->tabPage3->Controls->Add(this->label5);
			this->tabPage3->Controls->Add(this->shapeContainer3);
			this->tabPage3->Location = System::Drawing::Point(4, 22);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->Padding = System::Windows::Forms::Padding(3);
			this->tabPage3->Size = System::Drawing::Size(268, 284);
			this->tabPage3->TabIndex = 2;
			this->tabPage3->Text = L"Advanced";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// defaultButton
			// 
			this->defaultButton->Location = System::Drawing::Point(187, 255);
			this->defaultButton->Name = L"defaultButton";
			this->defaultButton->Size = System::Drawing::Size(75, 23);
			this->defaultButton->TabIndex = 9;
			this->defaultButton->Text = L"Defaults";
			this->defaultButton->UseVisualStyleBackColor = true;
			this->defaultButton->Click += gcnew System::EventHandler(this, &ExitNode::defaultButton_Click);
			// 
			// controlPort
			// 
			this->controlPort->Location = System::Drawing::Point(82, 99);
			this->controlPort->Name = L"controlPort";
			this->controlPort->Size = System::Drawing::Size(76, 20);
			this->controlPort->TabIndex = 8;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(14, 102);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(65, 13);
			this->label10->TabIndex = 7;
			this->label10->Text = L"Control Port:";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(7, 75);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(61, 13);
			this->label9->TabIndex = 6;
			this->label9->Text = L"Connection";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(95, 61);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(146, 13);
			this->label8->TabIndex = 5;
			this->label8->Text = L"(Requires restart of GeoLock)";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(178, 36);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(88, 13);
			this->label7->TabIndex = 4;
			this->label7->Text = L"minute(s). [1-300]";
			// 
			// updateIn
			// 
			this->updateIn->Location = System::Drawing::Point(98, 33);
			this->updateIn->Name = L"updateIn";
			this->updateIn->Size = System::Drawing::Size(76, 20);
			this->updateIn->TabIndex = 3;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(11, 36);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(84, 13);
			this->label6->TabIndex = 2;
			this->label6->Text = L"Update IP every";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(7, 7);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(95, 13);
			this->label5->TabIndex = 0;
			this->label5->Text = L"Update Frequency";
			// 
			// shapeContainer3
			// 
			this->shapeContainer3->Location = System::Drawing::Point(3, 3);
			this->shapeContainer3->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer3->Name = L"shapeContainer3";
			this->shapeContainer3->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(2) {this->lineShape4, 
				this->lineShape3});
			this->shapeContainer3->Size = System::Drawing::Size(262, 278);
			this->shapeContainer3->TabIndex = 1;
			this->shapeContainer3->TabStop = false;
			// 
			// lineShape4
			// 
			this->lineShape4->BorderColor = System::Drawing::SystemColors::Highlight;
			this->lineShape4->Name = L"lineShape4";
			this->lineShape4->X1 = 65;
			this->lineShape4->X2 = 258;
			this->lineShape4->Y1 = 80;
			this->lineShape4->Y2 = 80;
			// 
			// lineShape3
			// 
			this->lineShape3->BorderColor = System::Drawing::SystemColors::Highlight;
			this->lineShape3->Name = L"lineShape3";
			this->lineShape3->X1 = 101;
			this->lineShape3->X2 = 258;
			this->lineShape3->Y1 = 11;
			this->lineShape3->Y2 = 11;
			// 
			// okButton
			// 
			this->okButton->Location = System::Drawing::Point(121, 321);
			this->okButton->Name = L"okButton";
			this->okButton->Size = System::Drawing::Size(75, 23);
			this->okButton->TabIndex = 5;
			this->okButton->Text = L"OK";
			this->okButton->UseVisualStyleBackColor = true;
			this->okButton->Click += gcnew System::EventHandler(this, &ExitNode::okButton_Click);
			// 
			// cancelButton
			// 
			this->cancelButton->Location = System::Drawing::Point(202, 321);
			this->cancelButton->Name = L"cancelButton";
			this->cancelButton->Size = System::Drawing::Size(75, 23);
			this->cancelButton->TabIndex = 6;
			this->cancelButton->Text = L"Cancel";
			this->cancelButton->UseVisualStyleBackColor = true;
			this->cancelButton->Click += gcnew System::EventHandler(this, &ExitNode::cancelButton_Click);
			// 
			// ExitNode
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(287, 351);
			this->Controls->Add(this->cancelButton);
			this->Controls->Add(this->okButton);
			this->Controls->Add(this->tabControl1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximumSize = System::Drawing::Size(303, 389);
			this->MinimumSize = System::Drawing::Size(303, 389);
			this->Name = L"ExitNode";
			this->Text = L"GeoLock Configuration";
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

private: System::Void selectAll_Click(System::Object^  sender, System::EventArgs^  e) {
			 for (int i=0;i<excludedNodes->Items->Count;i++) 
				 excludedNodes->SetItemChecked(i,true);
			 this->excludedNodes->ClearSelected();
		 }
private: System::Void clearAll_Click(System::Object^  sender, System::EventArgs^  e) {
			 for (int i=0;i<excludedNodes->Items->Count;i++) 
				 excludedNodes->SetItemChecked(i,false);
			 this->excludedNodes->ClearSelected();
		 }
private: System::Void SelectAllPre_Click(System::Object^  sender, System::EventArgs^  e) {
			 for (int i=0;i<preferredNodes->Items->Count;i++) 
				preferredNodes->SetItemChecked(i,true);
			 this->preferredNodes->ClearSelected();
		 }
private: System::Void ClearAllPre_Click(System::Object^  sender, System::EventArgs^  e) {
			 for (int i=0;i<preferredNodes->Items->Count;i++) 
				preferredNodes->SetItemChecked(i,false);
			 this->preferredNodes->ClearSelected();
		 }
private: System::Void okButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 System::Configuration::Configuration ^config = 
				 System::Configuration::ConfigurationManager::OpenExeConfiguration(System::Configuration::ConfigurationUserLevel::None);
			 String ^toBeExcluded = gcnew System::String("");
		     String ^toBePreferred = gcnew System::String("");
			 IEnumerator^ counter = excludedNodes->CheckedItems->GetEnumerator();
			 while (counter->MoveNext()) {
				 Object^ itemChecked = safe_cast<Object^>(counter->Current);
				 toBeExcluded += itemChecked + ",";
			 }
			 if (toBeExcluded->Length > 0) toBeExcluded = toBeExcluded->Remove(toBeExcluded->Length-1);
			 config->AppSettings->Settings->Remove("excludedExitNodes");
			 config->AppSettings->Settings->Add("excludedExitNodes",toBeExcluded);
			 IEnumerator^ counter2 = preferredNodes->CheckedItems->GetEnumerator();
			 while (counter2->MoveNext()) {
				 Object^ itemChecked = safe_cast<Object^>(counter2->Current);
				 toBePreferred += itemChecked + ",";
			 }
			 if (toBePreferred->Length > 0) toBePreferred = toBePreferred->Remove(toBePreferred->Length-1);
			 config->AppSettings->Settings->Remove("exitNodes");
			 config->AppSettings->Settings->Add("exitNodes",toBePreferred);
			 int temp = 5;
			 try {
				 temp = System::Int32::Parse(this->updateIn->Text);
			 }
			 catch (Exception^ ex) {
				 temp = 1;
			 }
			 if (temp > 300) temp = 300;
			 config->AppSettings->Settings->Remove("updateFreq");
			 config->AppSettings->Settings->Add("updateFreq",temp.ToString());
			 config->AppSettings->Settings->Remove("controlPort");
			 config->AppSettings->Settings->Add("controlPort",this->controlPort->Text);
			 config->Save(System::Configuration::ConfigurationSaveMode::Modified);
			 System::Configuration::ConfigurationManager::RefreshSection("appSettings");
			 Close();
		 }
private: System::Void cancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 Close();
		 }
private: System::Void defaultButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->controlPort->Text = L"9051";
			 this->updateIn->Text = L"5";
		 }
};
}
