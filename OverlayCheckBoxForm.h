#pragma once
#include <vector>
#include <string>
#include "CommonFunctions.h"
namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class OverlayCheckBoxForm : public System::Windows::Forms::Form
	{
	public:
		OverlayCheckBoxForm(void)
		{
			InitializeComponent();
		}

	protected:
		~OverlayCheckBoxForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ txt_lbl;
	protected:
	public: System::Windows::Forms::CheckedListBox^ checkedListBox;
	public: System::Windows::Forms::Button^ next_btn;
	public: System::Windows::Forms::Button^ back_btn;



	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->txt_lbl = (gcnew System::Windows::Forms::Label());
			this->checkedListBox = (gcnew System::Windows::Forms::CheckedListBox());
			this->next_btn = (gcnew System::Windows::Forms::Button());
			this->back_btn = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// txt_lbl
			// 
			this->txt_lbl->AutoSize = true;
			this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txt_lbl->Location = System::Drawing::Point(229, 41);
			this->txt_lbl->Name = L"txt_lbl";
			this->txt_lbl->Size = System::Drawing::Size(414, 21);
			this->txt_lbl->TabIndex = 0;
			this->txt_lbl->Text = L"Check the files you want to mock and click the next button.";
			// 
			// checkedListBox
			// 
			this->checkedListBox->FormattingEnabled = true;
			this->checkedListBox->Location = System::Drawing::Point(112, 98);
			this->checkedListBox->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->checkedListBox->Name = L"checkedListBox";
			this->checkedListBox->Size = System::Drawing::Size(773, 327);
			this->checkedListBox->TabIndex = 1;
			// 
			// next_btn
			// 
			this->next_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->next_btn->Location = System::Drawing::Point(735, 470);
			this->next_btn->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->next_btn->Name = L"next_btn";
			this->next_btn->Size = System::Drawing::Size(152, 37);
			this->next_btn->TabIndex = 2;
			this->next_btn->Text = L"Next";
			this->next_btn->UseVisualStyleBackColor = true;
			// 
			// back_btn
			// 
			this->back_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->back_btn->Location = System::Drawing::Point(112, 470);
			this->back_btn->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->back_btn->Name = L"back_btn";
			this->back_btn->Size = System::Drawing::Size(152, 37);
			this->back_btn->TabIndex = 3;
			this->back_btn->Text = L"Back";
			this->back_btn->UseVisualStyleBackColor = true;
			// 
			// OverlayCheckBoxForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(967, 578);
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
#pragma endregion
	
	};
}
