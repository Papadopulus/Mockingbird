#pragma once
#include <vector>
#include <string>
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
		OverlayCheckBoxForm(void);

	protected:
		~OverlayCheckBoxForm();

	private: System::Windows::Forms::Label^ txt_lbl;
	public:
		System::Windows::Forms::CheckedListBox^ checkedListBox;
		System::Windows::Forms::Button^ next_btn;
		System::Windows::Forms::Button^ back_btn;
		System::Windows::Forms::Button^ advanced_btn;
		System::Windows::Forms::Button^ initial_btn;
		System::Windows::Forms::Button^ checkAllUncheckAll_btn;

	private:
		System::ComponentModel::Container^ components;

		void InitializeComponent(void);
	};
}
