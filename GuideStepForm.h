#pragma once

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Resources;
	using namespace System::Collections::Generic;

	public ref class GuideStepForm : public System::Windows::Forms::Form
	{
	public:
		GuideStepForm(void);

	protected:
		~GuideStepForm();

	private:
		array<String^>^ slideTitles;
		array<String^>^ slideTexts;
		array<String^>^ slideResourceNames; // Video resource names
		int currentSlideIndex;
		// List of temporary files to be deleted when the application terminates
		List<String^>^ tempVideoPaths;

		System::Windows::Forms::Label^ txt_lbl;
		System::Windows::Forms::Button^ next_btn;
		System::Windows::Forms::Button^ prev_btn;
		AxWMPLib::AxWindowsMediaPlayer^ axWindowsMediaPlayer1;
		System::Windows::Forms::Label^ title_lbl;

		System::ComponentModel::Container^ components;

		void InitSlides();
		String^ GetTempVideoPath(String^ resourceName);
		void DisplaySlide(int slideIndex);
		void CleanUpTempVideos();
		void InitializeComponent(void);
		System::Void next_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void prev_btn_Click(System::Object^ sender, System::EventArgs^ e);
	};
}
