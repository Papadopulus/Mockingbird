#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "OverlayCheckBoxForm.h"
#include "CommonFunctions.h"
#include "MockingFiles.h"
#include "AdvancedSettingsForm.h"
#include <set>

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace WK::Libraries::BetterFolderBrowserNS;
	using namespace System::Collections::Generic;

	public ref class FilesForm : public System::Windows::Forms::Form
	{
	public:
		FilesForm(void)
		{
			InitializeComponent();
			
			this->slideTimer = gcnew Timer();
			this->slideTimerClose = gcnew Timer();
			this->slideTimer->Interval = 10; // Set interval to 10 ms
			this->slideTimerClose->Interval = 10; // Set interval to 10 ms
			this->slideTimer->Tick += gcnew EventHandler(this, &FilesForm::slideTimer_Tick);
			this->slideTimerClose->Tick += gcnew EventHandler(this, &FilesForm::picture_btn_Click);
		}

	protected:
		~FilesForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: List<System::String^>^ includes;
	private: String^ lastUsedDirectory;;
	private: System::Windows::Forms::Button^ mutFolder_btn;
	private: System::Windows::Forms::Label^ dsc_lbl;
	private: System::Windows::Forms::Label^ labelTitle;
	private: Timer^ slideTimer;
	private: Timer^ slideTimerClose;
	// Adding declaration for application root path variable
	private : String^ applicationRootPath;
	private: int targetX; // Target X position for sliding form
	protected:
		OverlayCheckBoxForm^ overlay;
		AdvancedSettingsForm^ advancedForm;
	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->mutFolder_btn = (gcnew System::Windows::Forms::Button());
			this->dsc_lbl = (gcnew System::Windows::Forms::Label());
			this->labelTitle = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// mutFolder_btn
			// 
			this->mutFolder_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->mutFolder_btn->Location = System::Drawing::Point(328, 352);
			this->mutFolder_btn->Name = L"mutFolder_btn";
			this->mutFolder_btn->Size = System::Drawing::Size(256, 54);
			this->mutFolder_btn->TabIndex = 0;
			this->mutFolder_btn->Text = L"Select Mut Folder";
			this->mutFolder_btn->UseVisualStyleBackColor = true;
			this->mutFolder_btn->Click += gcnew System::EventHandler(this, &FilesForm::mutFolder_btn_Click);
			// 
			// dsc_lbl
			// 
			this->dsc_lbl->AutoSize = true;
			this->dsc_lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->dsc_lbl->Location = System::Drawing::Point(202, 199);
			this->dsc_lbl->Name = L"dsc_lbl";
			this->dsc_lbl->Size = System::Drawing::Size(525, 56);
			this->dsc_lbl->TabIndex = 1;
			this->dsc_lbl->Text = L"Click the button \"Select Mut Folder\" to select a mut folder. \r\nIn this folder, yo"
				L"u should find one .c file and one .h file.";
			// 
			// labelTitle
			// 
			this->labelTitle->AutoSize = true;
			this->labelTitle->Font = (gcnew System::Drawing::Font(L"Stencil", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->labelTitle->Location = System::Drawing::Point(218, 61);
			this->labelTitle->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->labelTitle->Name = L"labelTitle";
			this->labelTitle->Size = System::Drawing::Size(487, 40);
			this->labelTitle->TabIndex = 2;
			this->labelTitle->Text = L"Mocking All Dependencies";
			// 
			// FilesForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(967, 578);
			this->Controls->Add(this->labelTitle);
			this->Controls->Add(this->dsc_lbl);
			this->Controls->Add(this->mutFolder_btn);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			this->Margin = System::Windows::Forms::Padding(4);
			this->Name = L"FilesForm";
			this->Text = L"FilesForm";
			this->Load += gcnew System::EventHandler(this, &FilesForm::filesForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void filesForm_Load(System::Object^ sender, System::EventArgs^ e)
	{
		// Disables the ControlBox (minimize, maximize, close) for this form
		this->ControlBox = false;
	}
	private:
		System::Void mutFolder_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// Create and open a folder browser dialog
			BetterFolderBrowser^ folderDialog = gcnew BetterFolderBrowser();
			if (applicationRootPath != nullptr)
			{
				folderDialog->RootFolder = applicationRootPath;
			}
			else
			{
				folderDialog->RootFolder = Directory::GetCurrentDirectory();
			}
			if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK /*CommonFileDialogResult::Ok*/) {
				

				// Get the selected folder path
				String^ selectedPath = folderDialog->SelectedFolder;

				// Save parent path for checking /mocks folder
				applicationRootPath = selectedPath;
				// Call the function to process files in the selected folder
				ProcessFilesInFolder(selectedPath);
			}
		}

		void ProcessFilesInFolder(String^ folderPath)
		{
			// Create a vector to store all file paths
			std::vector<std::string> allFiles;
			// Convert managed string to standard string
			std::string directory = std::filesystem::path(CommonFunctions::toStandardString(folderPath)).string();

			// Iterate through the directory to find .c and .h files
			for (const auto& entry : std::filesystem::directory_iterator(directory)) {
				if (entry.path().extension() == ".c" || entry.path().extension() == ".h") {
					allFiles.push_back(entry.path().string());
				}
			}
			// Create a set to store all #include directives
			std::set<std::string> uniqueIncludes;
			// Get #include directives from each file and add them to the includes vector
			for (const auto& file : allFiles) {
				auto fileIncludes = GetIncludesFromFile(file);
				uniqueIncludes.insert(fileIncludes.begin(), fileIncludes.end());
			}
			// Inicijalizacija managed liste
			this->includes = gcnew System::Collections::Generic::List<System::String^>();

			// Iteriranje kroz std::vector i dodavanje elemenata u managed listu
			for (const auto& include : uniqueIncludes)
			{
				this->includes->Add(gcnew System::String(include.c_str()));
			}
			// Display #include directives to the user (for now just print to console)
			DisplayIncludesToUser();
		}
		std::vector<std::string> GetIncludesFromFile(const std::string& filePath) 
		{
			// Create a vector to store #include directives
			std::vector<std::string> includes;
			// Open the file for reading
			std::ifstream file(filePath);
			std::string line;
			// Regular expression to match #include directives
			std::regex includeRegex(R"(^\s*#include\s*["<](.*?)[">])");

			// Read the file line by line
			while (std::getline(file, line)) {
				std::smatch match;
				// If the line matches the #include regex, add it to the includes vector
				if (std::regex_search(line, match, includeRegex)) {
					includes.push_back(match[1].str());
				}
			}
			return includes;
		}
		void DisplayIncludesToUser() 
		{
			if (overlay == nullptr)
			{
				overlay = gcnew OverlayCheckBoxForm();
				// Set the start position and parent for the overlay form
				overlay->StartPosition = FormStartPosition::Manual;
				overlay->MdiParent = this->MdiParent;
				overlay->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FilesForm::OverlayForm_FormClosed);
				// Add an event handler for the buttons click event
				overlay->next_btn->Click += gcnew System::EventHandler(this, &FilesForm::next_btn_Click);
				overlay->back_btn->Click += gcnew System::EventHandler(this, &FilesForm::back_btn_Click);
				overlay->initial_btn->Click += gcnew System::EventHandler(this, &FilesForm::initial_btn_Click);
				overlay->checkAllUncheckAll_btn->Click += gcnew System::EventHandler(this, &FilesForm::checkAllUncheckAll_btn_Click);
				overlay->advanced_btn->Click += gcnew System::EventHandler(this, &FilesForm::advanced_btn_Click);
				overlay->Dock = System::Windows::Forms::DockStyle::Fill;
				overlay->Show();
			}
			else 
			{
				overlay->Activate();
			}

			// Read excluded includes from file
			std::set<std::string> excludedIncludes;
			std::ifstream file("excluded_includes.txt");
			std::string line;
			while (std::getline(file, line)) {
				excludedIncludes.insert(line);
			}
			file.close();

			overlay->checkedListBox->Items->Clear();
			// Add each #include directive to the checked list box in the overlay form
			for each(String^ include in this->includes) {
				if (excludedIncludes.find(CommonFunctions::toStandardString(include)) == excludedIncludes.end()) {
					
					overlay->checkedListBox->Items->Add(include);
				}
			}
			uncheckFiles();
		}
		System::Void advanced_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (advancedForm == nullptr) {
				advancedForm = gcnew AdvancedSettingsForm();
				advancedForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
				advancedForm->StartPosition = FormStartPosition::Manual;
				advancedForm->Location = Point(this->MdiParent->Right- advancedForm->Width, this->MdiParent->Top);
				targetX = this->MdiParent->Right - 2;
				advancedForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FilesForm::AdvancedSettingsForm_FormClosed);
				advancedForm->apply_btn->Click += gcnew System::EventHandler(this, &FilesForm::apply_btn_Click);
				advancedForm->pictureBox1->Click += gcnew System::EventHandler(this, &FilesForm::picture_btn_Click);
				this->MdiParent->TopMost=true;
				this->MdiParent->LocationChanged += gcnew System::EventHandler(this, &FilesForm::MainForm_LocationChanged);
				advancedForm->Show();
				slideTimer->Start();
			}
			else {
				advancedForm->Activate();
			}
			this->MdiParent->TopMost = false;
		}
		System::Void picture_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			slideTimerClose->Start();
			this->MdiParent->TopMost = true;
			if (advancedForm->Location.X > (targetX-advancedForm->Width))
			{
				advancedForm->Location = Point(advancedForm->Location.X - 20, advancedForm->Location.Y);
			}
			else
			{
				advancedForm->Close();
				this->MdiParent->TopMost = false;
				slideTimerClose->Stop();
			}
			

		}
		System::Void AdvancedSettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
		{
			// Set the overlay form to null when it is closed
			advancedForm = nullptr;
		}
		System::Void apply_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// Save items to a file or settings
			// Here we can save to a file or to a Settings variable
			std::ofstream file("excluded_includes.txt");
			for (int i = 0; i < advancedForm->listBox->Items->Count; i++) {
				file << CommonFunctions::toStandardString(advancedForm->listBox->Items[i]->ToString()) << std::endl;
			}
			file.close();
			DisplayIncludesToUser();
		}

		System::Void next_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// Create a vector to store the selected files
			std::vector<std::string> checkedFiles;
			// Add each checked item from the checked list box to the vector
			for (int i = 0; i < overlay->checkedListBox->CheckedItems->Count; i++)
			{
				std::string item = CommonFunctions::toStandardString(overlay->checkedListBox->CheckedItems[i]->ToString());
				checkedFiles.push_back(item);
			}
			MessageBox::Show("Now select the src folder that contains the checked files.");
			// Call the function to select the source folder
			if (advancedForm != nullptr)
			{
				advancedForm->Close();
			}
			SelectSrcFolder(checkedFiles);
		}
		System::Void back_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (advancedForm != nullptr)
			{
				advancedForm->Close();
			}
			overlay->Close();
		}
		System::Void initial_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			uncheckFiles();
		}
		System::Void checkAllUncheckAll_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			if (overlay->checkAllUncheckAll_btn->Text == "Check All")
			{
				// Check all items
				for (int i = 0; i < overlay->checkedListBox->Items->Count; i++)
				{
					overlay->checkedListBox->SetItemChecked(i, true);
				}
				overlay->checkAllUncheckAll_btn->Text = "Uncheck All";
			}
			else
			{
				// Uncheck all items
				for (int i = 0; i < overlay->checkedListBox->Items->Count; i++)
				{
					overlay->checkedListBox->SetItemChecked(i, false);
				}
				overlay->checkAllUncheckAll_btn->Text = "Check All";
			};
		}
		void SelectSrcFolder(std::vector<std::string> selectedFiles)
		{
			// Create and open a folder browser dialog
			BetterFolderBrowser^ folderDialog = gcnew BetterFolderBrowser();
			if (lastUsedDirectory != nullptr)
			{
				folderDialog->RootFolder = lastUsedDirectory;
			}
			else
			{
				folderDialog->RootFolder = Directory::GetCurrentDirectory();
			}		
			if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				// Convert managed string to standard string
				std::string selectedPath = CommonFunctions::toStandardString(folderDialog->SelectedPath);
				lastUsedDirectory = folderDialog->SelectedPath;
				// Recursively search folder and process files
				for (const auto& file : selectedFiles)
				{
					ProcessSelectedFileInFolder(file, selectedPath);
				}
				if (overlay != nullptr)
				{
					overlay->Close();

				}
				MessageBox::Show("All files are successfully saved. You are now ready to use mocked files.");
			}
		}
		void ProcessSelectedFileInFolder(const std::string& fileName, const std::string& folderPath)
		{
			try {
				// Get the parent path of the application root path
				std::string applicationRootPathStd = std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string();
				MockingFiles mockingFiles;
				// Recursively iterate through the directory to find the selected file
				for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
				{
					// If the file name matches, process it
					if (entry.path().filename() == fileName)
					{
						// Get the original folder and create a new folder path
						std::string originalFolder = entry.path().parent_path().string();
						std::string newFolder = applicationRootPathStd + "\\mocks/" + entry.path().parent_path().filename().string();

						// Create new folder if it doesn't exist
						std::filesystem::create_directories(newFolder);

						// Copy all .h files and mock .c files
						for (const auto& file : std::filesystem::directory_iterator(originalFolder))
						{
							// Copy .h file
							if (file.path().extension() == ".h")
							{
								std::filesystem::copy(file.path(), newFolder + "/" + file.path().filename().string(), std::filesystem::copy_options::overwrite_existing);

							}
							// Mock .c file
							if (file.path().extension() == ".c")
							{
								std::string filePathString = file.path().string();
								std::vector<std::string> mockedFunctions = mockingFiles.ProcessFile(filePathString);
								CommonFunctions::SaveMockedFile(newFolder + "/" + file.path().filename().string(), filePathString, mockedFunctions);

							}
						}

						
					}
				}
			}
			catch(const std::exception & e)
			{
				// Show an error message if an exception occurs
				String^ errorMessage = gcnew String(e.what());
				MessageBox::Show("Error: " + errorMessage);
			}
		}
		System::Void OverlayForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
		{
			// Set the overlay form to null when it is closed
			overlay = nullptr;
		}
		void slideTimer_Tick(System::Object^ sender, System::EventArgs^ e)
		{
			if (advancedForm->Location.X < targetX)
			{
				advancedForm->Location = Point(advancedForm->Location.X + 20, advancedForm->Location.Y);
			}
			else
			{
				advancedForm->Location = Point(this->MdiParent->Location.X + this->MdiParent->Width, this->MdiParent->Location.Y);
				slideTimer->Stop();
			}
		}
		System::Void MainForm_LocationChanged(System::Object ^ sender, System::EventArgs ^ e)
		{
			if (advancedForm != nullptr)
			{
				// Pomerite AdvancedForm u odnosu na novu lokaciju MainForm
				advancedForm->Location = System::Drawing::Point(this->MdiParent->Location.X + this->MdiParent->Width, this->MdiParent->Location.Y);

			}
		}
		void uncheckFiles()
		{
			try {
				// Get the parent path of the application root path
				std::string parentPath = std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string();

				// Recursively find all files in the parent folder
				std::set<std::string> parentFolderFiles;
				for (const auto& entry : std::filesystem::recursive_directory_iterator(parentPath))
				{
					// Skip .svn directories
					if (entry.is_directory() && entry.path().filename() == ".svn")
					{
						continue;
					}
					if (entry.is_regular_file()) // Ensure it's a file and not a directory
					{
						parentFolderFiles.insert(entry.path().filename().string());
					}
				}

				// Uncheck the files in the checkedListBox
				for (int i = 0; i < overlay->checkedListBox->Items->Count; i++)
				{
					String^ item = overlay->checkedListBox->Items[i]->ToString();
					std::string itemStd = CommonFunctions::toStandardString(item);

					if (parentFolderFiles.find(itemStd) != parentFolderFiles.end())
					{
						overlay->checkedListBox->SetItemChecked(i, false); // Uncheck the item
					}
					else
					{
						overlay->checkedListBox->SetItemChecked(i, true); // Check the item
					}
				}
			}
			catch (const std::exception& e)
			{
				// Show an error message if an exception occurs
				String^ errorMessage = gcnew String(e.what());
				MessageBox::Show("Error: " + errorMessage);
			}
		}
	};
}
