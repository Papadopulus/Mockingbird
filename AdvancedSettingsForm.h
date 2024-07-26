#pragma once
#include <fstream>
#include "CommonFunctions.h"

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for AdvancedSettingsForm
	/// </summary>
	public ref class AdvancedSettingsForm : public System::Windows::Forms::Form
	{
	public:
		AdvancedSettingsForm(void)
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
		~AdvancedSettingsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::ListBox^ listBox;
	protected:

	protected:
	private: System::Windows::Forms::Button^ add_btn;
	private: System::Windows::Forms::Button^ remove_btn;

	public: System::Windows::Forms::Button^ apply_btn;
	private: System::Windows::Forms::Label^ txt_lbl;
	private: System::Windows::Forms::TextBox^ textBox;
	private: System::Windows::Forms::Panel^ panel;

	private: System::Windows::Forms::Label^ title_lbl;
	public: System::Windows::Forms::PictureBox^ pictureBox1;



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
			System::Resources::ResourceManager^ resources = (gcnew System::Resources::ResourceManager(L"MockingApplication.Resources", System::Reflection::Assembly::GetExecutingAssembly()));
			this->listBox = (gcnew System::Windows::Forms::ListBox());
			this->add_btn = (gcnew System::Windows::Forms::Button());
			this->remove_btn = (gcnew System::Windows::Forms::Button());
			this->apply_btn = (gcnew System::Windows::Forms::Button());
			this->txt_lbl = (gcnew System::Windows::Forms::Label());
			this->textBox = (gcnew System::Windows::Forms::TextBox());
			this->panel = (gcnew System::Windows::Forms::Panel());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->title_lbl = (gcnew System::Windows::Forms::Label());
			this->panel->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// listBox
			// 
			this->listBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->listBox->FormattingEnabled = true;
			this->listBox->ItemHeight = 21;
			this->listBox->Location = System::Drawing::Point(22, 193);
			this->listBox->Margin = System::Windows::Forms::Padding(4);
			this->listBox->Name = L"listBox";
			this->listBox->Size = System::Drawing::Size(198, 298);
			this->listBox->TabIndex = 0;
			// 
			// add_btn
			// 
			this->add_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->add_btn->Location = System::Drawing::Point(239, 323);
			this->add_btn->Margin = System::Windows::Forms::Padding(4);
			this->add_btn->Name = L"add_btn";
			this->add_btn->Size = System::Drawing::Size(156, 33);
			this->add_btn->TabIndex = 1;
			this->add_btn->Text = L"Add";
			this->add_btn->UseVisualStyleBackColor = true;
			this->add_btn->Click += gcnew System::EventHandler(this, &AdvancedSettingsForm::add_btn_Click);
			// 
			// remove_btn
			// 
			this->remove_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->remove_btn->Location = System::Drawing::Point(240, 408);
			this->remove_btn->Margin = System::Windows::Forms::Padding(4);
			this->remove_btn->Name = L"remove_btn";
			this->remove_btn->Size = System::Drawing::Size(156, 33);
			this->remove_btn->TabIndex = 2;
			this->remove_btn->Text = L"Remove";
			this->remove_btn->UseVisualStyleBackColor = true;
			this->remove_btn->Click += gcnew System::EventHandler(this, &AdvancedSettingsForm::remove_btn_Click);
			// 
			// apply_btn
			// 
			this->apply_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->apply_btn->Location = System::Drawing::Point(122, 532);
			this->apply_btn->Margin = System::Windows::Forms::Padding(4);
			this->apply_btn->Name = L"apply_btn";
			this->apply_btn->Size = System::Drawing::Size(205, 33);
			this->apply_btn->TabIndex = 4;
			this->apply_btn->Text = L"Apply";
			this->apply_btn->UseVisualStyleBackColor = true;
			// 
			// txt_lbl
			// 
			this->txt_lbl->AutoSize = true;
			this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txt_lbl->Location = System::Drawing::Point(31, 84);
			this->txt_lbl->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->txt_lbl->Name = L"txt_lbl";
			this->txt_lbl->Size = System::Drawing::Size(331, 84);
			this->txt_lbl->TabIndex = 5;
			this->txt_lbl->Text = L"Here you can add and remove items \r\nfrom the list of #include directives \r\nyou wa"
				L"nt to exclude.";
			// 
			// textBox
			// 
			this->textBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textBox->Location = System::Drawing::Point(240, 244);
			this->textBox->Margin = System::Windows::Forms::Padding(4);
			this->textBox->Name = L"textBox";
			this->textBox->Size = System::Drawing::Size(155, 29);
			this->textBox->TabIndex = 6;
			// 
			// panel
			// 
			this->panel->BackColor = System::Drawing::Color::White;
			this->panel->Controls->Add(this->pictureBox1);
			this->panel->Controls->Add(this->title_lbl);
			this->panel->Location = System::Drawing::Point(0, 0);
			this->panel->Margin = System::Windows::Forms::Padding(4);
			this->panel->Name = L"panel";
			this->panel->Size = System::Drawing::Size(533, 57);
			this->panel->TabIndex = 7;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"back")));
			this->pictureBox1->Location = System::Drawing::Point(360, -2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(59, 50);
			this->pictureBox1->TabIndex = 8;
			this->pictureBox1->TabStop = false;
			// 
			// title_lbl
			// 
			this->title_lbl->AutoSize = true;
			this->title_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->title_lbl->Location = System::Drawing::Point(12, 12);
			this->title_lbl->Name = L"title_lbl";
			this->title_lbl->Size = System::Drawing::Size(188, 28);
			this->title_lbl->TabIndex = 0;
			this->title_lbl->Text = L"Advanced Settings";
			// 
			// AdvancedSettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(421, 596);
			this->Controls->Add(this->panel);
			this->Controls->Add(this->textBox);
			this->Controls->Add(this->txt_lbl);
			this->Controls->Add(this->apply_btn);
			this->Controls->Add(this->remove_btn);
			this->Controls->Add(this->add_btn);
			this->Controls->Add(this->listBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"mockingBirdIco")));
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"AdvancedSettingsForm";
			this->Text = L"AdvancedSettingsForm";
			this->Load += gcnew System::EventHandler(this, &AdvancedSettingsForm::AdvancedSettingsForm_Load);
			this->panel->ResumeLayout(false);
			this->panel->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void add_btn_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		// Add item to list box
		if (!String::IsNullOrEmpty(textBox->Text))
		{
			listBox->Items->Add(textBox->Text);
			textBox->Clear();
		}
	}
private: System::Void remove_btn_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		// Remove selected item from list box
		if (listBox->SelectedIndex != -1) 
		{
			listBox->Items->RemoveAt(listBox->SelectedIndex);
		}
	}

private: System::Void AdvancedSettingsForm_Load(System::Object^ sender, System::EventArgs^ e) 
	{
		// Load excluded includes from file or settings
		std::ifstream file("excluded_includes.txt");
		std::string line;
		while (std::getline(file, line)) {
			listBox->Items->Add(gcnew String(line.c_str()));
		}
		file.close();
	}
};
}
