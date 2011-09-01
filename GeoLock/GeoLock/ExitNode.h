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
			//load settings from app.config
			String^ managedExcluded = System::Configuration::ConfigurationManager::AppSettings["excludedExitNodes"];
			String^ managedExit = System::Configuration::ConfigurationManager::AppSettings["exitNodes"];
			String^ updateFreq = System::Configuration::ConfigurationManager::AppSettings["updateFreq"];
			String^ forceUpdate = System::Configuration::ConfigurationManager::AppSettings["forceUpdate"];
			String^ controlPortI = System::Configuration::ConfigurationManager::AppSettings["controlPort"];
			String^ stayOnTop = System::Configuration::ConfigurationManager::AppSettings["persist"];
			String^ opacity = System::Configuration::ConfigurationManager::AppSettings["opacity"];
			String^ taskbar = System::Configuration::ConfigurationManager::AppSettings["taskbar"];

			//convert comma delimited list into array
			array<String^>^ excludedList = managedExcluded->Split(',');
			if (excludedList[0] != "") {
				for(int i=0;i<excludedList->Length;i++) {
					//search for country code
					int index = this->excludedNodes->FindString(excludedList[i]);
					//check the box
					if (index != -1) this->excludedNodes->SetItemChecked(index,true);
				}
			}
			//convert comma delimited list into array
			array<String^>^ exitList = managedExit->Split(',');
			if (exitList[0] != "") {
				for(int i=0;i<exitList->Length;i++) {
					//search for country code
					int index = this->preferredNodes->FindString(exitList[i]);
					//check the box
					if (index != -1) this->preferredNodes->SetItemChecked(index,true);
				}
			}
			//set update frequency, control port, and opacity
			this->updateIn->Text = updateFreq;
			this->controlPort->Text = controlPortI;
			this->opacitySelect->Value = System::Decimal::Parse(opacity);

			//set forceUpdate checkbox
			if (forceUpdate == "true") {
				this->checkBox1->Checked = true;
				this->checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
			}
			//set stayOnTop checkbox
			if (stayOnTop == "true") {
				this->ontop->Checked = true;
				this->ontop->CheckState = System::Windows::Forms::CheckState::Checked;
			}
			//set taskbar checkbox
			if (taskbar == "true") {
				this->taskBar->Checked = true;
				this->taskBar->CheckState = System::Windows::Forms::CheckState::Checked;
			}
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
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Label^  label9;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape4;
	private: System::Windows::Forms::TextBox^  controlPort;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Button^  defaultButton;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label11;
	private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape5;
	private: System::Windows::Forms::CheckBox^  ontop;
	private: System::Windows::Forms::CheckBox^  taskBar;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::NumericUpDown^  opacitySelect;
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
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->ClearAllPre = (gcnew System::Windows::Forms::Button());
			this->SelectAllPre = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->preferredNodes = (gcnew System::Windows::Forms::CheckedListBox());
			this->shapeContainer2 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->tabPage3 = (gcnew System::Windows::Forms::TabPage());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->opacitySelect = (gcnew System::Windows::Forms::NumericUpDown());
			this->taskBar = (gcnew System::Windows::Forms::CheckBox());
			this->ontop = (gcnew System::Windows::Forms::CheckBox());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->defaultButton = (gcnew System::Windows::Forms::Button());
			this->controlPort = (gcnew System::Windows::Forms::TextBox());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->updateIn = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->shapeContainer3 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape5 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->okButton = (gcnew System::Windows::Forms::Button());
			this->cancelButton = (gcnew System::Windows::Forms::Button());
			this->tabControl1->SuspendLayout();
			this->tabPage1->SuspendLayout();
			this->tabPage2->SuspendLayout();
			this->tabPage3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->opacitySelect))->BeginInit();
			this->SuspendLayout();
			// 
			// excludedNodes
			// 
			resources->ApplyResources(this->excludedNodes, L"excludedNodes");
			this->excludedNodes->CheckOnClick = true;
			this->excludedNodes->FormattingEnabled = true;
			this->excludedNodes->Items->AddRange(gcnew cli::array< System::Object^  >(240) {resources->GetString(L"excludedNodes.Items"), 
				resources->GetString(L"excludedNodes.Items1"), resources->GetString(L"excludedNodes.Items2"), resources->GetString(L"excludedNodes.Items3"), 
				resources->GetString(L"excludedNodes.Items4"), resources->GetString(L"excludedNodes.Items5"), resources->GetString(L"excludedNodes.Items6"), 
				resources->GetString(L"excludedNodes.Items7"), resources->GetString(L"excludedNodes.Items8"), resources->GetString(L"excludedNodes.Items9"), 
				resources->GetString(L"excludedNodes.Items10"), resources->GetString(L"excludedNodes.Items11"), resources->GetString(L"excludedNodes.Items12"), 
				resources->GetString(L"excludedNodes.Items13"), resources->GetString(L"excludedNodes.Items14"), resources->GetString(L"excludedNodes.Items15"), 
				resources->GetString(L"excludedNodes.Items16"), resources->GetString(L"excludedNodes.Items17"), resources->GetString(L"excludedNodes.Items18"), 
				resources->GetString(L"excludedNodes.Items19"), resources->GetString(L"excludedNodes.Items20"), resources->GetString(L"excludedNodes.Items21"), 
				resources->GetString(L"excludedNodes.Items22"), resources->GetString(L"excludedNodes.Items23"), resources->GetString(L"excludedNodes.Items24"), 
				resources->GetString(L"excludedNodes.Items25"), resources->GetString(L"excludedNodes.Items26"), resources->GetString(L"excludedNodes.Items27"), 
				resources->GetString(L"excludedNodes.Items28"), resources->GetString(L"excludedNodes.Items29"), resources->GetString(L"excludedNodes.Items30"), 
				resources->GetString(L"excludedNodes.Items31"), resources->GetString(L"excludedNodes.Items32"), resources->GetString(L"excludedNodes.Items33"), 
				resources->GetString(L"excludedNodes.Items34"), resources->GetString(L"excludedNodes.Items35"), resources->GetString(L"excludedNodes.Items36"), 
				resources->GetString(L"excludedNodes.Items37"), resources->GetString(L"excludedNodes.Items38"), resources->GetString(L"excludedNodes.Items39"), 
				resources->GetString(L"excludedNodes.Items40"), resources->GetString(L"excludedNodes.Items41"), resources->GetString(L"excludedNodes.Items42"), 
				resources->GetString(L"excludedNodes.Items43"), resources->GetString(L"excludedNodes.Items44"), resources->GetString(L"excludedNodes.Items45"), 
				resources->GetString(L"excludedNodes.Items46"), resources->GetString(L"excludedNodes.Items47"), resources->GetString(L"excludedNodes.Items48"), 
				resources->GetString(L"excludedNodes.Items49"), resources->GetString(L"excludedNodes.Items50"), resources->GetString(L"excludedNodes.Items51"), 
				resources->GetString(L"excludedNodes.Items52"), resources->GetString(L"excludedNodes.Items53"), resources->GetString(L"excludedNodes.Items54"), 
				resources->GetString(L"excludedNodes.Items55"), resources->GetString(L"excludedNodes.Items56"), resources->GetString(L"excludedNodes.Items57"), 
				resources->GetString(L"excludedNodes.Items58"), resources->GetString(L"excludedNodes.Items59"), resources->GetString(L"excludedNodes.Items60"), 
				resources->GetString(L"excludedNodes.Items61"), resources->GetString(L"excludedNodes.Items62"), resources->GetString(L"excludedNodes.Items63"), 
				resources->GetString(L"excludedNodes.Items64"), resources->GetString(L"excludedNodes.Items65"), resources->GetString(L"excludedNodes.Items66"), 
				resources->GetString(L"excludedNodes.Items67"), resources->GetString(L"excludedNodes.Items68"), resources->GetString(L"excludedNodes.Items69"), 
				resources->GetString(L"excludedNodes.Items70"), resources->GetString(L"excludedNodes.Items71"), resources->GetString(L"excludedNodes.Items72"), 
				resources->GetString(L"excludedNodes.Items73"), resources->GetString(L"excludedNodes.Items74"), resources->GetString(L"excludedNodes.Items75"), 
				resources->GetString(L"excludedNodes.Items76"), resources->GetString(L"excludedNodes.Items77"), resources->GetString(L"excludedNodes.Items78"), 
				resources->GetString(L"excludedNodes.Items79"), resources->GetString(L"excludedNodes.Items80"), resources->GetString(L"excludedNodes.Items81"), 
				resources->GetString(L"excludedNodes.Items82"), resources->GetString(L"excludedNodes.Items83"), resources->GetString(L"excludedNodes.Items84"), 
				resources->GetString(L"excludedNodes.Items85"), resources->GetString(L"excludedNodes.Items86"), resources->GetString(L"excludedNodes.Items87"), 
				resources->GetString(L"excludedNodes.Items88"), resources->GetString(L"excludedNodes.Items89"), resources->GetString(L"excludedNodes.Items90"), 
				resources->GetString(L"excludedNodes.Items91"), resources->GetString(L"excludedNodes.Items92"), resources->GetString(L"excludedNodes.Items93"), 
				resources->GetString(L"excludedNodes.Items94"), resources->GetString(L"excludedNodes.Items95"), resources->GetString(L"excludedNodes.Items96"), 
				resources->GetString(L"excludedNodes.Items97"), resources->GetString(L"excludedNodes.Items98"), resources->GetString(L"excludedNodes.Items99"), 
				resources->GetString(L"excludedNodes.Items100"), resources->GetString(L"excludedNodes.Items101"), resources->GetString(L"excludedNodes.Items102"), 
				resources->GetString(L"excludedNodes.Items103"), resources->GetString(L"excludedNodes.Items104"), resources->GetString(L"excludedNodes.Items105"), 
				resources->GetString(L"excludedNodes.Items106"), resources->GetString(L"excludedNodes.Items107"), resources->GetString(L"excludedNodes.Items108"), 
				resources->GetString(L"excludedNodes.Items109"), resources->GetString(L"excludedNodes.Items110"), resources->GetString(L"excludedNodes.Items111"), 
				resources->GetString(L"excludedNodes.Items112"), resources->GetString(L"excludedNodes.Items113"), resources->GetString(L"excludedNodes.Items114"), 
				resources->GetString(L"excludedNodes.Items115"), resources->GetString(L"excludedNodes.Items116"), resources->GetString(L"excludedNodes.Items117"), 
				resources->GetString(L"excludedNodes.Items118"), resources->GetString(L"excludedNodes.Items119"), resources->GetString(L"excludedNodes.Items120"), 
				resources->GetString(L"excludedNodes.Items121"), resources->GetString(L"excludedNodes.Items122"), resources->GetString(L"excludedNodes.Items123"), 
				resources->GetString(L"excludedNodes.Items124"), resources->GetString(L"excludedNodes.Items125"), resources->GetString(L"excludedNodes.Items126"), 
				resources->GetString(L"excludedNodes.Items127"), resources->GetString(L"excludedNodes.Items128"), resources->GetString(L"excludedNodes.Items129"), 
				resources->GetString(L"excludedNodes.Items130"), resources->GetString(L"excludedNodes.Items131"), resources->GetString(L"excludedNodes.Items132"), 
				resources->GetString(L"excludedNodes.Items133"), resources->GetString(L"excludedNodes.Items134"), resources->GetString(L"excludedNodes.Items135"), 
				resources->GetString(L"excludedNodes.Items136"), resources->GetString(L"excludedNodes.Items137"), resources->GetString(L"excludedNodes.Items138"), 
				resources->GetString(L"excludedNodes.Items139"), resources->GetString(L"excludedNodes.Items140"), resources->GetString(L"excludedNodes.Items141"), 
				resources->GetString(L"excludedNodes.Items142"), resources->GetString(L"excludedNodes.Items143"), resources->GetString(L"excludedNodes.Items144"), 
				resources->GetString(L"excludedNodes.Items145"), resources->GetString(L"excludedNodes.Items146"), resources->GetString(L"excludedNodes.Items147"), 
				resources->GetString(L"excludedNodes.Items148"), resources->GetString(L"excludedNodes.Items149"), resources->GetString(L"excludedNodes.Items150"), 
				resources->GetString(L"excludedNodes.Items151"), resources->GetString(L"excludedNodes.Items152"), resources->GetString(L"excludedNodes.Items153"), 
				resources->GetString(L"excludedNodes.Items154"), resources->GetString(L"excludedNodes.Items155"), resources->GetString(L"excludedNodes.Items156"), 
				resources->GetString(L"excludedNodes.Items157"), resources->GetString(L"excludedNodes.Items158"), resources->GetString(L"excludedNodes.Items159"), 
				resources->GetString(L"excludedNodes.Items160"), resources->GetString(L"excludedNodes.Items161"), resources->GetString(L"excludedNodes.Items162"), 
				resources->GetString(L"excludedNodes.Items163"), resources->GetString(L"excludedNodes.Items164"), resources->GetString(L"excludedNodes.Items165"), 
				resources->GetString(L"excludedNodes.Items166"), resources->GetString(L"excludedNodes.Items167"), resources->GetString(L"excludedNodes.Items168"), 
				resources->GetString(L"excludedNodes.Items169"), resources->GetString(L"excludedNodes.Items170"), resources->GetString(L"excludedNodes.Items171"), 
				resources->GetString(L"excludedNodes.Items172"), resources->GetString(L"excludedNodes.Items173"), resources->GetString(L"excludedNodes.Items174"), 
				resources->GetString(L"excludedNodes.Items175"), resources->GetString(L"excludedNodes.Items176"), resources->GetString(L"excludedNodes.Items177"), 
				resources->GetString(L"excludedNodes.Items178"), resources->GetString(L"excludedNodes.Items179"), resources->GetString(L"excludedNodes.Items180"), 
				resources->GetString(L"excludedNodes.Items181"), resources->GetString(L"excludedNodes.Items182"), resources->GetString(L"excludedNodes.Items183"), 
				resources->GetString(L"excludedNodes.Items184"), resources->GetString(L"excludedNodes.Items185"), resources->GetString(L"excludedNodes.Items186"), 
				resources->GetString(L"excludedNodes.Items187"), resources->GetString(L"excludedNodes.Items188"), resources->GetString(L"excludedNodes.Items189"), 
				resources->GetString(L"excludedNodes.Items190"), resources->GetString(L"excludedNodes.Items191"), resources->GetString(L"excludedNodes.Items192"), 
				resources->GetString(L"excludedNodes.Items193"), resources->GetString(L"excludedNodes.Items194"), resources->GetString(L"excludedNodes.Items195"), 
				resources->GetString(L"excludedNodes.Items196"), resources->GetString(L"excludedNodes.Items197"), resources->GetString(L"excludedNodes.Items198"), 
				resources->GetString(L"excludedNodes.Items199"), resources->GetString(L"excludedNodes.Items200"), resources->GetString(L"excludedNodes.Items201"), 
				resources->GetString(L"excludedNodes.Items202"), resources->GetString(L"excludedNodes.Items203"), resources->GetString(L"excludedNodes.Items204"), 
				resources->GetString(L"excludedNodes.Items205"), resources->GetString(L"excludedNodes.Items206"), resources->GetString(L"excludedNodes.Items207"), 
				resources->GetString(L"excludedNodes.Items208"), resources->GetString(L"excludedNodes.Items209"), resources->GetString(L"excludedNodes.Items210"), 
				resources->GetString(L"excludedNodes.Items211"), resources->GetString(L"excludedNodes.Items212"), resources->GetString(L"excludedNodes.Items213"), 
				resources->GetString(L"excludedNodes.Items214"), resources->GetString(L"excludedNodes.Items215"), resources->GetString(L"excludedNodes.Items216"), 
				resources->GetString(L"excludedNodes.Items217"), resources->GetString(L"excludedNodes.Items218"), resources->GetString(L"excludedNodes.Items219"), 
				resources->GetString(L"excludedNodes.Items220"), resources->GetString(L"excludedNodes.Items221"), resources->GetString(L"excludedNodes.Items222"), 
				resources->GetString(L"excludedNodes.Items223"), resources->GetString(L"excludedNodes.Items224"), resources->GetString(L"excludedNodes.Items225"), 
				resources->GetString(L"excludedNodes.Items226"), resources->GetString(L"excludedNodes.Items227"), resources->GetString(L"excludedNodes.Items228"), 
				resources->GetString(L"excludedNodes.Items229"), resources->GetString(L"excludedNodes.Items230"), resources->GetString(L"excludedNodes.Items231"), 
				resources->GetString(L"excludedNodes.Items232"), resources->GetString(L"excludedNodes.Items233"), resources->GetString(L"excludedNodes.Items234"), 
				resources->GetString(L"excludedNodes.Items235"), resources->GetString(L"excludedNodes.Items236"), resources->GetString(L"excludedNodes.Items237"), 
				resources->GetString(L"excludedNodes.Items238"), resources->GetString(L"excludedNodes.Items239")});
			this->excludedNodes->Name = L"excludedNodes";
			// 
			// tabControl1
			// 
			resources->ApplyResources(this->tabControl1, L"tabControl1");
			this->tabControl1->Controls->Add(this->tabPage1);
			this->tabControl1->Controls->Add(this->tabPage2);
			this->tabControl1->Controls->Add(this->tabPage3);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			// 
			// tabPage1
			// 
			resources->ApplyResources(this->tabPage1, L"tabPage1");
			this->tabPage1->Controls->Add(this->clearAll);
			this->tabPage1->Controls->Add(this->selectAll);
			this->tabPage1->Controls->Add(this->label2);
			this->tabPage1->Controls->Add(this->label1);
			this->tabPage1->Controls->Add(this->excludedNodes);
			this->tabPage1->Controls->Add(this->shapeContainer1);
			this->tabPage1->Name = L"tabPage1";
			this->tabPage1->UseVisualStyleBackColor = true;
			// 
			// clearAll
			// 
			resources->ApplyResources(this->clearAll, L"clearAll");
			this->clearAll->Name = L"clearAll";
			this->clearAll->UseVisualStyleBackColor = true;
			this->clearAll->Click += gcnew System::EventHandler(this, &ExitNode::clearAll_Click);
			// 
			// selectAll
			// 
			resources->ApplyResources(this->selectAll, L"selectAll");
			this->selectAll->Name = L"selectAll";
			this->selectAll->UseVisualStyleBackColor = true;
			this->selectAll->Click += gcnew System::EventHandler(this, &ExitNode::selectAll_Click);
			// 
			// label2
			// 
			resources->ApplyResources(this->label2, L"label2");
			this->label2->Name = L"label2";
			// 
			// label1
			// 
			resources->ApplyResources(this->label1, L"label1");
			this->label1->Name = L"label1";
			// 
			// shapeContainer1
			// 
			resources->ApplyResources(this->shapeContainer1, L"shapeContainer1");
			this->shapeContainer1->Name = L"shapeContainer1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(1) {this->lineShape1});
			this->shapeContainer1->TabStop = false;
			// 
			// lineShape1
			// 
			resources->ApplyResources(this->lineShape1, L"lineShape1");
			this->lineShape1->Name = L"lineShape1";
			// 
			// tabPage2
			// 
			resources->ApplyResources(this->tabPage2, L"tabPage2");
			this->tabPage2->Controls->Add(this->label4);
			this->tabPage2->Controls->Add(this->ClearAllPre);
			this->tabPage2->Controls->Add(this->SelectAllPre);
			this->tabPage2->Controls->Add(this->label3);
			this->tabPage2->Controls->Add(this->preferredNodes);
			this->tabPage2->Controls->Add(this->shapeContainer2);
			this->tabPage2->Name = L"tabPage2";
			this->tabPage2->UseVisualStyleBackColor = true;
			// 
			// label4
			// 
			resources->ApplyResources(this->label4, L"label4");
			this->label4->Name = L"label4";
			// 
			// ClearAllPre
			// 
			resources->ApplyResources(this->ClearAllPre, L"ClearAllPre");
			this->ClearAllPre->Name = L"ClearAllPre";
			this->ClearAllPre->UseVisualStyleBackColor = true;
			this->ClearAllPre->Click += gcnew System::EventHandler(this, &ExitNode::ClearAllPre_Click);
			// 
			// SelectAllPre
			// 
			resources->ApplyResources(this->SelectAllPre, L"SelectAllPre");
			this->SelectAllPre->Name = L"SelectAllPre";
			this->SelectAllPre->UseVisualStyleBackColor = true;
			this->SelectAllPre->Click += gcnew System::EventHandler(this, &ExitNode::SelectAllPre_Click);
			// 
			// label3
			// 
			resources->ApplyResources(this->label3, L"label3");
			this->label3->Name = L"label3";
			// 
			// preferredNodes
			// 
			resources->ApplyResources(this->preferredNodes, L"preferredNodes");
			this->preferredNodes->CheckOnClick = true;
			this->preferredNodes->FormattingEnabled = true;
			this->preferredNodes->Items->AddRange(gcnew cli::array< System::Object^  >(240) {resources->GetString(L"preferredNodes.Items"), 
				resources->GetString(L"preferredNodes.Items1"), resources->GetString(L"preferredNodes.Items2"), resources->GetString(L"preferredNodes.Items3"), 
				resources->GetString(L"preferredNodes.Items4"), resources->GetString(L"preferredNodes.Items5"), resources->GetString(L"preferredNodes.Items6"), 
				resources->GetString(L"preferredNodes.Items7"), resources->GetString(L"preferredNodes.Items8"), resources->GetString(L"preferredNodes.Items9"), 
				resources->GetString(L"preferredNodes.Items10"), resources->GetString(L"preferredNodes.Items11"), resources->GetString(L"preferredNodes.Items12"), 
				resources->GetString(L"preferredNodes.Items13"), resources->GetString(L"preferredNodes.Items14"), resources->GetString(L"preferredNodes.Items15"), 
				resources->GetString(L"preferredNodes.Items16"), resources->GetString(L"preferredNodes.Items17"), resources->GetString(L"preferredNodes.Items18"), 
				resources->GetString(L"preferredNodes.Items19"), resources->GetString(L"preferredNodes.Items20"), resources->GetString(L"preferredNodes.Items21"), 
				resources->GetString(L"preferredNodes.Items22"), resources->GetString(L"preferredNodes.Items23"), resources->GetString(L"preferredNodes.Items24"), 
				resources->GetString(L"preferredNodes.Items25"), resources->GetString(L"preferredNodes.Items26"), resources->GetString(L"preferredNodes.Items27"), 
				resources->GetString(L"preferredNodes.Items28"), resources->GetString(L"preferredNodes.Items29"), resources->GetString(L"preferredNodes.Items30"), 
				resources->GetString(L"preferredNodes.Items31"), resources->GetString(L"preferredNodes.Items32"), resources->GetString(L"preferredNodes.Items33"), 
				resources->GetString(L"preferredNodes.Items34"), resources->GetString(L"preferredNodes.Items35"), resources->GetString(L"preferredNodes.Items36"), 
				resources->GetString(L"preferredNodes.Items37"), resources->GetString(L"preferredNodes.Items38"), resources->GetString(L"preferredNodes.Items39"), 
				resources->GetString(L"preferredNodes.Items40"), resources->GetString(L"preferredNodes.Items41"), resources->GetString(L"preferredNodes.Items42"), 
				resources->GetString(L"preferredNodes.Items43"), resources->GetString(L"preferredNodes.Items44"), resources->GetString(L"preferredNodes.Items45"), 
				resources->GetString(L"preferredNodes.Items46"), resources->GetString(L"preferredNodes.Items47"), resources->GetString(L"preferredNodes.Items48"), 
				resources->GetString(L"preferredNodes.Items49"), resources->GetString(L"preferredNodes.Items50"), resources->GetString(L"preferredNodes.Items51"), 
				resources->GetString(L"preferredNodes.Items52"), resources->GetString(L"preferredNodes.Items53"), resources->GetString(L"preferredNodes.Items54"), 
				resources->GetString(L"preferredNodes.Items55"), resources->GetString(L"preferredNodes.Items56"), resources->GetString(L"preferredNodes.Items57"), 
				resources->GetString(L"preferredNodes.Items58"), resources->GetString(L"preferredNodes.Items59"), resources->GetString(L"preferredNodes.Items60"), 
				resources->GetString(L"preferredNodes.Items61"), resources->GetString(L"preferredNodes.Items62"), resources->GetString(L"preferredNodes.Items63"), 
				resources->GetString(L"preferredNodes.Items64"), resources->GetString(L"preferredNodes.Items65"), resources->GetString(L"preferredNodes.Items66"), 
				resources->GetString(L"preferredNodes.Items67"), resources->GetString(L"preferredNodes.Items68"), resources->GetString(L"preferredNodes.Items69"), 
				resources->GetString(L"preferredNodes.Items70"), resources->GetString(L"preferredNodes.Items71"), resources->GetString(L"preferredNodes.Items72"), 
				resources->GetString(L"preferredNodes.Items73"), resources->GetString(L"preferredNodes.Items74"), resources->GetString(L"preferredNodes.Items75"), 
				resources->GetString(L"preferredNodes.Items76"), resources->GetString(L"preferredNodes.Items77"), resources->GetString(L"preferredNodes.Items78"), 
				resources->GetString(L"preferredNodes.Items79"), resources->GetString(L"preferredNodes.Items80"), resources->GetString(L"preferredNodes.Items81"), 
				resources->GetString(L"preferredNodes.Items82"), resources->GetString(L"preferredNodes.Items83"), resources->GetString(L"preferredNodes.Items84"), 
				resources->GetString(L"preferredNodes.Items85"), resources->GetString(L"preferredNodes.Items86"), resources->GetString(L"preferredNodes.Items87"), 
				resources->GetString(L"preferredNodes.Items88"), resources->GetString(L"preferredNodes.Items89"), resources->GetString(L"preferredNodes.Items90"), 
				resources->GetString(L"preferredNodes.Items91"), resources->GetString(L"preferredNodes.Items92"), resources->GetString(L"preferredNodes.Items93"), 
				resources->GetString(L"preferredNodes.Items94"), resources->GetString(L"preferredNodes.Items95"), resources->GetString(L"preferredNodes.Items96"), 
				resources->GetString(L"preferredNodes.Items97"), resources->GetString(L"preferredNodes.Items98"), resources->GetString(L"preferredNodes.Items99"), 
				resources->GetString(L"preferredNodes.Items100"), resources->GetString(L"preferredNodes.Items101"), resources->GetString(L"preferredNodes.Items102"), 
				resources->GetString(L"preferredNodes.Items103"), resources->GetString(L"preferredNodes.Items104"), resources->GetString(L"preferredNodes.Items105"), 
				resources->GetString(L"preferredNodes.Items106"), resources->GetString(L"preferredNodes.Items107"), resources->GetString(L"preferredNodes.Items108"), 
				resources->GetString(L"preferredNodes.Items109"), resources->GetString(L"preferredNodes.Items110"), resources->GetString(L"preferredNodes.Items111"), 
				resources->GetString(L"preferredNodes.Items112"), resources->GetString(L"preferredNodes.Items113"), resources->GetString(L"preferredNodes.Items114"), 
				resources->GetString(L"preferredNodes.Items115"), resources->GetString(L"preferredNodes.Items116"), resources->GetString(L"preferredNodes.Items117"), 
				resources->GetString(L"preferredNodes.Items118"), resources->GetString(L"preferredNodes.Items119"), resources->GetString(L"preferredNodes.Items120"), 
				resources->GetString(L"preferredNodes.Items121"), resources->GetString(L"preferredNodes.Items122"), resources->GetString(L"preferredNodes.Items123"), 
				resources->GetString(L"preferredNodes.Items124"), resources->GetString(L"preferredNodes.Items125"), resources->GetString(L"preferredNodes.Items126"), 
				resources->GetString(L"preferredNodes.Items127"), resources->GetString(L"preferredNodes.Items128"), resources->GetString(L"preferredNodes.Items129"), 
				resources->GetString(L"preferredNodes.Items130"), resources->GetString(L"preferredNodes.Items131"), resources->GetString(L"preferredNodes.Items132"), 
				resources->GetString(L"preferredNodes.Items133"), resources->GetString(L"preferredNodes.Items134"), resources->GetString(L"preferredNodes.Items135"), 
				resources->GetString(L"preferredNodes.Items136"), resources->GetString(L"preferredNodes.Items137"), resources->GetString(L"preferredNodes.Items138"), 
				resources->GetString(L"preferredNodes.Items139"), resources->GetString(L"preferredNodes.Items140"), resources->GetString(L"preferredNodes.Items141"), 
				resources->GetString(L"preferredNodes.Items142"), resources->GetString(L"preferredNodes.Items143"), resources->GetString(L"preferredNodes.Items144"), 
				resources->GetString(L"preferredNodes.Items145"), resources->GetString(L"preferredNodes.Items146"), resources->GetString(L"preferredNodes.Items147"), 
				resources->GetString(L"preferredNodes.Items148"), resources->GetString(L"preferredNodes.Items149"), resources->GetString(L"preferredNodes.Items150"), 
				resources->GetString(L"preferredNodes.Items151"), resources->GetString(L"preferredNodes.Items152"), resources->GetString(L"preferredNodes.Items153"), 
				resources->GetString(L"preferredNodes.Items154"), resources->GetString(L"preferredNodes.Items155"), resources->GetString(L"preferredNodes.Items156"), 
				resources->GetString(L"preferredNodes.Items157"), resources->GetString(L"preferredNodes.Items158"), resources->GetString(L"preferredNodes.Items159"), 
				resources->GetString(L"preferredNodes.Items160"), resources->GetString(L"preferredNodes.Items161"), resources->GetString(L"preferredNodes.Items162"), 
				resources->GetString(L"preferredNodes.Items163"), resources->GetString(L"preferredNodes.Items164"), resources->GetString(L"preferredNodes.Items165"), 
				resources->GetString(L"preferredNodes.Items166"), resources->GetString(L"preferredNodes.Items167"), resources->GetString(L"preferredNodes.Items168"), 
				resources->GetString(L"preferredNodes.Items169"), resources->GetString(L"preferredNodes.Items170"), resources->GetString(L"preferredNodes.Items171"), 
				resources->GetString(L"preferredNodes.Items172"), resources->GetString(L"preferredNodes.Items173"), resources->GetString(L"preferredNodes.Items174"), 
				resources->GetString(L"preferredNodes.Items175"), resources->GetString(L"preferredNodes.Items176"), resources->GetString(L"preferredNodes.Items177"), 
				resources->GetString(L"preferredNodes.Items178"), resources->GetString(L"preferredNodes.Items179"), resources->GetString(L"preferredNodes.Items180"), 
				resources->GetString(L"preferredNodes.Items181"), resources->GetString(L"preferredNodes.Items182"), resources->GetString(L"preferredNodes.Items183"), 
				resources->GetString(L"preferredNodes.Items184"), resources->GetString(L"preferredNodes.Items185"), resources->GetString(L"preferredNodes.Items186"), 
				resources->GetString(L"preferredNodes.Items187"), resources->GetString(L"preferredNodes.Items188"), resources->GetString(L"preferredNodes.Items189"), 
				resources->GetString(L"preferredNodes.Items190"), resources->GetString(L"preferredNodes.Items191"), resources->GetString(L"preferredNodes.Items192"), 
				resources->GetString(L"preferredNodes.Items193"), resources->GetString(L"preferredNodes.Items194"), resources->GetString(L"preferredNodes.Items195"), 
				resources->GetString(L"preferredNodes.Items196"), resources->GetString(L"preferredNodes.Items197"), resources->GetString(L"preferredNodes.Items198"), 
				resources->GetString(L"preferredNodes.Items199"), resources->GetString(L"preferredNodes.Items200"), resources->GetString(L"preferredNodes.Items201"), 
				resources->GetString(L"preferredNodes.Items202"), resources->GetString(L"preferredNodes.Items203"), resources->GetString(L"preferredNodes.Items204"), 
				resources->GetString(L"preferredNodes.Items205"), resources->GetString(L"preferredNodes.Items206"), resources->GetString(L"preferredNodes.Items207"), 
				resources->GetString(L"preferredNodes.Items208"), resources->GetString(L"preferredNodes.Items209"), resources->GetString(L"preferredNodes.Items210"), 
				resources->GetString(L"preferredNodes.Items211"), resources->GetString(L"preferredNodes.Items212"), resources->GetString(L"preferredNodes.Items213"), 
				resources->GetString(L"preferredNodes.Items214"), resources->GetString(L"preferredNodes.Items215"), resources->GetString(L"preferredNodes.Items216"), 
				resources->GetString(L"preferredNodes.Items217"), resources->GetString(L"preferredNodes.Items218"), resources->GetString(L"preferredNodes.Items219"), 
				resources->GetString(L"preferredNodes.Items220"), resources->GetString(L"preferredNodes.Items221"), resources->GetString(L"preferredNodes.Items222"), 
				resources->GetString(L"preferredNodes.Items223"), resources->GetString(L"preferredNodes.Items224"), resources->GetString(L"preferredNodes.Items225"), 
				resources->GetString(L"preferredNodes.Items226"), resources->GetString(L"preferredNodes.Items227"), resources->GetString(L"preferredNodes.Items228"), 
				resources->GetString(L"preferredNodes.Items229"), resources->GetString(L"preferredNodes.Items230"), resources->GetString(L"preferredNodes.Items231"), 
				resources->GetString(L"preferredNodes.Items232"), resources->GetString(L"preferredNodes.Items233"), resources->GetString(L"preferredNodes.Items234"), 
				resources->GetString(L"preferredNodes.Items235"), resources->GetString(L"preferredNodes.Items236"), resources->GetString(L"preferredNodes.Items237"), 
				resources->GetString(L"preferredNodes.Items238"), resources->GetString(L"preferredNodes.Items239")});
			this->preferredNodes->Name = L"preferredNodes";
			// 
			// shapeContainer2
			// 
			resources->ApplyResources(this->shapeContainer2, L"shapeContainer2");
			this->shapeContainer2->Name = L"shapeContainer2";
			this->shapeContainer2->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(1) {this->lineShape2});
			this->shapeContainer2->TabStop = false;
			// 
			// lineShape2
			// 
			resources->ApplyResources(this->lineShape2, L"lineShape2");
			this->lineShape2->Name = L"lineShape2";
			// 
			// tabPage3
			// 
			resources->ApplyResources(this->tabPage3, L"tabPage3");
			this->tabPage3->Controls->Add(this->label13);
			this->tabPage3->Controls->Add(this->label12);
			this->tabPage3->Controls->Add(this->opacitySelect);
			this->tabPage3->Controls->Add(this->taskBar);
			this->tabPage3->Controls->Add(this->ontop);
			this->tabPage3->Controls->Add(this->label11);
			this->tabPage3->Controls->Add(this->checkBox1);
			this->tabPage3->Controls->Add(this->defaultButton);
			this->tabPage3->Controls->Add(this->controlPort);
			this->tabPage3->Controls->Add(this->label10);
			this->tabPage3->Controls->Add(this->label9);
			this->tabPage3->Controls->Add(this->label7);
			this->tabPage3->Controls->Add(this->updateIn);
			this->tabPage3->Controls->Add(this->label6);
			this->tabPage3->Controls->Add(this->label5);
			this->tabPage3->Controls->Add(this->shapeContainer3);
			this->tabPage3->Name = L"tabPage3";
			this->tabPage3->UseVisualStyleBackColor = true;
			// 
			// label13
			// 
			resources->ApplyResources(this->label13, L"label13");
			this->label13->Name = L"label13";
			// 
			// label12
			// 
			resources->ApplyResources(this->label12, L"label12");
			this->label12->Name = L"label12";
			// 
			// opacitySelect
			// 
			resources->ApplyResources(this->opacitySelect, L"opacitySelect");
			this->opacitySelect->Name = L"opacitySelect";
			this->opacitySelect->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {100, 0, 0, 0});
			// 
			// taskBar
			// 
			resources->ApplyResources(this->taskBar, L"taskBar");
			this->taskBar->Name = L"taskBar";
			this->taskBar->UseVisualStyleBackColor = true;
			// 
			// ontop
			// 
			resources->ApplyResources(this->ontop, L"ontop");
			this->ontop->Name = L"ontop";
			this->ontop->UseVisualStyleBackColor = true;
			// 
			// label11
			// 
			resources->ApplyResources(this->label11, L"label11");
			this->label11->Name = L"label11";
			// 
			// checkBox1
			// 
			resources->ApplyResources(this->checkBox1, L"checkBox1");
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// defaultButton
			// 
			resources->ApplyResources(this->defaultButton, L"defaultButton");
			this->defaultButton->Name = L"defaultButton";
			this->defaultButton->UseVisualStyleBackColor = true;
			this->defaultButton->Click += gcnew System::EventHandler(this, &ExitNode::defaultButton_Click);
			// 
			// controlPort
			// 
			resources->ApplyResources(this->controlPort, L"controlPort");
			this->controlPort->Name = L"controlPort";
			// 
			// label10
			// 
			resources->ApplyResources(this->label10, L"label10");
			this->label10->Name = L"label10";
			// 
			// label9
			// 
			resources->ApplyResources(this->label9, L"label9");
			this->label9->Name = L"label9";
			// 
			// label7
			// 
			resources->ApplyResources(this->label7, L"label7");
			this->label7->Name = L"label7";
			// 
			// updateIn
			// 
			resources->ApplyResources(this->updateIn, L"updateIn");
			this->updateIn->Name = L"updateIn";
			// 
			// label6
			// 
			resources->ApplyResources(this->label6, L"label6");
			this->label6->Name = L"label6";
			// 
			// label5
			// 
			resources->ApplyResources(this->label5, L"label5");
			this->label5->Name = L"label5";
			// 
			// shapeContainer3
			// 
			resources->ApplyResources(this->shapeContainer3, L"shapeContainer3");
			this->shapeContainer3->Name = L"shapeContainer3";
			this->shapeContainer3->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(3) {this->lineShape5, 
				this->lineShape4, this->lineShape3});
			this->shapeContainer3->TabStop = false;
			// 
			// lineShape5
			// 
			resources->ApplyResources(this->lineShape5, L"lineShape5");
			this->lineShape5->Name = L"lineShape5";
			// 
			// lineShape4
			// 
			resources->ApplyResources(this->lineShape4, L"lineShape4");
			this->lineShape4->Name = L"lineShape4";
			// 
			// lineShape3
			// 
			resources->ApplyResources(this->lineShape3, L"lineShape3");
			this->lineShape3->Name = L"lineShape3";
			// 
			// okButton
			// 
			resources->ApplyResources(this->okButton, L"okButton");
			this->okButton->Name = L"okButton";
			this->okButton->UseVisualStyleBackColor = true;
			this->okButton->Click += gcnew System::EventHandler(this, &ExitNode::okButton_Click);
			// 
			// cancelButton
			// 
			resources->ApplyResources(this->cancelButton, L"cancelButton");
			this->cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->cancelButton->Name = L"cancelButton";
			this->cancelButton->UseVisualStyleBackColor = true;
			this->cancelButton->Click += gcnew System::EventHandler(this, &ExitNode::cancelButton_Click);
			// 
			// ExitNode
			// 
			this->AcceptButton = this->okButton;
			resources->ApplyResources(this, L"$this");
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->CancelButton = this->cancelButton;
			this->Controls->Add(this->cancelButton);
			this->Controls->Add(this->okButton);
			this->Controls->Add(this->tabControl1);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ExitNode";
			this->ShowInTaskbar = false;
			this->tabControl1->ResumeLayout(false);
			this->tabPage1->ResumeLayout(false);
			this->tabPage1->PerformLayout();
			this->tabPage2->ResumeLayout(false);
			this->tabPage2->PerformLayout();
			this->tabPage3->ResumeLayout(false);
			this->tabPage3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->opacitySelect))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
