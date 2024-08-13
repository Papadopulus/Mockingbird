#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class ProgressForm : public System::Windows::Forms::Form
	{
	public:
		ProgressForm(void);

	protected:
		~ProgressForm();

	public:
		System::Windows::Forms::Label^ status_lbl;
		ReaLTaiizor::Controls::MetroProgressBar^ metroProgressBar1;

	private:
		System::ComponentModel::Container^ components;

		void InitializeComponent(void);

	};
}
