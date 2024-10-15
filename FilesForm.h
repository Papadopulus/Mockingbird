#pragma once
#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include <set>
#include <unordered_set>
#include "OverlayCheckBoxForm.h"
#include "CommonFunctions.h"
#include "MockingFiles.h"
#include "AdvancedSettingsForm.h"
#include "ProgressForm.h"
#include "MockingSettings.h"
#include "IncludeModuleSettings.h"
#include "OverwriteForm.h"
#include "DuplicateFilesForm.h"


namespace MockingApplication {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Collections::Generic;
	using namespace WK::Libraries::BetterFolderBrowserNS;

	public ref struct MockedFileInfo
	{
		String^ fileName;      // Name of the file
		bool isDuplicate;      // Is the file a duplicate
		String^ originalPath;  // Path to the original file (if any)
		String^ parentFolder; // Folder from where the file originates
	};
	public ref struct FileEntry
	{
		String^ relativePath;  // The path we display in the ListBox
		String^ fullPath;      // Full file path

		FileEntry(String^ relative, String^ full)
		{
			relativePath = relative;
			fullPath = full;
		}

		// Override ToString() to display only the relative path in the ListBox
		virtual String^ ToString() override
		{
			return relativePath;
		}
	};
	public ref class FilesForm : public System::Windows::Forms::Form
	{
	public:
		FilesForm(void);

	protected:
		~FilesForm();

	private: 
		List<System::String^>^ includes;
		List<MockedFileInfo^>^ mockedFilesInfo = gcnew List<MockedFileInfo^>();
		bool existingMockedFiles = false;
		System::Windows::Forms::Button^ mutFolder_btn;
		System::Windows::Forms::Label^ dsc_lbl;
		System::Windows::Forms::Label^ labelTitle;
		Timer^ slideTimer;
		bool isOpenTimer;
		MockingFiles mockingFiles;
		// Adding declaration for application root path variable
		String^ applicationRootPath;
		String^ srcPath;
		int targetX; // Target X position for sliding form
		System::Collections::Generic::List<System::String^>^ functionsForMockingSettings;
		System::Collections::Generic::List< IncludeModuleSettings^>^ includeSettings = gcnew System::Collections::Generic::List< IncludeModuleSettings^>();
	protected:
		OverlayCheckBoxForm^ overlay;
		AdvancedSettingsForm^ advancedForm;
		ProgressForm^ progressForm;
		MockingSettings^ mockingSettingsForm;
		OverwriteForm^ overwriteForm;
		DuplicateFilesForm^ duplicateForm;
	public:
		BackgroundWorker^ bgWorker;

	private:
		System::ComponentModel::Container^ components;

		void InitializeComponent(void);

	private: 
		System::Void filesForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void add_static_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void InsertStaticMacroInFiles(String^ folderPath);
		void ReplaceStaticInFiles(String^ folderPath);
		System::Void mutFolder_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void ProcessFilesInFolder(String^ folderPath);
		void ProcessFilesInFolderForStaticElements(String^ folderPath);
		std::vector<std::string> GetIncludesFromFile(const std::string& filePath);
		void DisplayIncludesToUser();
		void checkedListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e);
		System::Void advanced_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void picture_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void AdvancedSettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void apply_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void next_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void back_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void save_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void apply_Mocking_Settings_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void initial_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void checkAllUncheckAll_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void SelectSrcFolder();
		void ProcessSelectedFileInFolder(MockedFileInfo^ fileInfo, const std::string& folderPath,bool isDuplicate);
		System::Void OverlayForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		void slideTimer_Tick(System::Object^ sender, System::EventArgs^ e);
		System::Void MainForm_LocationChanged(System::Object^ sender, System::EventArgs^ e);
		void uncheckFiles();
		String^ LoadLastUsedPath(String^ key);
		void SaveLastUsedPath(String^ key, String^ path);
		void FilesForm::bgWorker_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e);
		void FilesForm::bgWorker_ProgressChanged(Object^ sender, ProgressChangedEventArgs^ e);
		void FilesForm::bgWorker_DoWork(Object^ sender, DoWorkEventArgs^ e);
		System::Void checkedListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		std::string removeExtension(String^ fileNameWithExtension);
		void InsertMacrosIntoHeader(const std::string& headerFilePath, const std::vector<std::string>& macroDefinitions);
		System::Void FilesForm::mockingSettings_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void FilesForm::MockingSettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void CheckListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		void ProcessFileForMockingSettings();
		void DisplayFunctionsForMockingSettings(std::vector<std::string> functions);
		std::string ParseFunctionName(const std::string& functionSignature);
		void ShowOverwriteForm();
		void CheckExistingMockedFiles(List<MockedFileInfo^>^ selectedFiles);
		System::Void FilesForm::overwrite_next_btn(System::Object^ sender, System::EventArgs^ e);
		void FilesForm::OnFileDoubleClick(Object^ sender, MouseEventArgs^ e);
	};
}
