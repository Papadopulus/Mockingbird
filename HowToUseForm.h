#pragma once
#include "GuideStepForm.h"

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Resources;
	using namespace System::IO;

	public ref class HowToUseForm : public System::Windows::Forms::Form
	{
	public:
		HowToUseForm(void);

	protected:
		~HowToUseForm();
	private: 
		System::Windows::Forms::Label^ labelTitle;
		AxWMPLib::AxWindowsMediaPlayer^ axWindowsMediaPlayer1;
		System::Windows::Forms::Label^ txt_lbl;
		GuideStepForm^ guideStepForm;
	public: 
		System::Windows::Forms::Button^ start_guide_btn;
		static String^ tempVideoPath; // Stores the path to the temporary video file

	private:
		System::ComponentModel::Container^ components;
		void InitializeComponent(void);

	private:
		System::Void start_guide_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void GuideStepForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void HowToUseForm_Load(System::Object^ sender, System::EventArgs^ e);

	};
}
