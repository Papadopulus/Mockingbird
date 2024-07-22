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

	/// <summary>
	/// Summary for OverlayCheckBoxForm
	/// </summary>
	public ref class OverlayCheckBoxForm : public System::Windows::Forms::Form
	{
	public:
		OverlayCheckBoxForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
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


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->txt_lbl = (gcnew System::Windows::Forms::Label());
			this->checkedListBox = (gcnew System::Windows::Forms::CheckedListBox());
			this->next_btn = (gcnew System::Windows::Forms::Button());
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
			this->checkedListBox->FormattingEnabled = true;
			this->checkedListBox->Location = System::Drawing::Point(84, 80);
			this->checkedListBox->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->checkedListBox->Name = L"checkedListBox";
			this->checkedListBox->Size = System::Drawing::Size(581, 274);
			this->checkedListBox->TabIndex = 1;
			// 
			// next_btn
			// 
			this->next_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->next_btn->Location = System::Drawing::Point(551, 382);
			this->next_btn->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->next_btn->Name = L"next_btn";
			this->next_btn->Size = System::Drawing::Size(114, 30);
			this->next_btn->TabIndex = 2;
			this->next_btn->Text = L"Next";
			this->next_btn->UseVisualStyleBackColor = true;
			// 
			// OverlayCheckBoxForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(725, 470);
			this->Controls->Add(this->next_btn);
			this->Controls->Add(this->checkedListBox);
			this->Controls->Add(this->txt_lbl);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Margin = System::Windows::Forms::Padding(2, 2, 2, 2);
			this->Name = L"OverlayCheckBoxForm";
			this->Text = L"OverlayCheckBoxForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	
	};
}
