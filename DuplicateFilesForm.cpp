#include "pch.h"
#include "DuplicateFilesForm.h"

using namespace MockingApplication;

DuplicateFilesForm::DuplicateFilesForm(void)
{
	InitializeComponent();
}
DuplicateFilesForm::~DuplicateFilesForm()
{
	if (components)
	{
		delete components;
	}
}
void DuplicateFilesForm::InitializeComponent(void)
{
	System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(DuplicateFilesForm::typeid));
	this->ok_btn = (gcnew System::Windows::Forms::Button());
	this->listBox1 = (gcnew System::Windows::Forms::ListBox());
	this->label1 = (gcnew System::Windows::Forms::Label());
	this->SuspendLayout();
	// 
	// ok_btn
	// 
	this->ok_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->ok_btn->Location = System::Drawing::Point(704, 472);
	this->ok_btn->Margin = System::Windows::Forms::Padding(4);
	this->ok_btn->Name = L"ok_btn";
	this->ok_btn->Size = System::Drawing::Size(155, 36);
	this->ok_btn->TabIndex = 0;
	this->ok_btn->Text = L"Ok";
	this->ok_btn->UseVisualStyleBackColor = true;
	this->ok_btn->Click += gcnew System::EventHandler(this, &DuplicateFilesForm::ok_btn_Click);
	// 
	// listBox1
	// 
	this->listBox1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->listBox1->FormattingEnabled = true;
	this->listBox1->ItemHeight = 28;
	this->listBox1->Location = System::Drawing::Point(96, 143);
	this->listBox1->Margin = System::Windows::Forms::Padding(4);
	this->listBox1->Name = L"listBox1";
	this->listBox1->Size = System::Drawing::Size(763, 312);
	this->listBox1->TabIndex = 1;
	// 
	// label1
	// 
	this->label1->AutoSize = true;
	this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label1->Location = System::Drawing::Point(117, 41);
	this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
	this->label1->Name = L"label1";
	this->label1->Size = System::Drawing::Size(680, 84);
	this->label1->TabIndex = 2;
	this->label1->Text = resources->GetString(L"label1.Text");
	// 
	// DuplicateFilesForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(967, 578);
	this->Controls->Add(this->label1);
	this->Controls->Add(this->listBox1);
	this->Controls->Add(this->ok_btn);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Margin = System::Windows::Forms::Padding(4);
	this->Name = L"DuplicateFilesForm";
	this->Text = L"DuplicateFilesForm";
	this->ResumeLayout(false);
	this->PerformLayout();

}
System::Void DuplicateFilesForm::ok_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}