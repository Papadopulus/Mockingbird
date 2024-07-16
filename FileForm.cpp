#include "pch.h"
#include "FileForm.h"

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
			std::string filePathString = toStandardString(filePath);

			// Creating an instance of the MockingFiles class
			MockingFiles mockingFiles;

			// Passing the file path to the ProcessFile method
			std::vector<std::string> mockedFunctions = mockingFiles.ProcessFile(filePathString);

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
				std::string savePath = toStandardString(saveFileDialog1->FileName);
				// Call the SaveMockedFile function to save the mocked functions to the selected path
				SaveMockedFile(savePath, filePathString, mockedFunctions);
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
std::string FileForm::toStandardString(System::String^ string)
{
	// Imports the Marshal class from the System::Runtime::InteropServices namespace
	using System::Runtime::InteropServices::Marshal;
	// Converts a managed string to an unmanaged ANSI string
	System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
	// Casts IntPtr to a char pointer
	char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
	// Creates a std::string from a char pointer using the length of the managed string
	std::string returnString(charPointer, string->Length);
	// Frees memory allocated for an unmanaged string
	Marshal::FreeHGlobal(pointer);

	return returnString;
}
void FileForm::SaveMockedFile(const std::string& mockedFilePath, const std::string& a_originalFilePath, const std::vector<std::string>& a_mockFunctions)
{
	// Creating an output stream to write to a file with mockedFilePath
	std::ofstream writer(mockedFilePath);

	writer << "#include <stdarg.h>\n";
	writer << "#include <stddef.h>\n";
	writer << "#include <setjmp.h>\n";
	writer << "#include <string.h>\n";
	writer << "#include \"cmocka.h\"\n";


	std::string directory = std::filesystem::path(a_originalFilePath).parent_path().string();
	std::string baseFileName = std::filesystem::path(a_originalFilePath).stem().string();
	std::vector<std::string> headerFiles;

	// A loop that goes through all the files in the directory
	for (const auto& entry : std::filesystem::directory_iterator(directory))
	{
		// Checks if the filename contains baseFileName and if the file has a .h extension. and adds the path to the headerFiles variable
		if (entry.path().filename().string().find(baseFileName) != std::string::npos && entry.path().extension() == ".h")
		{
			headerFiles.push_back(entry.path().string());
		}
	}
	// A loop that goes through the headerFiles and writes to the output file #include for each header file
	for (const auto& headerFile : headerFiles)
	{
		writer << "#include \"" << std::filesystem::path(headerFile).filename().string() << "\"\n";
	}
	// Each mock function is written to the output file
	writer << "\n// Mocked functions\n";
	for (const auto& mockFunction : a_mockFunctions)
	{
		writer << mockFunction;
	}
	writer.close();
}
