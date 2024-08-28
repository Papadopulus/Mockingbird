#include "pch.h"
#include "FileForm.h"
#include "CommonFunctions.h"

using namespace MockingApplication;

FileForm::FileForm(void)
{
	InitializeComponent();
}
FileForm::~FileForm()
{
	if (components)
	{
		delete components;
	}
}
System::Void FileForm::InitializeComponent(void)
{
	this->labelTitle = (gcnew System::Windows::Forms::Label());
	this->labelDescription = (gcnew System::Windows::Forms::Label());
	this->mockSingleFile_btn = (gcnew System::Windows::Forms::Button());
	this->SuspendLayout();
	// 
	// labelTitle
	// 
	this->labelTitle->AutoSize = true;
	this->labelTitle->Font = (gcnew System::Drawing::Font(L"Stencil", 20.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(0)));
	this->labelTitle->Location = System::Drawing::Point(195, 48);
	this->labelTitle->Name = L"labelTitle";
	this->labelTitle->Size = System::Drawing::Size(319, 32);
	this->labelTitle->TabIndex = 0;
	this->labelTitle->Text = L"Mocking a Single File";
	// 
	// labelDescription
	// 
	this->labelDescription->AutoSize = true;
	this->labelDescription->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(238)));
	this->labelDescription->Location = System::Drawing::Point(97, 131);
	this->labelDescription->Name = L"labelDescription";
	this->labelDescription->Size = System::Drawing::Size(526, 84);
	this->labelDescription->TabIndex = 1;
	this->labelDescription->Text = L"How to use:\r\n1. Click on the \"Mock\" button to select a specific file that you wan"
		L"t to mock.\r\n2. Choose a location where you want to save the mocked file.\r\n3. You"
		L"r mock file is ready to use.\r\n";
	// 
	// mockSingleFile_btn
	// 
	this->mockSingleFile_btn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
		static_cast<System::Byte>(238)));
	this->mockSingleFile_btn->Location = System::Drawing::Point(246, 286);
	this->mockSingleFile_btn->Name = L"mockSingleFile_btn";
	this->mockSingleFile_btn->Size = System::Drawing::Size(192, 44);
	this->mockSingleFile_btn->TabIndex = 2;
	this->mockSingleFile_btn->Text = L"Mock";
	this->mockSingleFile_btn->UseVisualStyleBackColor = true;
	this->mockSingleFile_btn->Click += gcnew System::EventHandler(this, &FileForm::mockSingleFile_btn_Click);
	// 
	// FileForm
	// 
	this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
	this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
	this->ClientSize = System::Drawing::Size(725, 470);
	this->Controls->Add(this->mockSingleFile_btn);
	this->Controls->Add(this->labelDescription);
	this->Controls->Add(this->labelTitle);
	this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
	this->Name = L"FileForm";
	this->Text = L"FileForm";
	this->Load += gcnew System::EventHandler(this, &FileForm::fileForm_Load);
	this->ResumeLayout(false);
	this->PerformLayout();

}
System::Void FileForm::fileForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	this->ControlBox = false;
}
System::Void FileForm::mockSingleFile_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Creating and configuring OpenFileDialog to open a dialog
	OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog();
	// Setting the filter to show only .c files
	openFileDialog1->Filter = "C files (*.c)|*.c|All files (*.*)|*.*";
	// Set the filter index to 1, so the first filter (C files) is selected by default
	openFileDialog1->FilterIndex = 1;
	// Restore the directory to the previously selected one
	openFileDialog1->RestoreDirectory = true;
	// Displaying a dialog and checking if the user has selected a file
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		try {
			// Getting the path of the selected file
			String^ filePath = openFileDialog1->FileName;

			// Converting String^ to std::string
			std::string filePathString = CommonFunctions::toStandardString(filePath);

			// Creating an instance of the MockingFiles class
			MockingFiles mockingFiles;

			// Passing the file path to the ProcessFile method
			std::vector<std::string> mockedFunctions = mockingFiles.ProcessFile(filePathString,nullptr);

			MessageBox::Show("File successfully mocked. Now choose where you want to save the mocked file.");

			// Create a new instance of SaveFileDialog
			SaveFileDialog^ saveFileDialog1 = gcnew SaveFileDialog();
			// Set the default file name in the dialog to the filename of the original file
			saveFileDialog1->FileName = gcnew String(std::filesystem::path(filePathString).filename().string().c_str());
			// Set the filter to show only C files and all files
			saveFileDialog1->Filter = "C files (*.c)|*.c|All files (*.*)|*.*";
			// Set the filter index to 1, so the first filter (C files) is selected by default
			saveFileDialog1->FilterIndex = 1;
			// Restore the directory to the previously selected one
			saveFileDialog1->RestoreDirectory = true;

			// Show the dialog and check if the user clicked "Save"
			if (saveFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				// Convert the managed String^ to std::string for the save path
				std::string savePath = CommonFunctions::toStandardString(saveFileDialog1->FileName);
				// Call the SaveMockedFile function to save the mocked functions to the selected path
				CommonFunctions::SaveMockedFile(savePath, filePathString, mockedFunctions);
				MessageBox::Show("File successfully saved. You are now ready to use mocked file.");
			}
		}
		catch (const std::exception& e)
		{
			String^ errorMessage = gcnew String(e.what());
			MessageBox::Show("Error: " + errorMessage);
		}
	}
}

