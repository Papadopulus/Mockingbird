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

	public ref class AdvancedSettingsForm : public System::Windows::Forms::Form
	{
	public:
		AdvancedSettingsForm(void);

	protected:
		~AdvancedSettingsForm();

	public: 
		System::Windows::Forms::ListBox^ listBox;
		System::Windows::Forms::PictureBox^ pictureBox1;
		System::Windows::Forms::Button^ apply_btn;
	private: 
		System::Windows::Forms::Button^ add_btn;
		System::Windows::Forms::Button^ remove_btn;
		System::Windows::Forms::Label^ txt_lbl;
		System::Windows::Forms::TextBox^ textBox;
		System::Windows::Forms::Panel^ panel;
		System::Windows::Forms::Label^ title_lbl;

	private:
		System::ComponentModel::Container ^components;

		void InitializeComponent(void);

	private: 
		System::Void add_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void remove_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void AdvancedSettingsForm_Load(System::Object^ sender, System::EventArgs^ e);
};
}
