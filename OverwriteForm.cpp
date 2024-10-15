#include "pch.h"
#include "OverwriteForm.h"

using namespace MockingApplication;

OverwriteForm::OverwriteForm(void)
{
	InitializeComponent();
}
OverwriteForm::~OverwriteForm()
{
	if (components)
	{
		delete components;
	}
}
void OverwriteForm::InitializeComponent(void)
{
	this->cfgCheckedListBox = (gcnew System::Windows::Forms::CheckedListBox());
	this->cfgCheckedListBox->HorizontalScrollbar = true;
	this->overwrite_next_btn = (gcnew System::Windows::Forms::Button());
	this->label1 = (gcnew System::Windows::Forms::Label());
	this->libCheckedListBox = (gcnew System::Windows::Forms::CheckedListBox());
	this->libCheckedListBox->HorizontalScrollbar = true;
	this->otherCheckedListBox = (gcnew System::Windows::Forms::CheckedListBox());
	this->otherCheckedListBox->HorizontalScrollbar = true;
	this->cfg_lbl = (gcnew System::Windows::Forms::Label());
	this->lib_lbl = (gcnew System::Windows::Forms::Label());
	this->other_lbl = (gcnew System::Windows::Forms::Label());
	this->back_btn = (gcnew System::Windows::Forms::Button());
	this->SuspendLayout();
	// 
	// cfgCheckedListBox
	// 
	this->cfgCheckedListBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->cfgCheckedListBox->FormattingEnabled = true;
	this->cfgCheckedListBox->Location = System::Drawing::Point(44, 112);
	this->cfgCheckedListBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->cfgCheckedListBox->Name = L"cfgCheckedListBox";
	this->cfgCheckedListBox->Size = System::Drawing::Size(265, 352);
	this->cfgCheckedListBox->TabIndex = 0;
	// 
	// overwrite_next_btn
	// 
	this->overwrite_next_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->overwrite_next_btn->Location = System::Drawing::Point(735, 472);
	this->overwrite_next_btn->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->overwrite_next_btn->Name = L"overwrite_next_btn";
	this->overwrite_next_btn->Size = System::Drawing::Size(152, 37);
	this->overwrite_next_btn->TabIndex = 1;
	this->overwrite_next_btn->Text = L"Next";
	this->overwrite_next_btn->UseVisualStyleBackColor = true;
	// 
	// label1
	// 
	this->label1->AutoSize = true;
	this->label1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->label1->Location = System::Drawing::Point(72, 26);
	this->label1->Name = L"label1";
	this->label1->Size = System::Drawing::Size(769, 28);
	this->label1->TabIndex = 2;
	this->label1->Text = L"These files have already been mocked. Check the files below that you want to over"
		L"write.";
	// 
	// libCheckedListBox
	// 
	this->libCheckedListBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->libCheckedListBox->FormattingEnabled = true;
	this->libCheckedListBox->Location = System::Drawing::Point(342, 112);
	this->libCheckedListBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->libCheckedListBox->Name = L"libCheckedListBox";
	this->libCheckedListBox->Size = System::Drawing::Size(265, 352);
	this->libCheckedListBox->TabIndex = 3;
	// 
	// otherCheckedListBox
	// 
	this->otherCheckedListBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->otherCheckedListBox->FormattingEnabled = true;
	this->otherCheckedListBox->Location = System::Drawing::Point(640, 112);
	this->otherCheckedListBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->otherCheckedListBox->Name = L"otherCheckedListBox";
	this->otherCheckedListBox->Size = System::Drawing::Size(265, 352);
	this->otherCheckedListBox->TabIndex = 4;
	// 
	// cfg_lbl
	// 
	this->cfg_lbl->AutoSize = true;
	this->cfg_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->cfg_lbl->Location = System::Drawing::Point(72, 68);
	this->cfg_lbl->Name = L"cfg_lbl";
	this->cfg_lbl->Size = System::Drawing::Size(190, 28);
	this->cfg_lbl->TabIndex = 5;
	this->cfg_lbl->Text = L"Files from cfg folder:";
	// 
	// lib_lbl
	// 
	this->lib_lbl->AutoSize = true;
	this->lib_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->lib_lbl->Location = System::Drawing::Point(373, 68);
	this->lib_lbl->Name = L"lib_lbl";
	this->lib_lbl->Size = System::Drawing::Size(185, 28);
	this->lib_lbl->TabIndex = 6;
	this->lib_lbl->Text = L"Files from lib folder:";
	// 
	// other_lbl
	// 
	this->other_lbl->AutoSize = true;
	this->other_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->other_lbl->Location = System::Drawing::Point(658, 68);
	this->other_lbl->Name = L"other_lbl";
	this->other_lbl->Size = System::Drawing::Size(218, 28);
	this->other_lbl->TabIndex = 7;
	this->other_lbl->Text = L"Files from other folders:";
	// 
	// back_btn
	// 
	this->back_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->back_btn->Location = System::Drawing::Point(77, 472);
	this->back_btn->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->back_btn->Name = L"back_btn";
	this->back_btn->Size = System::Drawing::Size(152, 37);
	this->back_btn->TabIndex = 8;
	this->back_btn->Text = L"Back";
	this->back_btn->UseVisualStyleBackColor = true;
	this->back_btn->Click += gcnew System::EventHandler(this, &OverwriteForm::back_btn_Click);
	// 
	// OverwriteForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(967, 578);
	this->Controls->Add(this->back_btn);
	this->Controls->Add(this->other_lbl);
	this->Controls->Add(this->lib_lbl);
	this->Controls->Add(this->cfg_lbl);
	this->Controls->Add(this->otherCheckedListBox);
	this->Controls->Add(this->libCheckedListBox);
	this->Controls->Add(this->label1);
	this->Controls->Add(this->overwrite_next_btn);
	this->Controls->Add(this->cfgCheckedListBox);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
	this->Name = L"OverwriteForm";
	this->Text = L"OverwriteForm";
	this->ResumeLayout(false);
	this->PerformLayout();

}
System::Void OverwriteForm::back_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	this->Close();
}