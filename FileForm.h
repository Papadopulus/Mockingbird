#pragma once
#include "MockingFiles.h"
#include <filesystem>
#include <fstream>
namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class FileForm : public System::Windows::Forms::Form
	{
	public:
		FileForm(void);
	protected:
		~FileForm();

	private: 
		System::Windows::Forms::Label^ labelTitle;
		System::Windows::Forms::Label^ labelDescription;
		System::Windows::Forms::Button^ mockSingleFile_btn;

	private:
		System::ComponentModel::Container ^components;
		
		void InitializeComponent(void);

	private: 
		System::Void fileForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void mockSingleFile_btn_Click(System::Object^ sender, System::EventArgs^ e);
};
}
