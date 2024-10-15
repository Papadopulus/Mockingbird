#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Home : public System::Windows::Forms::Form
	{
	public:
		Home(void);

	protected:
		~Home();

	private: 
		System::Windows::Forms::Label^ labelTitle;
		System::Windows::Forms::Label^ labelDescription;
		System::Windows::Forms::PictureBox^ pictureBox1;
	private:
		System::ComponentModel::Container ^components;

		void InitializeComponent(void);

	private: System::Void homeForm_Load(System::Object^ sender, System::EventArgs^ e);

	};
}
