#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class OverwriteForm : public System::Windows::Forms::Form
	{
	public:
		OverwriteForm(void);

	protected:
		~OverwriteForm();
	public: 
		System::Windows::Forms::CheckedListBox^ cfgCheckedListBox;
		System::Windows::Forms::Button^ overwrite_next_btn;
		System::Windows::Forms::CheckedListBox^ libCheckedListBox;
		System::Windows::Forms::CheckedListBox^ otherCheckedListBox;

	private: 
		System::Windows::Forms::Label^ label1;
		System::Windows::Forms::Label^ cfg_lbl;
		System::Windows::Forms::Label^ lib_lbl;
		System::Windows::Forms::Label^ other_lbl;
		System::Windows::Forms::Button^ back_btn;

	private:
		System::ComponentModel::Container ^components;

		void InitializeComponent(void);

	private: System::Void back_btn_Click(System::Object^ sender, System::EventArgs^ e);
};
}
