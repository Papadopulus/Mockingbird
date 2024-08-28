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
    this->SuspendLayout();
    // 
    // txt_lbl
    // 
    this->txt_lbl->AutoSize = true;
    this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->txt_lbl->Location = System::Drawing::Point(172, 33);
    this->txt_lbl->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
    this->txt_lbl->Name = L"txt_lbl";
    this->txt_lbl->Size = System::Drawing::Size(414, 21);
    this->txt_lbl->TabIndex = 0;
    this->txt_lbl->Text = L"Check the files you want to mock and click the next button.";
    // 
    // checkedListBox
    // 
    this->checkedListBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->checkedListBox->FormattingEnabled = true;
    this->checkedListBox->Location = System::Drawing::Point(86, 128);
    this->checkedListBox->Margin = System::Windows::Forms::Padding(2);
    this->checkedListBox->Name = L"checkedListBox";
    this->checkedListBox->Size = System::Drawing::Size(581, 220);
    this->checkedListBox->TabIndex = 1;
    // 
    // next_btn
    // 
    this->next_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->next_btn->Location = System::Drawing::Point(551, 391);
    this->next_btn->Margin = System::Windows::Forms::Padding(2);
    this->next_btn->Name = L"next_btn";
    this->next_btn->Size = System::Drawing::Size(114, 30);
    this->next_btn->TabIndex = 2;
    this->next_btn->Text = L"Next";
    this->next_btn->UseVisualStyleBackColor = true;
    // 
    // back_btn
    // 
    this->back_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->back_btn->Location = System::Drawing::Point(84, 391);
    this->back_btn->Margin = System::Windows::Forms::Padding(2);
    this->back_btn->Name = L"back_btn";
    this->back_btn->Size = System::Drawing::Size(114, 30);
    this->back_btn->TabIndex = 3;
    this->back_btn->Text = L"Back";
    this->back_btn->UseVisualStyleBackColor = true;
    // 
    // advanced_btn
    // 
    this->advanced_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->advanced_btn->Location = System::Drawing::Point(563, 353);
    this->advanced_btn->Name = L"advanced_btn";
    this->advanced_btn->Size = System::Drawing::Size(92, 26);
    this->advanced_btn->TabIndex = 4;
    this->advanced_btn->Text = L"Advanced";
    this->advanced_btn->UseVisualStyleBackColor = true;
    // 
    // initial_btn
    // 
    this->initial_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->initial_btn->Location = System::Drawing::Point(184, 97);
    this->initial_btn->Name = L"initial_btn";
    this->initial_btn->Size = System::Drawing::Size(92, 26);
    this->initial_btn->TabIndex = 5;
    this->initial_btn->Text = L"Initial State";
    this->initial_btn->UseVisualStyleBackColor = true;
    // 
    // checkAllUncheckAll_btn
    // 
    this->checkAllUncheckAll_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->checkAllUncheckAll_btn->Location = System::Drawing::Point(86, 97);
    this->checkAllUncheckAll_btn->Name = L"checkAllUncheckAll_btn";
    this->checkAllUncheckAll_btn->Size = System::Drawing::Size(92, 26);
    this->checkAllUncheckAll_btn->TabIndex = 6;
    this->checkAllUncheckAll_btn->Text = L"Check All";
    this->checkAllUncheckAll_btn->UseVisualStyleBackColor = true;
    // 
    // mockingSettings_btn
    // 
    this->mockingSettings_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
        static_cast<System::Byte>(0)));
    this->mockingSettings_btn->Location = System::Drawing::Point(546, 97);
    this->mockingSettings_btn->Name = L"mockingSettings_btn";
    this->mockingSettings_btn->Size = System::Drawing::Size(119, 26);
    this->mockingSettings_btn->TabIndex = 7;
    this->mockingSettings_btn->Text = L"Mocking Settings";
    this->mockingSettings_btn->UseVisualStyleBackColor = true;
    // 
    // OverlayCheckBoxForm
    // 
    this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
    this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
    this->ClientSize = System::Drawing::Size(725, 470);
    this->Controls->Add(this->mockingSettings_btn);
    this->Controls->Add(this->checkAllUncheckAll_btn);
    this->Controls->Add(this->initial_btn);
    this->Controls->Add(this->advanced_btn);
    this->Controls->Add(this->back_btn);
    this->Controls->Add(this->next_btn);
    this->Controls->Add(this->checkedListBox);
    this->Controls->Add(this->txt_lbl);
    this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
    this->Margin = System::Windows::Forms::Padding(2);
    this->Name = L"OverlayCheckBoxForm";
    this->Text = L"OverlayCheckBoxForm";
    this->ResumeLayout(false);
    this->PerformLayout();

}