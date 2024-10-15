#include "pch.h"
#include "OverlayCheckBoxForm.h"

using namespace MockingApplication;

OverlayCheckBoxForm::OverlayCheckBoxForm(void)
{
	InitializeComponent();
}
OverlayCheckBoxForm::~OverlayCheckBoxForm()
{
	if (components)
	{
		delete components;
	}
}
void OverlayCheckBoxForm::InitializeComponent(void)
{
    this->txt_lbl = (gcnew System::Windows::Forms::Label());
    this->checkedListBox = (gcnew System::Windows::Forms::CheckedListBox());
    this->next_btn = (gcnew System::Windows::Forms::Button());
    this->back_btn = (gcnew System::Windows::Forms::Button());
    this->advanced_btn = (gcnew System::Windows::Forms::Button());
    this->initial_btn = (gcnew System::Windows::Forms::Button());
    this->checkAllUncheckAll_btn = (gcnew System::Windows::Forms::Button());
    this->mockingSettings_btn = (gcnew System::Windows::Forms::Button());
    this->add_static_btn = (gcnew System::Windows::Forms::Button());
    this->SuspendLayout();
    // 
    // txt_lbl
    // 
    this->txt_lbl->AutoSize = true;
    this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->txt_lbl->Location = System::Drawing::Point(212, 34);
    this->txt_lbl->Name = L"txt_lbl";
    this->txt_lbl->Size = System::Drawing::Size(523, 28);
    this->txt_lbl->TabIndex = 0;
    this->txt_lbl->Text = L"Check the files you want to mock and click the next button.";
    // 
    // checkedListBox
    // 
    this->checkedListBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->checkedListBox->FormattingEnabled = true;
    this->checkedListBox->Location = System::Drawing::Point(77, 129);
    this->checkedListBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
    this->checkedListBox->Name = L"checkedListBox";
    this->checkedListBox->Size = System::Drawing::Size(810, 294);
    this->checkedListBox->TabIndex = 1;
    // 
    // next_btn
    // 
    this->next_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
    this->next_btn->Location = System::Drawing::Point(735, 472);
    this->next_btn->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
    this->next_btn->Name = L"next_btn";
    this->next_btn->Size = System::Drawing::Size(152, 37);
    this->next_btn->TabIndex = 2;
    this->next_btn->Text = L"Next";
    this->next_btn->UseVisualStyleBackColor = true;
    // 
    // back_btn
    // 
    this->back_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F));
    this->back_btn->Location = System::Drawing::Point(77, 472);
    this->back_btn->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
    this->back_btn->Name = L"back_btn";
    this->back_btn->Size = System::Drawing::Size(152, 37);
    this->back_btn->TabIndex = 3;
    this->back_btn->Text = L"Back";
    this->back_btn->UseVisualStyleBackColor = true;
    // 
    // advanced_btn
    // 
    this->advanced_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->advanced_btn->Location = System::Drawing::Point(764, 406);
    this->advanced_btn->Margin = System::Windows::Forms::Padding(4);
    this->advanced_btn->Name = L"advanced_btn";
    this->advanced_btn->Size = System::Drawing::Size(123, 32);
    this->advanced_btn->TabIndex = 4;
    this->advanced_btn->Text = L"Advanced";
    this->advanced_btn->UseVisualStyleBackColor = true;
    // 
    // initial_btn
    // 
    this->initial_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->initial_btn->Location = System::Drawing::Point(208, 90);
    this->initial_btn->Margin = System::Windows::Forms::Padding(4);
    this->initial_btn->Name = L"initial_btn";
    this->initial_btn->Size = System::Drawing::Size(123, 32);
    this->initial_btn->TabIndex = 5;
    this->initial_btn->Text = L"Initial State";
    this->initial_btn->UseVisualStyleBackColor = true;
    // 
    // checkAllUncheckAll_btn
    // 
    this->checkAllUncheckAll_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->checkAllUncheckAll_btn->Location = System::Drawing::Point(77, 90);
    this->checkAllUncheckAll_btn->Margin = System::Windows::Forms::Padding(4);
    this->checkAllUncheckAll_btn->Name = L"checkAllUncheckAll_btn";
    this->checkAllUncheckAll_btn->Size = System::Drawing::Size(123, 32);
    this->checkAllUncheckAll_btn->TabIndex = 6;
    this->checkAllUncheckAll_btn->Text = L"Check All";
    this->checkAllUncheckAll_btn->UseVisualStyleBackColor = true;
    // 
    // mockingSettings_btn
    // 
    this->mockingSettings_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->mockingSettings_btn->Location = System::Drawing::Point(728, 90);
    this->mockingSettings_btn->Margin = System::Windows::Forms::Padding(4);
    this->mockingSettings_btn->Name = L"mockingSettings_btn";
    this->mockingSettings_btn->Size = System::Drawing::Size(159, 32);
    this->mockingSettings_btn->TabIndex = 7;
    this->mockingSettings_btn->Text = L"Mocking Settings";
    this->mockingSettings_btn->UseVisualStyleBackColor = true;
    // 
    // add_static_btn
    // 
    this->add_static_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->add_static_btn->Location = System::Drawing::Point(529, 90);
    this->add_static_btn->Margin = System::Windows::Forms::Padding(4);
    this->add_static_btn->Name = L"add_static_btn";
    this->add_static_btn->Size = System::Drawing::Size(191, 32);
    this->add_static_btn->TabIndex = 8;
    this->add_static_btn->Text = L"Add Static To Files";
    this->add_static_btn->UseVisualStyleBackColor = true;
    // 
    // OverlayCheckBoxForm
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(967, 578);
    this->Controls->Add(this->add_static_btn);
    this->Controls->Add(this->mockingSettings_btn);
    this->Controls->Add(this->checkAllUncheckAll_btn);
    this->Controls->Add(this->initial_btn);
    this->Controls->Add(this->advanced_btn);
    this->Controls->Add(this->back_btn);
    this->Controls->Add(this->next_btn);
    this->Controls->Add(this->checkedListBox);
    this->Controls->Add(this->txt_lbl);
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
    this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
    this->Name = L"OverlayCheckBoxForm";
    this->Text = L"OverlayCheckBoxForm";
    this->ResumeLayout(false);
    this->PerformLayout();

}