#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	public ref class DuplicateFilesForm : public System::Windows::Forms::Form
	{
	public:
		DuplicateFilesForm(void);

	protected:
		~DuplicateFilesForm();
	private:
		System::Windows::Forms::Button^ ok_btn;
		System::Windows::Forms::Label^ label1;
	public: System::Windows::Forms::ListBox^ listBox1;


	private:
		System::ComponentModel::Container^ components;

		void InitializeComponent(void);

	private: System::Void ok_btn_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
