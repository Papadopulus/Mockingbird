#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "OverlayCheckBoxForm.h"
#include "CommonFunctions.h"
#include "MockingFiles.h"
#include <set>

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	public ref class FilesForm : public System::Windows::Forms::Form
	{
	public:
		FilesForm(void)
		{
			InitializeComponent();
		}

	protected:
		~FilesForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ mutFolder_btn;
	private: System::Windows::Forms::Label^ dsc_lbl;
	private: System::Windows::Forms::Label^ labelTitle;
	// Adding declaration for application root path variable
	private : String^ applicationRootPath;
	protected:
		OverlayCheckBoxForm^ overlay = gcnew OverlayCheckBoxForm();

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
			FolderBrowserDialog^ folderDialog = gcnew FolderBrowserDialog();
			if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				// Get the selected folder path
				String^ selectedPath = folderDialog->SelectedPath;

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
			std::vector<std::string> includes(uniqueIncludes.begin(), uniqueIncludes.end());
			// Display #include directives to the user (for now just print to console)
			DisplayIncludesToUser(includes);
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
		void DisplayIncludesToUser(std::vector<std::string> includes) 
		{
			if (overlay == nullptr)
			{
				overlay = gcnew OverlayCheckBoxForm();
			}
			// Set the start position and parent for the overlay form
			overlay->StartPosition = FormStartPosition::Manual;
			overlay->MdiParent = this->MdiParent;
			overlay->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FilesForm::OverlayForm_FormClosed);
			overlay->Dock = System::Windows::Forms::DockStyle::Fill;
			overlay->Show();
			// Add each #include directive to the checked list box in the overlay form
			for (std::string include : includes) {
				System::String^ includeManaged = gcnew System::String(include.c_str());
				overlay->checkedListBox->Items->Add(includeManaged);
			}
			// Add an event handler for the next button click event
			overlay->next_btn->Click += gcnew System::EventHandler(this, &FilesForm::next_btn_Click);
			overlay->back_btn->Click += gcnew System::EventHandler(this, &FilesForm::back_btn_Click);
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
			SelectSrcFolder(checkedFiles);
		}
		System::Void back_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			overlay->Close();
		}
		void SelectSrcFolder(std::vector<std::string> selectedFiles)
		{
			// Create and open a folder browser dialog
			FolderBrowserDialog^ folderDialog = gcnew FolderBrowserDialog();
			if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				// Convert managed string to standard string
				std::string selectedPath = CommonFunctions::toStandardString(folderDialog->SelectedPath);

				// Recursively search folder and process files
				for (const auto& file : selectedFiles)
				{
					ProcessSelectedFileInFolder(file, selectedPath);
				}
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
								std::filesystem::copy(file.path(), newFolder + "/" + file.path().filename().string());
							}
							// Mock .c file
							if (file.path().extension() == ".c")
							{
								std::string filePathString = file.path().string();
								std::vector<std::string> mockedFunctions = mockingFiles.ProcessFile(filePathString);
								CommonFunctions::SaveMockedFile(newFolder + "/" + file.path().filename().string(), filePathString, mockedFunctions);
								MessageBox::Show("File successfully saved. You are now ready to use mocked file.");
								overlay->Close();
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
	};
}
