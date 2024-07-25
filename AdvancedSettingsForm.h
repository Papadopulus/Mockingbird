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
	private: System::Windows::Forms::Button^ cancel_btn;
	public: System::Windows::Forms::Button^ apply_btn;
	private: System::Windows::Forms::Label^ txt_lbl;
	private: System::Windows::Forms::TextBox^ textBox;
	private: System::Windows::Forms::Panel^ panel;



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
			this->listBox = (gcnew System::Windows::Forms::ListBox());
			this->add_btn = (gcnew System::Windows::Forms::Button());
			this->remove_btn = (gcnew System::Windows::Forms::Button());
			this->cancel_btn = (gcnew System::Windows::Forms::Button());
			this->apply_btn = (gcnew System::Windows::Forms::Button());
			this->txt_lbl = (gcnew System::Windows::Forms::Label());
			this->textBox = (gcnew System::Windows::Forms::TextBox());
			this->panel = (gcnew System::Windows::Forms::Panel());
			this->SuspendLayout();
			// 
			// listBox
			// 
			this->listBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->listBox->FormattingEnabled = true;
			this->listBox->ItemHeight = 17;
			this->listBox->Location = System::Drawing::Point(28, 173);
			this->listBox->Name = L"listBox";
			this->listBox->Size = System::Drawing::Size(185, 208);
			this->listBox->TabIndex = 0;
			// 
			// add_btn
			// 
			this->add_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->add_btn->Location = System::Drawing::Point(242, 263);
			this->add_btn->Name = L"add_btn";
			this->add_btn->Size = System::Drawing::Size(117, 27);
			this->add_btn->TabIndex = 1;
			this->add_btn->Text = L"Add";
			this->add_btn->UseVisualStyleBackColor = true;
			this->add_btn->Click += gcnew System::EventHandler(this, &AdvancedSettingsForm::add_btn_Click);
			// 
			// remove_btn
			// 
			this->remove_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->remove_btn->Location = System::Drawing::Point(242, 326);
			this->remove_btn->Name = L"remove_btn";
			this->remove_btn->Size = System::Drawing::Size(117, 27);
			this->remove_btn->TabIndex = 2;
			this->remove_btn->Text = L"Remove";
			this->remove_btn->UseVisualStyleBackColor = true;
			this->remove_btn->Click += gcnew System::EventHandler(this, &AdvancedSettingsForm::remove_btn_Click);
			// 
			// cancel_btn
			// 
			this->cancel_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->cancel_btn->Location = System::Drawing::Point(28, 420);
			this->cancel_btn->Name = L"cancel_btn";
			this->cancel_btn->Size = System::Drawing::Size(117, 27);
			this->cancel_btn->TabIndex = 3;
			this->cancel_btn->Text = L"Cancel";
			this->cancel_btn->UseVisualStyleBackColor = true;
			this->cancel_btn->Click += gcnew System::EventHandler(this, &AdvancedSettingsForm::cancel_btn_Click);
			// 
			// apply_btn
			// 
			this->apply_btn->Location = System::Drawing::Point(242, 420);
			this->apply_btn->Name = L"apply_btn";
			this->apply_btn->Size = System::Drawing::Size(117, 27);
			this->apply_btn->TabIndex = 4;
			this->apply_btn->Text = L"Apply";
			this->apply_btn->UseVisualStyleBackColor = true;
			// 
			// txt_lbl
			// 
			this->txt_lbl->AutoSize = true;
			this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txt_lbl->Location = System::Drawing::Point(24, 79);
			this->txt_lbl->Name = L"txt_lbl";
			this->txt_lbl->Size = System::Drawing::Size(352, 42);
			this->txt_lbl->TabIndex = 5;
			this->txt_lbl->Text = L"Here you can add and remove items from the list \r\nof #include directives you want"
				L" to exclude.";
			// 
			// textBox
			// 
			this->textBox->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->textBox->Location = System::Drawing::Point(242, 206);
			this->textBox->Name = L"textBox";
			this->textBox->Size = System::Drawing::Size(117, 25);
			this->textBox->TabIndex = 6;
			// 
			// panel
			// 
			this->panel->BackColor = System::Drawing::Color::White;
			this->panel->Location = System::Drawing::Point(0, 0);
			this->panel->Name = L"panel";
			this->panel->Size = System::Drawing::Size(400, 46);
			this->panel->TabIndex = 7;
			this->panel->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &AdvancedSettingsForm::panel_Mouse_Down);
			// 
			// AdvancedSettingsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(398, 484);
			this->Controls->Add(this->panel);
			this->Controls->Add(this->textBox);
			this->Controls->Add(this->txt_lbl);
			this->Controls->Add(this->apply_btn);
			this->Controls->Add(this->cancel_btn);
			this->Controls->Add(this->remove_btn);
			this->Controls->Add(this->add_btn);
			this->Controls->Add(this->listBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Name = L"AdvancedSettingsForm";
			this->Text = L"AdvancedSettingsForm";
			this->TopMost = true;
			this->Load += gcnew System::EventHandler(this, &AdvancedSettingsForm::AdvancedSettingsForm_Load);
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

private: System::Void cancel_btn_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		this->Close();
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
private: System::Void panel_Mouse_Down(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) 
	{
		if (e->Button == System::Windows::Forms::MouseButtons::Left) {
			if (e->Button == System::Windows::Forms::MouseButtons::Left) {
				ReleaseCapture();
				SendMessage(this->Handle, WM_NCLBUTTONDOWN, HTCAPTION, IntPtr(0));
			}
		}
	}
	   [DllImport("user32.dll")]
		   static bool ReleaseCapture();
	   [DllImport("user32.dll")]
		   static IntPtr SendMessage(IntPtr hWnd, int Msg, IntPtr wParam, IntPtr lParam);
	   const int WM_NCLBUTTONDOWN = 0xA1;
	   const IntPtr HTCAPTION = IntPtr(0x2);
};
}
