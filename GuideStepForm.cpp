#include "pch.h"
#include "GuideStepForm.h"

using namespace MockingApplication;

GuideStepForm::GuideStepForm(void)
{
	InitializeComponent();
	// Initialize slide resources and data
	InitSlides();
	// Display the first slide
	DisplaySlide(0);
}
GuideStepForm::~GuideStepForm()
{
	if (components)
	{
		delete components;
	}
	CleanUpTempVideos();
}
void GuideStepForm::InitSlides()
{
	// Sample slides (titles, texts and resource names)
	slideTitles = gcnew array<String^> {
		L"Home - Introduction Page",
			L"Mock Single File",
			L"Mock Files with Dependencies",
			L"Check/Uncheck All & Initial State",
			L"Advanced Options",
			L"Add Static To Files",
			L"Mocking Settings",
			L"Final Dependency Mocking",
			L"Duplicate Files in Lib Folder" };
	slideTexts = gcnew array<String^> {
		L"The Home page of the Mockingbird provides users with a brief introduction to the application's functionalities.\n"
			"On the left side of the screen, there's a navigation menu allowing users to access various features of the app.\n"
			"The center of the screen prominently displays a welcome message that emphasizes the application's name and its\n"
			"main goal – to automate the process of generating mock files, thereby simplifying unit testing.",

			L"The Mock Single File page allows users to easily generate a mock for a single file. The process is intuitive and\n"
			L"includes the following steps: Clicking the Mock button opens a file browser where the user can select the file\n"
			L"they want to mock. After selecting the file, a second file browser will open, prompting the user to choose a\n"
			L"location to save the created mock file. Once the location is selected, the mock file will be generated and ready\n"
			L"for use.This simple procedure streamlines working with individual files, allowing users to mock them quickly.",

			L"The user starts by clicking a button that opens a folder browser In the folder browser, the user selects the Mut\n"
			L"folder, typically containing a c. and a .h file for the test. After selecting the folder, a new form appears, \n"
			L"displaying a list of dependencies in checkboxes.These dependencies are pulled from the files in the selected Mut folder.",

			L"The Check All button allows the user to automatically select all dependencies. Once clicked, the Uncheck All button\n"
			L"appears, allowing the user to deselect all dependencies.There’s also an Initial State button that, when clicked,\n"
			L"restores the dependency selection to its initial state, where all non - mocked dependencies are selected by default.",

			L"Clicking the Advanced button opens a side form where the user can specify dependencies they do not want to appear in\n"
			L"the dependency checkbox list.To add a dependency to this exclusion list, the user enters the name of the dependency\n"
			L"in an input field and clicks Add.The user can also remove dependencies from this exclusion list by selecting them\n"
			L"and clicking Remove.This will make the dependency visible again in the main dependency list on the primary form.\n"
			L"After making changes, the user must click Apply to save any adjustments made in the advanced settings.",

			L"Clicking the Add Static To Files button adds the following code block to main.c and the .c file from the Mut:\n"
			L"#ifdef UNIT_TESTING                         In addition, static elements (functions, structures, prototypes, \n"
			L"#define STATIC                                  variables) in main.c will be added in the form of macros, such\n"
			L"#define STATIC_INLINE	  					                     as: extern void LibTuningMgr_ServiceFunc(void* pData);         \n"
			L"#else		 		 					                                              extern const S_LibTuningMgr_LoadClbk_t LoadClbkTable[];      \n"
			L"#define STATIC static                                                                                                                    \n"
			L"#define STATIC_INLINE static inline                                                                                                 \n"
			L"#endif                                                                                                                                         ",

			L"When the user selects a dependency from the checkbox list, the Mocking Settings button appears. Clicking it opens\n"
			L"a folder browser where the user selects the source folder of the application containing the selected dependencies.\n"
			L"After choosing the folder, a new form appears, displaying a checkbox list of all functions within the selected \n"
			L"dependency.By selecting a function, the function’s parameters are shown.For each parameter, the user can choose\n"
			L"one of three options : Ptr, Length, or None.To mock a function with enhanced capabilities, the user must select\n"
			L"Ptr for one parameter and Length for another, while leaving other parameters as None.After configuring the settings,\n"
			L" the user clicks Apply to save the changes for that function.",

			L"Once the user has selected all dependencies to mock and adjusted the necessary settings, they can click the Next button.\n"
			L"If there are files that have already been mocked and selected for re - mocking, a new form will appear displaying a\n"
			L"list of these files.The user can uncheck files they do not want to overwrite.By clicking Next, the user selects the\n"
			L"source folder of the application where these files are located, and the app will mock all functions of the selected\n"
			L"dependencies according to the user’s preferences.",

			L"If there are duplicated mock files in the lib folder, the duplicates will be shown in a new form.\n"
			L"The user can double - click on a file in the list, and a file explorer will open at the destination of the selected file,\n"
			L"allowing the user to delete or modify it.",
	};

	// Resource names for each video
	slideResourceNames = gcnew array<String^> {
		L"step1",
			L"step2",
			L"step3",
			L"step4",
			L"step5",
			L"step6",
			L"step7",
			L"step8",
			L"step9"
	};

	// Initialize the list for temporary paths
	tempVideoPaths = gcnew List<String^>();

	currentSlideIndex = 0;
}
String^ GuideStepForm::GetTempVideoPath(String^ resourceName)
{
	// Create a unique temp path for the video file
	String^ tempPath = Path::GetTempFileName() + ".mp4";
	// If the file already exists, don't create it again
	if (File::Exists(tempPath))
	{
		return tempPath;
	}
	System::Resources::ResourceManager^ resourceManager = (gcnew System::Resources::ResourceManager(L"MockingApplication.Resources", System::Reflection::Assembly::GetExecutingAssembly()));
	array<Byte>^ resourceData = (array<Byte>^)resourceManager->GetObject(resourceName);
	File::WriteAllBytes(tempPath, resourceData);

	// Add the temp file to the list of temporary files
	tempVideoPaths->Add(tempPath);

	return tempPath;
}
void GuideStepForm::DisplaySlide(int slideIndex)
{
	if (slideIndex < 0 || slideIndex >= slideTitles->Length) return;
	// Set the title and text for the current slide
	title_lbl->Text = slideTitles[slideIndex];
	txt_lbl->Text = slideTexts[slideIndex];

	// Load the video from the resource as a temporary file
	String^ tempVideoPath = GetTempVideoPath(slideResourceNames[slideIndex]);
	axWindowsMediaPlayer1->URL = tempVideoPath;
	axWindowsMediaPlayer1->settings->mute = true;
	axWindowsMediaPlayer1->Ctlcontrols->play();

	// Update the buttons
	prev_btn->Enabled = (slideIndex > 0);
	if (slideIndex == slideTitles->Length - 1)
	{
		next_btn->Text = L"Finish";
	}
	else
	{
		next_btn->Text = L"Next";
	}

}
void GuideStepForm::CleanUpTempVideos()
{
	// Delete all temporary video files
	for each (String ^ path in tempVideoPaths)
	{
		if (File::Exists(path))
		{
			try
			{
				File::Delete(path);
			}
			catch (Exception^)
			{
			}
		}
	}
}
void GuideStepForm::InitializeComponent(void)
{
	System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(GuideStepForm::typeid));
	this->txt_lbl = (gcnew System::Windows::Forms::Label());
	this->next_btn = (gcnew System::Windows::Forms::Button());
	this->prev_btn = (gcnew System::Windows::Forms::Button());
	this->axWindowsMediaPlayer1 = (gcnew AxWMPLib::AxWindowsMediaPlayer());
	this->title_lbl = (gcnew System::Windows::Forms::Label());
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axWindowsMediaPlayer1))->BeginInit();
	this->SuspendLayout();
	// 
	// txt_lbl
	// 
	this->txt_lbl->AutoSize = true;
	this->txt_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F));
	this->txt_lbl->Location = System::Drawing::Point(9, 37);
	this->txt_lbl->Name = L"txt_lbl";
	this->txt_lbl->Size = System::Drawing::Size(0, 17);
	this->txt_lbl->TabIndex = 1;
	this->txt_lbl->TextAlign = System::Drawing::ContentAlignment::TopCenter;
	// 
	// next_btn
	// 
	this->next_btn->Location = System::Drawing::Point(500, 400);
	this->next_btn->Name = L"next_btn";
	this->next_btn->Size = System::Drawing::Size(100, 30);
	this->next_btn->TabIndex = 3;
	this->next_btn->Text = L"Next";
	this->next_btn->Click += gcnew System::EventHandler(this, &GuideStepForm::next_btn_Click);
	// 
	// prev_btn
	// 
	this->prev_btn->Location = System::Drawing::Point(120, 400);
	this->prev_btn->Name = L"prev_btn";
	this->prev_btn->Size = System::Drawing::Size(100, 30);
	this->prev_btn->TabIndex = 4;
	this->prev_btn->Text = L"Previous";
	this->prev_btn->Click += gcnew System::EventHandler(this, &GuideStepForm::prev_btn_Click);
	// 
	// axWindowsMediaPlayer1
	// 
	this->axWindowsMediaPlayer1->Enabled = true;
	this->axWindowsMediaPlayer1->Location = System::Drawing::Point(120, 133);
	this->axWindowsMediaPlayer1->Name = L"axWindowsMediaPlayer1";
	this->axWindowsMediaPlayer1->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^>(resources->GetObject(L"axWindowsMediaPlayer1.OcxState")));
	this->axWindowsMediaPlayer1->Size = System::Drawing::Size(489, 261);
	this->axWindowsMediaPlayer1->TabIndex = 2;
	// 
	// title_lbl
	// 
	this->title_lbl->AutoSize = true;
	this->title_lbl->Font = (gcnew System::Drawing::Font(L"Stencil", 14.25F));
	this->title_lbl->Location = System::Drawing::Point(217, 8);
	this->title_lbl->Name = L"title_lbl";
	this->title_lbl->Size = System::Drawing::Size(0, 22);
	this->title_lbl->TabIndex = 0;
	this->title_lbl->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
	// 
	// GuideStepForm
	// 
	this->ClientSize = System::Drawing::Size(725, 470);
	this->Controls->Add(this->title_lbl);
	this->Controls->Add(this->txt_lbl);
	this->Controls->Add(this->axWindowsMediaPlayer1);
	this->Controls->Add(this->next_btn);
	this->Controls->Add(this->prev_btn);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Name = L"GuideStepForm";
	(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->axWindowsMediaPlayer1))->EndInit();
	this->ResumeLayout(false);
	this->PerformLayout();

}
System::Void GuideStepForm::next_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (currentSlideIndex + 1 == slideTitles->Length)
	{
		this->Close();
		return;
	}
	// Display the next slide
	DisplaySlide(++currentSlideIndex);
}
System::Void GuideStepForm::prev_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Show the previous slide
	DisplaySlide(--currentSlideIndex);
}