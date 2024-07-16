#include "pch.h"
#include "MainForm.h"

using namespace CppCLRWinFormsProject;

MainForm::MainForm(void)
{
	InitializeComponent();
	this->pnlSideBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pnlSideBar_MouseDown);

	homeForm = gcnew MockingApplication::Home();
	homeForm->MdiParent = this;
	homeForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::HomeForm_FormClosed);
	homeForm->Dock = System::Windows::Forms::DockStyle::Fill;
	homeForm->Show();
}
MainForm::~MainForm()
{
	if (components)
	{
		delete components;
	}
}
System::Void MainForm::InitializeComponent(void)
{
	System::Resources::ResourceManager^ resources = (gcnew System::Resources::ResourceManager(L"MockingApplication.Resources", System::Reflection::Assembly::GetExecutingAssembly()));
	this->pnlSideBar = (gcnew System::Windows::Forms::Panel());
	this->nightControlBox1 = (gcnew ReaLTaiizor::Controls::NightControlBox());
	this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
	this->label1 = (gcnew System::Windows::Forms::Label());
	this->flowLayoutPanel1 = (gcnew System::Windows::Forms::FlowLayoutPanel());
	this->homeContainer = (gcnew System::Windows::Forms::Panel());
	this->homeButton = (gcnew System::Windows::Forms::Button());
	this->fileContainer = (gcnew System::Windows::Forms::Panel());
	this->mockOneFile_btn = (gcnew System::Windows::Forms::Button());
	this->filesContainer = (gcnew System::Windows::Forms::Panel());
	this->mockFiles_btn = (gcnew System::Windows::Forms::Button());
	this->pnlSideBar->SuspendLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
	this->flowLayoutPanel1->SuspendLayout();
	this->homeContainer->SuspendLayout();
	this->fileContainer->SuspendLayout();
	this->filesContainer->SuspendLayout();
	this->SuspendLayout();
	// 
	// pnlSideBar
	// 
	this->pnlSideBar->BackColor = System::Drawing::Color::White;
	this->pnlSideBar->Controls->Add(this->nightControlBox1);
	this->pnlSideBar->Controls->Add(this->pictureBox2);
	this->pnlSideBar->Controls->Add(this->label1);
	this->pnlSideBar->Dock = System::Windows::Forms::DockStyle::Top;
	this->pnlSideBar->Location = System::Drawing::Point(0, 0);
	this->pnlSideBar->Name = L"pnlSideBar";
	this->pnlSideBar->Size = System::Drawing::Size(905, 46);
	this->pnlSideBar->TabIndex = 1;
	this->pnlSideBar->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::pnlSideBar_MouseDown);
	// 
	// nightControlBox1
	// 
	this->nightControlBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
	this->nightControlBox1->BackColor = System::Drawing::Color::Transparent;
	this->nightControlBox1->CloseHoverColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(199)),
		static_cast<System::Int32>(static_cast<System::Byte>(80)), static_cast<System::Int32>(static_cast<System::Byte>(80)));
	this->nightControlBox1->CloseHoverForeColor = System::Drawing::Color::White;
	this->nightControlBox1->Cursor = System::Windows::Forms::Cursors::Hand;
	this->nightControlBox1->DefaultLocation = true;
	this->nightControlBox1->DisableMaximizeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(105)),
		static_cast<System::Int32>(static_cast<System::Byte>(105)), static_cast<System::Int32>(static_cast<System::Byte>(105)));
	this->nightControlBox1->DisableMinimizeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(105)),
		static_cast<System::Int32>(static_cast<System::Byte>(105)), static_cast<System::Int32>(static_cast<System::Byte>(105)));
	this->nightControlBox1->EnableCloseColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(160)),
		static_cast<System::Int32>(static_cast<System::Byte>(160)), static_cast<System::Int32>(static_cast<System::Byte>(160)));
	this->nightControlBox1->EnableMaximizeButton = true;
	this->nightControlBox1->EnableMaximizeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(160)),
		static_cast<System::Int32>(static_cast<System::Byte>(160)), static_cast<System::Int32>(static_cast<System::Byte>(160)));
	this->nightControlBox1->EnableMinimizeButton = true;
	this->nightControlBox1->EnableMinimizeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(160)),
		static_cast<System::Int32>(static_cast<System::Byte>(160)), static_cast<System::Int32>(static_cast<System::Byte>(160)));
	this->nightControlBox1->Location = System::Drawing::Point(766, 0);
	this->nightControlBox1->MaximizeHoverColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(15)),
		static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
	this->nightControlBox1->MaximizeHoverForeColor = System::Drawing::Color::White;
	this->nightControlBox1->MinimizeHoverColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(15)),
		static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)));
	this->nightControlBox1->MinimizeHoverForeColor = System::Drawing::Color::White;
	this->nightControlBox1->Name = L"nightControlBox1";
	this->nightControlBox1->Size = System::Drawing::Size(139, 31);
	this->nightControlBox1->TabIndex = 4;
	// 
	// pictureBox2
	// 
	this->pictureBox2->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"mockingBirdRounded")));
	this->pictureBox2->Location = System::Drawing::Point(14, 4);
	this->pictureBox2->Name = L"pictureBox2";
	this->pictureBox2->Size = System::Drawing::Size(38, 38);
	this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	this->pictureBox2->TabIndex = 3;
	this->pictureBox2->TabStop = false;
	// 
	// label1
	// 
	this->label1->AutoSize = true;
	this->label1->Font = (gcnew System::Drawing::Font(L"Showcard Gothic", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label1->Location = System::Drawing::Point(57, 11);
	this->label1->Name = L"label1";
	this->label1->Size = System::Drawing::Size(143, 23);
	this->label1->TabIndex = 2;
	this->label1->Text = L"MOCKINGBIRD";
	// 
	// flowLayoutPanel1
	// 
	this->flowLayoutPanel1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(23)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
		static_cast<System::Int32>(static_cast<System::Byte>(29)));
	this->flowLayoutPanel1->Controls->Add(this->homeContainer);
	this->flowLayoutPanel1->Controls->Add(this->fileContainer);
	this->flowLayoutPanel1->Controls->Add(this->filesContainer);
	this->flowLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Left;
	this->flowLayoutPanel1->Location = System::Drawing::Point(0, 46);
	this->flowLayoutPanel1->Name = L"flowLayoutPanel1";
	this->flowLayoutPanel1->Size = System::Drawing::Size(188, 438);
	this->flowLayoutPanel1->TabIndex = 2;
	// 
	// homeContainer
	// 
	this->homeContainer->Controls->Add(this->homeButton);
	this->homeContainer->Location = System::Drawing::Point(3, 3);
	this->homeContainer->Name = L"homeContainer";
	this->homeContainer->Size = System::Drawing::Size(185, 72);
	this->homeContainer->TabIndex = 4;
	// 
	// homeButton
	// 
	this->homeButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(23)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
		static_cast<System::Int32>(static_cast<System::Byte>(29)));
	this->homeButton->Cursor = System::Windows::Forms::Cursors::Default;
	this->homeButton->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(238)));
	this->homeButton->ForeColor = System::Drawing::Color::White;
	this->homeButton->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"home")));
	this->homeButton->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
	this->homeButton->Location = System::Drawing::Point(0, 0);
	this->homeButton->Name = L"homeButton";
	this->homeButton->Size = System::Drawing::Size(182, 69);
	this->homeButton->TabIndex = 3;
	this->homeButton->Text = L"Home";
	this->homeButton->UseVisualStyleBackColor = false;
	this->homeButton->Click += gcnew System::EventHandler(this, &MainForm::homeButton_Click);
	// 
	// fileContainer
	// 
	this->fileContainer->Controls->Add(this->mockOneFile_btn);
	this->fileContainer->Location = System::Drawing::Point(3, 81);
	this->fileContainer->Name = L"fileContainer";
	this->fileContainer->Size = System::Drawing::Size(185, 72);
	this->fileContainer->TabIndex = 5;
	// 
	// mockOneFile_btn
	// 
	this->mockOneFile_btn->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(23)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
		static_cast<System::Int32>(static_cast<System::Byte>(29)));
	this->mockOneFile_btn->Cursor = System::Windows::Forms::Cursors::Default;
	this->mockOneFile_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(238)));
	this->mockOneFile_btn->ForeColor = System::Drawing::Color::White;
	this->mockOneFile_btn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"bird")));
	this->mockOneFile_btn->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
	this->mockOneFile_btn->Location = System::Drawing::Point(0, 0);
	this->mockOneFile_btn->Name = L"mockOneFile_btn";
	this->mockOneFile_btn->Padding = System::Windows::Forms::Padding(0, 0, 25, 0);
	this->mockOneFile_btn->Size = System::Drawing::Size(182, 69);
	this->mockOneFile_btn->TabIndex = 3;
	this->mockOneFile_btn->Text = L"Mock single file";
	this->mockOneFile_btn->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
	this->mockOneFile_btn->UseVisualStyleBackColor = false;
	this->mockOneFile_btn->Click += gcnew System::EventHandler(this, &MainForm::mockOneFile_btn_Click);
	// 
	// filesContainer
	// 
	this->filesContainer->Controls->Add(this->mockFiles_btn);
	this->filesContainer->Location = System::Drawing::Point(3, 159);
	this->filesContainer->Name = L"filesContainer";
	this->filesContainer->Size = System::Drawing::Size(185, 72);
	this->filesContainer->TabIndex = 6;
	// 
	// mockFiles_btn
	// 
	this->mockFiles_btn->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(23)), static_cast<System::Int32>(static_cast<System::Byte>(24)),
		static_cast<System::Int32>(static_cast<System::Byte>(29)));
	this->mockFiles_btn->Cursor = System::Windows::Forms::Cursors::Default;
	this->mockFiles_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(238)));
	this->mockFiles_btn->ForeColor = System::Drawing::Color::White;
	this->mockFiles_btn->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"bird2")));
	this->mockFiles_btn->ImageAlign = System::Drawing::ContentAlignment::MiddleLeft;
	this->mockFiles_btn->Location = System::Drawing::Point(0, 0);
	this->mockFiles_btn->Name = L"mockFiles_btn";
	this->mockFiles_btn->Padding = System::Windows::Forms::Padding(0, 0, 25, 0);
	this->mockFiles_btn->Size = System::Drawing::Size(182, 69);
	this->mockFiles_btn->TabIndex = 3;
	this->mockFiles_btn->Text = L"Mock files with dependencies";
	this->mockFiles_btn->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
	this->mockFiles_btn->UseVisualStyleBackColor = false;
	this->mockFiles_btn->Click += gcnew System::EventHandler(this, &MainForm::mockFiles_btn_Click);
	// 
	// MainForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(905, 484);
	this->Controls->Add(this->flowLayoutPanel1);
	this->Controls->Add(this->pnlSideBar);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"mockingBirdIco")));
	this->IsMdiContainer = true;
	this->Name = L"MainForm";
	this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Show;
	this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
	this->Text = L"Mockingbird";
	this->pnlSideBar->ResumeLayout(false);
	this->pnlSideBar->PerformLayout();
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
	this->flowLayoutPanel1->ResumeLayout(false);
	this->homeContainer->ResumeLayout(false);
	this->fileContainer->ResumeLayout(false);
	this->filesContainer->ResumeLayout(false);
	this->ResumeLayout(false);
}
System::Void MainForm::homeButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (homeForm == nullptr)
	{
		homeForm = gcnew MockingApplication::Home();
		homeForm->MdiParent = this;
		homeForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::HomeForm_FormClosed);
		homeForm->Dock = System::Windows::Forms::DockStyle::Fill;
		homeForm->Show();
	}
	else
	{
		homeForm->Activate();
	}
}
System::Void MainForm::HomeForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	fileForm = nullptr;
}
System::Void MainForm::mockOneFile_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (fileForm == nullptr)
	{
		fileForm = gcnew MockingApplication::FileForm();
		fileForm->MdiParent = this;
		fileForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::FileForm_FormClosed);
		fileForm->Dock = System::Windows::Forms::DockStyle::Fill;
		fileForm->Show();
	}
	else
	{
		fileForm->Activate();
	}
}
System::Void MainForm::FileForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	fileForm = nullptr;
}
System::Void MainForm::mockFiles_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (filesForm == nullptr)
	{
		filesForm = gcnew MockingApplication::FilesForm();
		filesForm->MdiParent = this;
		filesForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::FilesForm_FormClosed);
		filesForm->Dock = System::Windows::Forms::DockStyle::Fill;
		filesForm->Show();
	}
	else
	{
		filesForm->Activate();
	}
}
System::Void MainForm::FilesForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	filesForm = nullptr;
}
System::Void MainForm::pnlSideBar_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	if (e->Button == System::Windows::Forms::MouseButtons::Left) {
		ReleaseCapture();
		SendMessage(this->Handle, WM_NCLBUTTONDOWN, HTCAPTION, IntPtr(0));
	}
}