//BEGIN "Select All" and "Clear All" button definitions
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
//END "Select All" and "Clear All" button definitions

//save changes and close the window
private: System::Void okButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 //prepare app.config for editing
			 System::Configuration::Configuration ^config = 
				 System::Configuration::ConfigurationManager::OpenExeConfiguration(System::Configuration::ConfigurationUserLevel::None);
			 String ^toBeExcluded = gcnew System::String("");
		     String ^toBePreferred = gcnew System::String("");
			 //build a comma delimited list of excluded nodes
			 IEnumerator^ counter = excludedNodes->CheckedItems->GetEnumerator();
			 while (counter->MoveNext()) {
				 Object^ itemChecked = safe_cast<Object^>(counter->Current);
				 toBeExcluded += itemChecked + ",";
			 }
			 //write new excluded nodes to app.config
			 if (toBeExcluded->Length > 0) toBeExcluded = toBeExcluded->Remove(toBeExcluded->Length-1);
			 config->AppSettings->Settings->Remove("excludedExitNodes");
			 config->AppSettings->Settings->Add("excludedExitNodes",toBeExcluded);
			 //build a comma delimited list of preferred nodes
			 IEnumerator^ counter2 = preferredNodes->CheckedItems->GetEnumerator();
			 while (counter2->MoveNext()) {
				 Object^ itemChecked = safe_cast<Object^>(counter2->Current);
				 toBePreferred += itemChecked + ",";
			 }
			 //write new preferred nodes to app.config
			 if (toBePreferred->Length > 0) toBePreferred = toBePreferred->Remove(toBePreferred->Length-1);
			 config->AppSettings->Settings->Remove("exitNodes");
			 config->AppSettings->Settings->Add("exitNodes",toBePreferred);
			 //temporary update frequency to prevent the user from doing anything strange
			 int temp = 5;
			 try {
				 //try to convert whatever the user put into the update frequency box into a number
				 temp = System::Int32::Parse(this->updateIn->Text);
			 }
			 catch (Exception^ ex) {
				 //if unsuccessful, assume it was a fraction and set to lowest possible (1)
				 temp = 1;
			 }
			 //if larger than 5 hours, reduce to maximum
			 if (temp > 300) temp = 300;
			 //write update cleansed update frequency
			 config->AppSettings->Settings->Remove("updateFreq");
			 config->AppSettings->Settings->Add("updateFreq",temp.ToString());
			 //write force update boolean
			 config->AppSettings->Settings->Remove("forceUpdate");
			 if (this->checkBox1->Checked) config->AppSettings->Settings->Add("forceUpdate","true");
			 else config->AppSettings->Settings->Add("forceUpdate","false");
			 //write stayOnTop boolean
			 config->AppSettings->Settings->Remove("persist");
			 if (this->ontop->Checked) config->AppSettings->Settings->Add("persist","true");
			 else config->AppSettings->Settings->Add("persist","false");
			 //write taskbar boolean
			 config->AppSettings->Settings->Remove("taskbar");
			 if (this->taskBar->Checked) config->AppSettings->Settings->Add("taskbar","true");
			 else config->AppSettings->Settings->Add("taskbar","false");
			 //temporary control port to prevent the user from doing anything strange
			 int port = 0;
			 try {
				 //try to convert whatever the user put in as the control port into a number
				 port = System::Int32::Parse(this->controlPort->Text);
			 }
			 catch (Exception^ ex) {
				 //if unsuccessful, revert to default port
				 port = 9051;
			 }
			 //if port is larger than valid port numbers, revert to default port
			 if (port > 65535) port = 9051;

			 //write control port
			 config->AppSettings->Settings->Remove("controlPort");
			 config->AppSettings->Settings->Add("controlPort",port.ToString());
			 //write opacity
			 config->AppSettings->Settings->Remove("opacity");
			 config->AppSettings->Settings->Add("opacity",this->opacitySelect->Value.ToString());
			 //save app.config
			 config->Save(System::Configuration::ConfigurationSaveMode::Modified);
			 System::Configuration::ConfigurationManager::RefreshSection("appSettings");
			 Close();
		 }
//Close the window and save no changes
private: System::Void cancelButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 Close();
		 }
//Set default options for advanced features
private: System::Void defaultButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->controlPort->Text = L"9051";
			 this->updateIn->Text = L"5";
			 this->checkBox1->Checked = false;
			 this->ontop->Checked = false;
			 this->opacitySelect->Value = 100;
			 this->taskBar->Checked = false;
		 }
};
}