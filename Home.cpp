#include "pch.h"
#include "Home.h"

using namespace MockingApplication;

Home::Home(void)
{
	InitializeComponent();
}
Home::~Home()
{
	if (components)
	{
		delete components;
	}
}
void Home::InitializeComponent(void)
{
	System::Resources::ResourceManager^ resources = (gcnew System::Resources::ResourceManager(L"MockingApplication.Resources", System::Reflection::Assembly::GetExecutingAssembly()));
	this->labelTitle = (gcnew System::Windows::Forms::Label());
	this->labelDescription = (gcnew System::Windows::Forms::Label());
	this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
	this->SuspendLayout();
	// 
	// labelTitle
	// 
	this->labelTitle->AutoSize = true;
	this->labelTitle->Font = (gcnew System::Drawing::Font(L"Stencil", 40, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->labelTitle->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(50)), static_cast<System::Int32>(static_cast<System::Byte>(15)),
		static_cast<System::Int32>(static_cast<System::Byte>(15)));
	this->labelTitle->Location = System::Drawing::Point(27, 14);
	this->labelTitle->Name = L"labelTitle";
	this->labelTitle->Size = System::Drawing::Size(279, 128);
	this->labelTitle->TabIndex = 0;
	this->labelTitle->Text = L"WELCOME\r\n      TO";
	// 
	// labelDescription
	// 
	this->labelDescription->AutoSize = true;
	this->labelDescription->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->labelDescription->Location = System::Drawing::Point(311, 58);
	this->labelDescription->Name = L"labelDescription";
	this->labelDescription->Size = System::Drawing::Size(391, 336);
	this->labelDescription->TabIndex = 1;
	this->labelDescription->Text =
		"Mockingbird is an application developed\n"
		"by programmers to facilitate the writing of unit tests.\n"
		"The application is designed to automate the\n"
		"mocking of files and offers two primary functionalities :\n\n"

		"1. Mocking a Single File : Users can select an individual\n"
		"file, and the application will mock only that specific file.\n"
		"2. Mocking All Dependencies : Users can choose a root\n"
		"folder, and the application will analyze all.c and .h files\n"
		"within that folder to gather all included dependencies.\n\n"

		"By streamlining the process of mocking files and their\n"
		"dependencies, Mockingbird significantly reduces the\n"
		"time and effort required for unit testing, enabling\n"
		"developers to focus more on writing and improving\n"
		"their code.";

	// 
	// pictureBox1
	// 
	this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"mockingBirdFull")));
	this->pictureBox1->Location = System::Drawing::Point(41, 110);
	this->pictureBox1->Name = L"pictureBox1";
	this->pictureBox1->Size = System::Drawing::Size(252, 328);
	this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
	this->pictureBox1->TabIndex = 2;
	this->pictureBox1->TabStop = false;
	// 
	// Home
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(725, 470);
	this->Controls->Add(this->labelDescription);
	this->Controls->Add(this->labelTitle);
	this->Controls->Add(this->pictureBox1);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Name = L"Home";
	this->Text = L"Home";
	this->Load += gcnew System::EventHandler(this, &Home::homeForm_Load);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
	this->ResumeLayout(false);
	this->PerformLayout();

}
System::Void Home::homeForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	this->ControlBox = false;
}
