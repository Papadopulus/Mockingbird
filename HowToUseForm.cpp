#include "pch.h"
#include "HowToUseForm.h"

using namespace MockingApplication;

HowToUseForm::HowToUseForm(void)
{
	InitializeComponent();
	tempVideoPath = nullptr;
}
HowToUseForm::~HowToUseForm()
{
	if (components)
	{
		delete components;
	}
}
void HowToUseForm::InitializeComponent(void)
{
	System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(HowToUseForm::typeid));
	this->labelTitle = (gcnew System::Windows::Forms::Label());
	this->axWindowsMediaPlayer1 = (gcnew AxWMPLib::AxWindowsMediaPlayer());
	this->txt_lbl = (gcnew System::Windows::Forms::Label());
	this->start_guide_btn = (gcnew System::Windows::Forms::Button());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axWindowsMediaPlayer1))->BeginInit();
	this->SuspendLayout();
	// 
	// labelTitle
	// 
	this->labelTitle->AutoSize = true;
	this->labelTitle->Font = (gcnew System::Drawing::Font(L"Stencil", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->labelTitle->Location = System::Drawing::Point(222, 21);
	this->labelTitle->Name = L"labelTitle";
	this->labelTitle->Size = System::Drawing::Size(256, 32);
	this->labelTitle->TabIndex = 1;
	this->labelTitle->Text = L"HOW TO USE GUIDE";
	// 
	// axWindowsMediaPlayer1
	// 
	this->axWindowsMediaPlayer1->Enabled = true;
	this->axWindowsMediaPlayer1->Location = System::Drawing::Point(104, 67);
	this->axWindowsMediaPlayer1->Name = L"axWindowsMediaPlayer1";
	this->axWindowsMediaPlayer1->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"axWindowsMediaPlayer1.OcxState")));
	this->axWindowsMediaPlayer1->Size = System::Drawing::Size(489, 261);
	this->axWindowsMediaPlayer1->TabIndex = 2;
	// 
	// txt_lbl
	// 
	this->txt_lbl->AutoSize = true;
	this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->txt_lbl->Location = System::Drawing::Point(101, 341);
	this->txt_lbl->Name = L"txt_lbl";
	this->txt_lbl->Size = System::Drawing::Size(499, 34);
	this->txt_lbl->TabIndex = 3;
	this->txt_lbl->Text = L"This guide will walk you through the steps to effectively use Mockingbird, from\r\n"
		L"mocking single files to handling complex dependencies. Press \'Start Guide\' to be"
		L"gin.";
	// 
	// start_guide_btn
	// 
	this->start_guide_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
	this->start_guide_btn->Location = System::Drawing::Point(286, 386);
	this->start_guide_btn->Margin = System::Windows::Forms::Padding(2);
	this->start_guide_btn->Name = L"start_guide_btn";
	this->start_guide_btn->Size = System::Drawing::Size(114, 30);
	this->start_guide_btn->TabIndex = 4;
	this->start_guide_btn->Text = L"Start Guide";
	this->start_guide_btn->UseVisualStyleBackColor = true;
	this->start_guide_btn->Click += gcnew System::EventHandler(this, &HowToUseForm::start_guide_btn_Click);
	// 
	// HowToUseForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(725, 470);
	this->Controls->Add(this->start_guide_btn);
	this->Controls->Add(this->txt_lbl);
	this->Controls->Add(this->axWindowsMediaPlayer1);
	this->Controls->Add(this->labelTitle);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Name = L"HowToUseForm";
	this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
	this->Text = L"HowToUseForm";
	this->Load += gcnew System::EventHandler(this, &HowToUseForm::HowToUseForm_Load);
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axWindowsMediaPlayer1))->EndInit();
	this->ResumeLayout(false);
	this->PerformLayout();

}
System::Void HowToUseForm::start_guide_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (guideStepForm == nullptr)
	{
		guideStepForm = gcnew GuideStepForm();
		guideStepForm->StartPosition = FormStartPosition::Manual;
		guideStepForm->MdiParent = this->MdiParent;
		guideStepForm->Dock = System::Windows::Forms::DockStyle::Fill;
		guideStepForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &HowToUseForm::GuideStepForm_FormClosed);
		guideStepForm->Show();
	}
	else
	{
		guideStepForm->Activate();
	}
	this->Close();

}
System::Void HowToUseForm::GuideStepForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	// Set the overlay form to null when it is closed
	guideStepForm = nullptr;
}
System::Void HowToUseForm::HowToUseForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	if (tempVideoPath == nullptr || !File::Exists(tempVideoPath))
	{
		tempVideoPath = Path::GetTempFileName() + ".mp4";
		System::Resources::ResourceManager^ resourceManager = (gcnew System::Resources::ResourceManager(L"MockingApplication.Resources", System::Reflection::Assembly::GetExecutingAssembly()));
		array<Byte>^ resourceData = (array<Byte>^)resourceManager->GetObject(L"HowToUseVideo");

		File::WriteAllBytes(tempVideoPath, resourceData);

	}

	// Set file path for Windows Media Player
	axWindowsMediaPlayer1->URL = tempVideoPath;
	axWindowsMediaPlayer1->Ctlcontrols->play();
}