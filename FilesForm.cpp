#include "pch.h"
#include "FilesForm.h"


using namespace MockingApplication;

FilesForm::FilesForm(void)
{
	InitializeComponent();

	this->slideTimer = gcnew Timer();
	this->slideTimer->Interval = 10; // Set interval to 10 ms
	this->slideTimer->Tick += gcnew EventHandler(this, &FilesForm::slideTimer_Tick);
	this->isOpenTimer = false;

	bgWorker = gcnew BackgroundWorker();
	bgWorker->WorkerReportsProgress = true;
	bgWorker->WorkerSupportsCancellation = true;

	bgWorker->DoWork += gcnew DoWorkEventHandler(this, &FilesForm::bgWorker_DoWork);
	bgWorker->ProgressChanged += gcnew ProgressChangedEventHandler(this, &FilesForm::bgWorker_ProgressChanged);
	bgWorker->RunWorkerCompleted += gcnew RunWorkerCompletedEventHandler(this, &FilesForm::bgWorker_RunWorkerCompleted);
}
FilesForm::~FilesForm()
{
	if (components)
	{
		delete components;
	}
}
void FilesForm::InitializeComponent(void)
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
System::Void FilesForm::filesForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	// Disables the ControlBox (minimize, maximize, close) for this form
	this->ControlBox = false;
}
System::Void FilesForm::mutFolder_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Create and open a folder browser dialog
	BetterFolderBrowser^ folderDialog = gcnew BetterFolderBrowser();
	String^ lastMutPath = LoadLastUsedPath("mut");
	folderDialog->RootFolder = (lastMutPath != nullptr) ? lastMutPath : Directory::GetCurrentDirectory();
	if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {


		// Get the selected folder path
		String^ selectedPath = folderDialog->SelectedFolder;

		// Save parent path for checking /mocks folder
		applicationRootPath = selectedPath;

		SaveLastUsedPath("mut", selectedPath);
		// Call the function to process files in the selected folder
		ProcessFilesInFolder(selectedPath);
	}
}
void FilesForm::ProcessFilesInFolder(String^ folderPath)
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
	std::unordered_set<std::string> uniqueIncludes;
	// Get #include directives from each file and add them to the includes vector
	for (const auto& file : allFiles) {
		auto fileIncludes = GetIncludesFromFile(file);
		uniqueIncludes.insert(fileIncludes.begin(), fileIncludes.end());
	}
	// Initialization of the managed list
	this->includes = gcnew System::Collections::Generic::List<System::String^>();

	// Iterating through a std::vector and adding elements to a managed list
	for (const auto& include : uniqueIncludes)
	{
		this->includes->Add(gcnew System::String(include.c_str()));
	}
	// Display #include directives to the user (for now just print to console)
	DisplayIncludesToUser();
}
std::vector<std::string> FilesForm::GetIncludesFromFile(const std::string& filePath)
{
	// Create a vector to store #include directives
	std::vector<std::string> includes;
	// Open the file for reading
	std::ifstream file(filePath);
	if (!file.is_open()) {
		throw std::runtime_error("Could not open file: " + filePath);
	}
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
void FilesForm::DisplayIncludesToUser()
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
	for each (String ^ include in this->includes) {
		if (excludedIncludes.find(CommonFunctions::toStandardString(include)) == excludedIncludes.end()) {

			overlay->checkedListBox->Items->Add(include);
		}
	}
	overlay->next_btn->Enabled = false;
	overlay->checkedListBox->ItemCheck += gcnew System::Windows::Forms::ItemCheckEventHandler(this, &FilesForm::checkedListBox_ItemCheck);
	uncheckFiles();
}
void FilesForm::checkedListBox_ItemCheck(System::Object^ sender, System::Windows::Forms::ItemCheckEventArgs^ e)
{
	// Set a temporary counter to check how many items have been checked
	int checkedCount = overlay->checkedListBox->CheckedItems->Count;

	// If the item is currently being checked, increment the counter
	if (e->NewValue == CheckState::Checked)
	{
		checkedCount++;
	}
	// If the item is checked out, decrement the counter
	else if (e->NewValue == CheckState::Unchecked)
	{
		checkedCount--;
	}

	// Enable the Next button only if at least one item is checked
	overlay->next_btn->Enabled = (checkedCount > 0);
}
System::Void FilesForm::advanced_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (advancedForm == nullptr) {
		advancedForm = gcnew AdvancedSettingsForm();
		advancedForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
		advancedForm->StartPosition = FormStartPosition::Manual;
		advancedForm->ShowInTaskbar = false;
		advancedForm->Location = Point(this->MdiParent->Right - advancedForm->Width, this->MdiParent->Top);
		targetX = this->MdiParent->Right - 2;
		advancedForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FilesForm::AdvancedSettingsForm_FormClosed);
		advancedForm->apply_btn->Click += gcnew System::EventHandler(this, &FilesForm::apply_btn_Click);
		advancedForm->pictureBox1->Click += gcnew System::EventHandler(this, &FilesForm::picture_btn_Click);
		this->MdiParent->TopMost = true;
		this->MdiParent->LocationChanged += gcnew System::EventHandler(this, &FilesForm::MainForm_LocationChanged);
		advancedForm->Show();
		isOpenTimer = false;
		slideTimer->Start();
	}
	else {
		advancedForm->Activate();
	}
	this->MdiParent->TopMost = false;
}
System::Void FilesForm::picture_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	isOpenTimer = true;
	this->MdiParent->TopMost = true;
	slideTimer->Start();
}
System::Void FilesForm::AdvancedSettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	// Set the advancedForm form to null when it is closed
	advancedForm = nullptr;
}
System::Void FilesForm::apply_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Save items to a settings file
	std::ofstream file("excluded_includes.txt");
	for (int i = 0; i < advancedForm->listBox->Items->Count; i++) {
		file << CommonFunctions::toStandardString(advancedForm->listBox->Items[i]->ToString()) << std::endl;
	}
	file.close();
	DisplayIncludesToUser();
}
System::Void FilesForm::next_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (overlay->checkedListBox->CheckedItems->Count == 0)
	{
		MessageBox::Show("You must select at least one item before proceeding.");
		return;
	}
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
System::Void FilesForm::back_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (advancedForm != nullptr)
	{
		advancedForm->Close();
	}
	overlay->Close();
}
System::Void FilesForm::initial_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	uncheckFiles();
}
System::Void FilesForm::checkAllUncheckAll_btn_Click(System::Object^ sender, System::EventArgs^ e)
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
	}
}
void FilesForm::SelectSrcFolder(std::vector<std::string> selectedFiles)
{
	// Create and open a folder browser dialog
	BetterFolderBrowser^ folderDialog = gcnew BetterFolderBrowser();

	String^ lastSrcPath = LoadLastUsedPath("src");
	folderDialog->RootFolder = (lastSrcPath != nullptr) ? lastSrcPath : Directory::GetCurrentDirectory();

	if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		// Show ProgressForm
		progressForm = gcnew ProgressForm();
		progressForm->Show();
		progressForm->StartPosition = FormStartPosition::Manual;
		progressForm->MdiParent = this->MdiParent;
		progressForm->Dock = System::Windows::Forms::DockStyle::Fill;
		progressForm->metroProgressBar1->Maximum = selectedFiles.size();
		progressForm->metroProgressBar1->Value = 0;
		progressForm->status_lbl->Text = "Mocking files...";
		progressForm->Refresh();


		// Convert managed string to standard string
		std::string selectedPath = CommonFunctions::toStandardString(folderDialog->SelectedPath);

		SaveLastUsedPath("src", folderDialog->SelectedPath);

		List<String^>^ managedSelectedFiles = gcnew List<String^>();
		for (const auto& file : selectedFiles)
		{
			managedSelectedFiles->Add(gcnew String(file.c_str()));
		}
		FileProcessArgs^ args = gcnew FileProcessArgs(managedSelectedFiles, folderDialog->SelectedPath);
		bgWorker->RunWorkerAsync(args);

		//int progress = 0;
		//// Recursively search folder and process files
		//for (const auto& file : selectedFiles)
		//{
		//	ProcessSelectedFileInFolder(file, selectedPath);
		//	progress++;
		//	progressForm->metroProgressBar1->Value = progress;
		//	Application::DoEvents();
		//}
		//if (overlay != nullptr)
		//{
		//	overlay->Close();

		//}
		//// Close ProgressForm when mopping is complete
		//progressForm->status_lbl->Text = "Mocking finished.";
		//System::Threading::Thread::Sleep(2000);
		//progressForm->Close();

	}
}
void FilesForm::bgWorker_DoWork(Object^ sender, DoWorkEventArgs^ e)
{
	BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
	FileProcessArgs^ args = dynamic_cast<FileProcessArgs^>(e->Argument);

	List<String^>^ selectedFiles = args->selectedFiles;
	String^ selectedPath = args->selectedPath;

	int progress = 0;
	for each (String ^ file in selectedFiles)
	{
		std::string filePath = CommonFunctions::toStandardString(file);
		std::string path = CommonFunctions::toStandardString(selectedPath);

		ProcessSelectedFileInFolder(filePath, path);
		progress++;
		worker->ReportProgress(progress);
	}
}
void FilesForm::bgWorker_ProgressChanged(Object^ sender, ProgressChangedEventArgs^ e)
{
	progressForm->metroProgressBar1->Value = e->ProgressPercentage;
	progressForm->Refresh();
}
void FilesForm::bgWorker_RunWorkerCompleted(Object^ sender, RunWorkerCompletedEventArgs^ e)
{
	if (overlay != nullptr)
	{
		overlay->Close();
	}

	progressForm->status_lbl->Text = "Mocking finished.";
	System::Threading::Thread::Sleep(1000);
	progressForm->Close();
}
void FilesForm::ProcessSelectedFileInFolder(const std::string& fileName, const std::string& folderPath)
{
	try {
		// Get the parent path of the application root path
		std::string applicationRootPathStd = std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string();
		MockingFiles mockingFiles;
		// Recursively iterate through the directory to find the selected file
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
		{
			if (entry.is_directory() && entry.path().filename() == ".svn")
			{
				continue; // Skip .svn directories
			}
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
	catch (const std::exception& e)
	{
		// Show an error message if an exception occurs
		String^ errorMessage = gcnew String(e.what());
		MessageBox::Show("Error: " + errorMessage);
	}
}
System::Void FilesForm::OverlayForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	// Set the overlay form to null when it is closed
	overlay = nullptr;
}
void FilesForm::slideTimer_Tick(System::Object^ sender, System::EventArgs^ e)
{
	if (isOpenTimer == false)
	{
		if (advancedForm->Location.X < targetX)
		{
			advancedForm->Location = Point(advancedForm->Location.X + 20, advancedForm->Location.Y);
		}
		else
		{
			advancedForm->Location = Point(this->MdiParent->Location.X + this->MdiParent->Width, this->MdiParent->Location.Y);
			slideTimer->Stop();
			isOpenTimer = true;
		}
	}
	else
	{
		if (advancedForm->Location.X > (targetX - advancedForm->Width))
		{
			advancedForm->Location = Point(advancedForm->Location.X - 20, advancedForm->Location.Y);
		}
		else
		{
			advancedForm->Close();
			this->MdiParent->TopMost = false;
			slideTimer->Stop();
			isOpenTimer = false;
		}
	}
	
}
System::Void FilesForm::MainForm_LocationChanged(System::Object^ sender, System::EventArgs^ e)
{
	if (advancedForm != nullptr)
	{
		// Move the AdvancedForm relative to the new MainForm location
		advancedForm->Location = System::Drawing::Point(this->MdiParent->Location.X + this->MdiParent->Width, this->MdiParent->Location.Y);

	}
}
void FilesForm::uncheckFiles()
{
	try {
		// Get the parent path of the application root path
		std::string parentPath = std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string();

		// Recursively find all files in the parent folder
		std::unordered_set<std::string> parentFolderFiles;
		for (const auto& entry : std::filesystem::recursive_directory_iterator(parentPath))
		{
			if (entry.is_directory() && entry.path().filename() == ".svn")
			{
				continue; // Skip .svn directories
			}
			if (entry.is_regular_file())
			{
				parentFolderFiles.insert(entry.path().filename().string());
			}
		}

		// Uncheck the files in the checkedListBox
		for (int i = 0; i < overlay->checkedListBox->Items->Count; i++)
		{
			String^ item = overlay->checkedListBox->Items[i]->ToString();
			std::string itemStd = CommonFunctions::toStandardString(item);

			bool isChecked = (parentFolderFiles.find(itemStd) != parentFolderFiles.end());
			overlay->checkedListBox->SetItemChecked(i, !isChecked); // Uncheck if found, otherwise check it
		}
	}
	catch (const std::exception& e)
	{
		// Show an error message if an exception occurs
		String^ errorMessage = gcnew String(e.what());
		MessageBox::Show("Error: " + errorMessage);
	}
}
String^ FilesForm::LoadLastUsedPath(String^ key)
{
	String^ configFilePath = "last_used_paths.txt";

	if (!File::Exists(configFilePath))
		return nullptr;

	StreamReader^ reader = gcnew StreamReader(configFilePath);
	String^ line;
	while ((line = reader->ReadLine()) != nullptr)
	{
		array<String^>^ parts = line->Split('=');
		if (parts->Length == 2 && parts[0]->Trim() == key)
		{
			reader->Close();
			return parts[1]->Trim();
		}
	}
	reader->Close();
	return nullptr;
}
void FilesForm::SaveLastUsedPath(String^ key, String^ path)
{
	String^ configFilePath = "last_used_paths.txt";
	Dictionary<String^, String^>^ paths = gcnew Dictionary<String^, String^>();

	if (File::Exists(configFilePath))
	{
		StreamReader^ reader = gcnew StreamReader(configFilePath);
		String^ line;
		while ((line = reader->ReadLine()) != nullptr)
		{
			array<String^>^ parts = line->Split('=');
			if (parts->Length == 2)
			{
				paths[parts[0]->Trim()] = parts[1]->Trim();
			}
		}
		reader->Close();
	}

	paths[key] = path;

	StreamWriter^ writer = gcnew StreamWriter(configFilePath);
	for each (KeyValuePair<String^, String^> kvp in paths)
	{
		writer->WriteLine(kvp.Key + "=" + kvp.Value);
	}
	writer->Close();
}