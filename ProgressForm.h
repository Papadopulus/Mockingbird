#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ProgressForm
	/// </summary>
	public ref class ProgressForm : public System::Windows::Forms::Form
	{
	public:
		ProgressForm(void)
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
		~ProgressForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: 

		System::Windows::Forms::Label^ status_lbl;
		ReaLTaiizor::Controls::MetroProgressBar^ metroProgressBar1;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->status_lbl = (gcnew System::Windows::Forms::Label());
			this->metroProgressBar1 = (gcnew ReaLTaiizor::Controls::MetroProgressBar());
			this->SuspendLayout();
			// 
			// status_lbl
			// 
			this->status_lbl->AutoSize = true;
			this->status_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->status_lbl->Location = System::Drawing::Point(399, 290);
			this->status_lbl->Name = L"status_lbl";
			this->status_lbl->Size = System::Drawing::Size(65, 28);
			this->status_lbl->TabIndex = 1;
			this->status_lbl->Text = L"label1";
			// 
			// metroProgressBar1
			// 
			this->metroProgressBar1->BackgroundColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(38)),
				static_cast<System::Int32>(static_cast<System::Byte>(38)), static_cast<System::Int32>(static_cast<System::Byte>(38)));
			this->metroProgressBar1->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(38)),
				static_cast<System::Int32>(static_cast<System::Byte>(38)), static_cast<System::Int32>(static_cast<System::Byte>(38)));
			this->metroProgressBar1->DisabledBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(38)),
				static_cast<System::Int32>(static_cast<System::Byte>(38)), static_cast<System::Int32>(static_cast<System::Byte>(38)));
			this->metroProgressBar1->DisabledBorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(38)),
				static_cast<System::Int32>(static_cast<System::Byte>(38)), static_cast<System::Int32>(static_cast<System::Byte>(38)));
			this->metroProgressBar1->DisabledProgressColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(120)),
				static_cast<System::Int32>(static_cast<System::Byte>(65)), static_cast<System::Int32>(static_cast<System::Byte>(177)), static_cast<System::Int32>(static_cast<System::Byte>(225)));
			this->metroProgressBar1->ForeColor = System::Drawing::SystemColors::ControlText;
			this->metroProgressBar1->IsDerivedStyle = true;
			this->metroProgressBar1->Location = System::Drawing::Point(145, 243);
			this->metroProgressBar1->Maximum = 100;
			this->metroProgressBar1->Minimum = 0;
			this->metroProgressBar1->Name = L"metroProgressBar1";
			this->metroProgressBar1->Orientation = ReaLTaiizor::Enum::Metro::ProgressOrientation::Horizontal;
			this->metroProgressBar1->ProgressColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(65)),
				static_cast<System::Int32>(static_cast<System::Byte>(177)), static_cast<System::Int32>(static_cast<System::Byte>(225)));
			this->metroProgressBar1->Size = System::Drawing::Size(664, 28);
			this->metroProgressBar1->Style = ReaLTaiizor::Enum::Metro::Style::Dark;
			this->metroProgressBar1->StyleManager = nullptr;
			this->metroProgressBar1->TabIndex = 2;
			this->metroProgressBar1->Text = L"metroProgressBar1";
			this->metroProgressBar1->ThemeAuthor = L"Taiizor";
			this->metroProgressBar1->ThemeName = L"MetroDark";
			this->metroProgressBar1->Value = 0;
			// 
			// ProgressForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(949, 531);
			this->Controls->Add(this->metroProgressBar1);
			this->Controls->Add(this->status_lbl);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"ProgressForm";
			this->Text = L"ProgressForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
