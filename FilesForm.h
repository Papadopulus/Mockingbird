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
#include "ProgressForm.h"

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
		FilesForm(void);

	protected:
		~FilesForm();

	private: List<System::String^>^ includes;
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
		ProgressForm^ progressForm;
	private:
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code

		void InitializeComponent(void);

#pragma endregion
	private: 
		System::Void filesForm_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void mutFolder_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void ProcessFilesInFolder(String^ folderPath);
		std::vector<std::string> GetIncludesFromFile(const std::string& filePath);
		void DisplayIncludesToUser();
		void checkedListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e);
		System::Void advanced_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void picture_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void AdvancedSettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		System::Void apply_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void next_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void back_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void initial_btn_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void checkAllUncheckAll_btn_Click(System::Object^ sender, System::EventArgs^ e);
		void SelectSrcFolder(std::vector<std::string> selectedFiles);
		void ProcessSelectedFileInFolder(const std::string& fileName, const std::string& folderPath);
		System::Void OverlayForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e);
		void slideTimer_Tick(System::Object^ sender, System::EventArgs^ e);
		System::Void MainForm_LocationChanged(System::Object^ sender, System::EventArgs^ e);
		void uncheckFiles();
		String^ LoadLastUsedPath(String^ key);
		void SaveLastUsedPath(String^ key, String^ path);
	};
}
