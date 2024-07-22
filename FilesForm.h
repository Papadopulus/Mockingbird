#pragma once
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include "OverlayCheckBoxForm.h"
#include "CommonFunctions.h"
#include "MockingFiles.h"

namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for FilesForm
	/// </summary>
	public ref class FilesForm : public System::Windows::Forms::Form
	{
	public:
		FilesForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FilesForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ mutFolder_btn;
	protected:
	private: System::Windows::Forms::Label^ dsc_lbl;
	private: System::Windows::Forms::Label^ labelTitle;
	private: 
	// Dodavanje deklaracije promenljive za application root path
	private : String^ applicationRootPath;
	protected:
		OverlayCheckBoxForm^ overlay = gcnew OverlayCheckBoxForm();

	protected:




	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
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
		this->ControlBox = false;
	}
	private:
		System::Void mutFolder_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			FolderBrowserDialog^ folderDialog = gcnew FolderBrowserDialog();
			if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				String^ selectedPath = folderDialog->SelectedPath;

				//Sacuvaj parent path za proveru /test/mocks foldera
				applicationRootPath = selectedPath;
				
				// Pozovi funkciju za procesiranje fajlova u izabranom folderu
				ProcessFilesInFolder(selectedPath);
			}
		}

		void ProcessFilesInFolder(String^ folderPath)
		{
			std::vector<std::string> allFiles;
			std::string directory = std::filesystem::path(CommonFunctions::toStandardString(folderPath)).string();

			for (const auto& entry : std::filesystem::directory_iterator(directory)) {
				if (entry.path().extension() == ".c" || entry.path().extension() == ".h") {
					allFiles.push_back(entry.path().string());
				}
			}

			std::vector<std::string> includes;
			for (const auto& file : allFiles) {
				auto fileIncludes = GetIncludesFromFile(file);
				includes.insert(includes.end(), fileIncludes.begin(), fileIncludes.end());
			}

			// Prikaži #include direktive korisniku (za sada samo ispisujemo u konzoli)
			DisplayIncludesToUser(includes);
		}
		std::vector<std::string> GetIncludesFromFile(const std::string& filePath) {
			std::vector<std::string> includes;
			std::ifstream file(filePath);
			std::string line;
			std::regex includeRegex(R"(^\s*#include\s*["<](.*?)[">])");

			while (std::getline(file, line)) {
				std::smatch match;
				if (std::regex_search(line, match, includeRegex)) {
					includes.push_back(match[1].str());
				}
			}
			return includes;
		}
		void DisplayIncludesToUser(std::vector<std::string> includes) {


			overlay->StartPosition = FormStartPosition::Manual;
			overlay->MdiParent = this->MdiParent;
			overlay->Dock = System::Windows::Forms::DockStyle::Fill;
			overlay->Show();
			for (std::string include : includes) {
				//System::String^ includeManaged = msclr::interop::marshal_as<System::String^>(include);
				System::String^ includeManaged = gcnew System::String(include.c_str());
				overlay->checkedListBox->Items->Add(includeManaged);
			}
			overlay->next_btn->Click += gcnew System::EventHandler(this, &FilesForm::next_btn_Click);
		}
		System::Void next_btn_Click(System::Object^ sender, System::EventArgs^ e)
		{
			std::vector<std::string> checkedFiles;
			for (int i = 0; i < overlay->checkedListBox->CheckedItems->Count; i++)
			{
				std::string item = CommonFunctions::toStandardString(overlay->checkedListBox->CheckedItems[i]->ToString());
				checkedFiles.push_back(item);
			}
			SelectSrcFolder(checkedFiles);
		}
		void SelectSrcFolder(std::vector<std::string> selectedFiles)
		{
			FolderBrowserDialog^ folderDialog = gcnew FolderBrowserDialog();
			if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				std::string selectedPath = CommonFunctions::toStandardString(folderDialog->SelectedPath);

				// Rekurzivno pretraži folder i obradi fajlove
				for (const auto& file : selectedFiles)
				{
					ProcessSelectedFileInFolder(file, selectedPath);
				}
			}
		}
		void ProcessSelectedFileInFolder(const std::string& fileName, const std::string& folderPath)
		{
			try {

				std::string applicationRootPathStd = std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string();
				MockingFiles mockingFiles;
				for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
				{
					if (entry.path().filename() == fileName)
					{
						std::string originalFolder = entry.path().parent_path().string();
						std::string newFolder = applicationRootPathStd + "\\mocks/" + entry.path().parent_path().filename().string();

						// Kreiraj novi folder ako ne postoji
						std::filesystem::create_directories(newFolder);

						// Kopiraj sve .h fajlove
						for (const auto& hFile : std::filesystem::directory_iterator(originalFolder))
						{
							if (hFile.path().extension() == ".h")
							{
								std::filesystem::copy(hFile.path(), newFolder + "/" + hFile.path().filename().string());
							}
						}

						// Mokuj .c fajl
						if (entry.path().extension() == ".c")
						{
							std::string filePathString = entry.path().string();
							std::vector<std::string> mockedFunctions = mockingFiles.ProcessFile(filePathString);
							CommonFunctions::SaveMockedFile(newFolder + "/" + entry.path().filename().string(), filePathString, mockedFunctions);
							MessageBox::Show("File successfully saved. You are now ready to use mocked file.");
						}
					}
				}
			}
			catch(const std::exception & e)
			{
				String^ errorMessage = gcnew String(e.what());
				MessageBox::Show("Error: " + errorMessage);
			}
		}
		System::Void OverlayForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
		{
			overlay = nullptr;
		}
	};
}
