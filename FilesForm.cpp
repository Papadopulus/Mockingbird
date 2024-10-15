#include "pch.h"
#include "FilesForm.h"
#include <windows.h>

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
System::Void FilesForm::add_static_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	InsertStaticMacroInFiles(applicationRootPath);
	ReplaceStaticInFiles(applicationRootPath);
	ProcessFilesInFolderForStaticElements(applicationRootPath);

	// Disable button after click
	overlay->add_static_btn->Enabled = false;

	// Change button text
	overlay->add_static_btn->Text = "Static Added To Files";

}
void FilesForm::InsertStaticMacroInFiles(String^ folderPath)
{
	std::string directory = CommonFunctions::toStandardString(folderPath);

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.path().extension() == ".c") {
			std::ifstream file(entry.path().string());
			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			// Check if "Local Definitions" exists
			size_t pos = content.find("Local Definitions\n");

			// If it exists, add the macros below
			if (pos != std::string::npos) {
				// Find the end of a commented line and count two lines after that
				pos = content.find('\n', pos);  // End of line with "Local Definitions"
				pos = content.find('\n', pos + 1);  // Next line (first)

				// Insert the macros below the second line
				content.insert(pos + 1, "\n#ifdef UNIT_TESTING\n#define STATIC\n#define STATIC_INLINE\n#else\n#define STATIC static\n#define STATIC_INLINE static inline\n#endif\n");
			}
			// If it doesn't exist, add "// Local Definitions" and macros to the end of the file
			else {
				content.append("\n// Local Definitions\n#ifdef UNIT_TESTING\n#define STATIC\n#define STATIC_INLINE\n#else\n#define STATIC static\n#define STATIC_INLINE static inline\n#endif\n");
			}

			// Save the changed content back to the file
			std::ofstream outFile(entry.path().string());
			outFile << content;
			outFile.close();
		}
	}
	// Go back one folder and look for main.c
	std::filesystem::path parentDirectory = std::filesystem::path(directory).parent_path();
	std::filesystem::path mainFilePath = parentDirectory / "main.c";

	if (std::filesystem::exists(mainFilePath)) {
		std::ifstream mainFile(mainFilePath.string());
		std::string mainContent((std::istreambuf_iterator<char>(mainFile)), std::istreambuf_iterator<char>());
		mainFile.close();

		// Find the "Local Definitions" comment
		size_t pos = mainContent.find("Local Definitions\n");

		// If "Local Definitions" comment is found
		if (pos != std::string::npos) {
			// Find the end of a commented line and count two lines after that
			pos = mainContent.find('\n', pos); // End of line with "// Local Definitions"
			pos = mainContent.find('\n', pos + 1);  // Next line (first)

			// Insert the macros below the second line
			mainContent.insert(pos + 1, "\n#ifdef UNIT_TESTING\n#define STATIC\n#define STATIC_INLINE\n#else\n#define STATIC static\n#define STATIC_INLINE static inline\n#endif\n");
		}
		// If there are no "Local Definitions", add to the end of the file
		else {
			mainContent.append("\n//	Local Definitions\n#ifdef UNIT_TESTING\n#define STATIC\n#define STATIC_INLINE\n#else\n#define STATIC static\n#define STATIC_INLINE static inline\n#endif\n");
		}

		// Write the changed content back to main.c
		std::ofstream mainOutFile(mainFilePath.string());
		mainOutFile << mainContent;
		mainOutFile.close();
	}
}
void FilesForm::ReplaceStaticInFiles(String^ folderPath)
{
	std::string directory = CommonFunctions::toStandardString(folderPath);

	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.path().extension() == ".c") {
			std::ifstream file(entry.path().string());
			std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();

			std::string modifiedContent;
			bool insideFunctionBody = false;

			std::istringstream contentStream(content);
			std::string line;

			// Regex for static inline (except in function body)
			std::regex staticInlineRegex(R"(\bstatic\s+inline\b)");

			// Regex for static (except in function body)
			std::regex staticRegex(R"(\bstatic\b)");

			while (std::getline(contentStream, line)) {
				// If we find an opening parenthesis in the line, we are inside the body of the function
				if (line.find('{') != std::string::npos) {
					insideFunctionBody = true;
				}

				// If we find a closing parenthesis in the line, we exit the function body
				if (line.find('}') != std::string::npos) {
					insideFunctionBody = false;
				}

				// Check if the line contains #define
				if (line.find("#define") == std::string::npos) {
					// If the line does not contain a #define and we are not inside the function body
					if (!insideFunctionBody) {
						// Change static inline to STATIC_INLINE
						line = std::regex_replace(line, staticInlineRegex, "STATIC_INLINE");

						// Change static to STATIC (but only if we're not inside the function body)
						line = std::regex_replace(line, staticRegex, "STATIC");
					}
				}

				// Add a line to the modified content
				modifiedContent += line + "\n";
			}

			// We write the changed content back to the file
			std::ofstream outFile(entry.path().string());
			outFile << modifiedContent;
			outFile.close();
		}
	}
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
void FilesForm::ProcessFilesInFolderForStaticElements(String^ folderPath)
{
	// Create a vector to store all file paths
	std::vector<std::string> staticElements;
	// Convert managed string to standard string
	std::string directory = std::filesystem::path(CommonFunctions::toStandardString(folderPath)).string();

	// Iterate through the directory to find .c and .h files
	for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.path().extension() == ".c") {
			// Process the .c file and append static elements to the vector
			std::vector<std::string> fileStaticElements = mockingFiles.ProcessFileStatic(entry.path().string());
			staticElements.insert(staticElements.end(), fileStaticElements.begin(), fileStaticElements.end());
		}
	}
	// Now, go one directory back and find the main.c file
	std::filesystem::path mainFilePath = std::filesystem::path(directory).parent_path() / "main.c";

	if (std::filesystem::exists(mainFilePath)) {
		std::ifstream mainFile(mainFilePath.string());
		std::string mainContent((std::istreambuf_iterator<char>(mainFile)), std::istreambuf_iterator<char>());
		mainFile.close();

		// Find the "Local Definitions" comment
		size_t pos = mainContent.find("Local Definitions\n");

		// Convert static elements vector to a single string
		std::string staticElementsString;
		for (const auto& element : staticElements) {
			staticElementsString += element + "\n";
		}

		// If "Local Definitions" comment is found
		if (pos != std::string::npos) {
			// Find the end of a commented line and count two lines after that
			pos = mainContent.find('\n', pos);  // End of line with "// Local Definitions"
			pos = mainContent.find('\n', pos + 1);  // Next line (first)
			pos = mainContent.find('\n', pos + 1);  // Second line

			// Insert static elements after "Local Variables"
			mainContent.insert(pos, staticElementsString);
		}
		// If there are no "Local Definitions", add to the end of the file
		else {
			mainContent.append("\n//	Local Definitions\n" + staticElementsString);
		}

		// Write the changed content back to main.c
		std::ofstream mainOutFile(mainFilePath.string());
		mainOutFile << mainContent;
		mainOutFile.close();
	}
}
std::vector<std::string> FilesForm::GetIncludesFromFile(const std::string& filePath)
{
	// Create a vector to store #include directives
	std::vector<std::string> includes;
	// Get the directory of the current file
	std::string currentDirectory = std::filesystem::path(filePath).parent_path().string();
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
			std::string includePath = match[1].str();

			// Check if the included file is in the same directory
			std::filesystem::path fullPath = std::filesystem::path(currentDirectory) / includePath;

			// If the full path of the include file is not in the selected folder, add it to the list
			if (!std::filesystem::exists(fullPath) || fullPath.parent_path().string() != currentDirectory) {
				includes.push_back(includePath);
			}
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
		overlay->mockingSettings_btn->Click += gcnew System::EventHandler(this, &FilesForm::mockingSettings_btn_Click);
		overlay->add_static_btn->Click += gcnew System::EventHandler(this, &FilesForm::add_static_btn_Click);


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
	overlay->mockingSettings_btn->Enabled = false;
	overlay->checkedListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &FilesForm::checkedListBox_SelectedIndexChanged);
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
System::Void FilesForm::mockingSettings_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (srcPath == nullptr)
	{
		MessageBox::Show("Please select the source folder that contains the selected file.");
		// Create and open a folder browser dialog
		BetterFolderBrowser^ folderDialog = gcnew BetterFolderBrowser();

		String^ lastSrcPath = LoadLastUsedPath("src");
		folderDialog->RootFolder = (lastSrcPath != nullptr) ? lastSrcPath : Directory::GetCurrentDirectory();
		if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			srcPath = folderDialog->SelectedPath;
			SaveLastUsedPath("src", folderDialog->SelectedPath);
		}
	}
	if (srcPath)
	{
		if (mockingSettingsForm == nullptr)
		{
			mockingSettingsForm = gcnew MockingSettings();
			mockingSettingsForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::None;
			mockingSettingsForm->StartPosition = FormStartPosition::CenterParent;
			mockingSettingsForm->save_btn->Click += gcnew System::EventHandler(this, &FilesForm::save_btn_Click);
			mockingSettingsForm->apply_btn->Click += gcnew System::EventHandler(this, &FilesForm::apply_Mocking_Settings_btn_Click);
			mockingSettingsForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &FilesForm::MockingSettingsForm_FormClosed);
			mockingSettingsForm->checkedListBox->SelectedIndexChanged += gcnew System::EventHandler(this, &FilesForm::CheckListBox_SelectedIndexChanged);
			ProcessFileForMockingSettings();
			mockingSettingsForm->ShowDialog();
		}
		else
		{
			mockingSettingsForm->Activate();
		}
	}
	else
	{
		// Handle case where no folder is selected (dialog canceled or closed)
		MessageBox::Show("No folder selected. Please select a source folder to proceed.");
	}

}
System::Void FilesForm::apply_Mocking_Settings_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Working with the currently selected function in the checkedListBox
	int selectedIndex = mockingSettingsForm->checkedListBox->SelectedIndex;

	if (selectedIndex < 0) {
		MessageBox::Show("You have not selected any features.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	// Retrieve the currently selected item
	String^ itemString = mockingSettingsForm->checkedListBox->Items[selectedIndex]->ToString();

	// Parse the string to obtain the function name and return type
	FunctionSettings^ funcSettings = gcnew FunctionSettings();
	std::string itemStdString = CommonFunctions::toStandardString(itemString);
	auto tabPosition = itemStdString.find('\t');
	if (tabPosition != std::string::npos) {
		std::string returnType = itemStdString.substr(0, tabPosition);
		std::string functionName = itemStdString.substr(tabPosition + 1);

		funcSettings->returnType = gcnew String(returnType.c_str());
		funcSettings->functionName = gcnew String(functionName.c_str());
	}

	// Retrieve parameter settings
	for (int j = 0; j < mockingSettingsForm->flowLayoutPanel->Controls->Count; j += 2) {
		Label^ lbl = dynamic_cast<Label^>(mockingSettingsForm->flowLayoutPanel->Controls[j]);
		ComboBox^ cb = dynamic_cast<ComboBox^>(mockingSettingsForm->flowLayoutPanel->Controls[j + 1]);

		if (lbl != nullptr && cb != nullptr) {
			ParameterSettings^ paramSettings = gcnew ParameterSettings();
			paramSettings->paramName = lbl->Text->Replace(":", "");
			paramSettings->setting = cb->SelectedItem->ToString();
			funcSettings->parameters->Add(paramSettings);
		}
	}

	// Check if both "Ptr" and "Length" parameters are set
	bool hasPtr = false;
	bool hasLength = false;

	for each (ParameterSettings ^ param in funcSettings->parameters) {
		if (param->setting == "Ptr") {
			hasPtr = true;
		}
		if (param->setting == "Length") {
			hasLength = true;
		}
	}

	// If either parameter is not set, display a message and stop saving
	if (!hasPtr || !hasLength) {
		MessageBox::Show("You must set the 'Ptr' and 'Length' parameters before applying the changes.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		return;
	}

	// Add the function to the list of saved settings
	String^ currentIncludeName = overlay->checkedListBox->SelectedItem->ToString();
	IncludeModuleSettings^ currentModule = nullptr;

	for each (IncludeModuleSettings ^ module in includeSettings) {
		if (module->includeName == currentIncludeName) {
			currentModule = module;
			break;
		}
	}

	if (currentModule == nullptr) {
		currentModule = gcnew IncludeModuleSettings();
		currentModule->includeName = currentIncludeName;
		includeSettings->Add(currentModule);
	}

	// Remove the function if it already exists in the list (to avoid duplication)
	for (int i = 0; i < currentModule->functions->Count; i++) {
		if (currentModule->functions[i]->functionName == funcSettings->functionName) {
			currentModule->functions->RemoveAt(i);
			break;
		}
	}

	// Add the new function with the new settings
	currentModule->functions->Add(funcSettings);

	MessageBox::Show("Settings successfully applied.", "Success", MessageBoxButtons::OK, MessageBoxIcon::Information);
}
System::Void FilesForm::save_btn_Click(System::Object^ sender, System::EventArgs^ e)
{
	// Find the current module based on the selected include
	String^ currentIncludeName = overlay->checkedListBox->SelectedItem->ToString();
	IncludeModuleSettings^ currentModule = nullptr;

	for each (IncludeModuleSettings ^ module in includeSettings)
	{
		if (module->includeName == currentIncludeName)
		{
			currentModule = module;
			break;
		}
	}

	if (currentModule == nullptr)
	{
		MessageBox::Show("You haven't changed anything", "Failure", MessageBoxButtons::OK, MessageBoxIcon::Information);
	}
	// Create a list of functions to keep (those that are checked)
	std::vector<std::string> checkedFunctions;

	for (int i = 0; i < mockingSettingsForm->checkedListBox->Items->Count; i++)
	{
		if (mockingSettingsForm->checkedListBox->GetItemChecked(i))
		{
			std::string itemStdString = CommonFunctions::toStandardString(mockingSettingsForm->checkedListBox->Items[i]->ToString());
			auto tabPosition = itemStdString.find('\t');
			if (tabPosition != std::string::npos) {
				checkedFunctions.push_back(itemStdString.substr(tabPosition + 1));
			}

		}
	}
	// Iterate through the functions in reverse order and remove unchecked functions
	for (int i = currentModule->functions->Count - 1; i >= 0; i--)
	{
		bool isFunctionChecked = false;

		for (int j = 0; i < checkedFunctions.size(); j++)
		{
			if (currentModule->functions[i]->functionName == gcnew String(checkedFunctions[j].c_str()))
			{
				isFunctionChecked = true;
				break;
			}
		}

		// If the function is not checked, remove it from the list
		if (!isFunctionChecked)
		{
			currentModule->functions->RemoveAt(i);
		}
	}
	// Close the mocking settings form
	mockingSettingsForm->Close();
}
void FilesForm::ProcessFileForMockingSettings()
{
	try {
		// Recursively iterate through the directory to find the selected file
		for (const auto& entry : std::filesystem::recursive_directory_iterator(CommonFunctions::toStandardString(srcPath)))
		{
			if (entry.is_directory() && entry.path().filename() == ".svn")
			{
				continue; // Skip .svn directories
			}
			// If the file name matches, process it
			if (entry.path().filename() == CommonFunctions::toStandardString(overlay->checkedListBox->SelectedItem->ToString()))
			{

				// Get the original folder and create a new folder path
				std::string originalFolder = entry.path().parent_path().string();
				for (const auto& file : std::filesystem::directory_iterator(originalFolder))
				{
					// Mock .c file
					if (file.path().extension() == ".c")
					{
						// Getting the path to the executable file
						std::string exePath = CommonFunctions::GetExecutablePath();

						// Relative path to ctags.exe
						std::string ctagsPath = exePath + "\\ctags\\ctags.exe";
						// Get the path to the temporary folder
						char tempPath[MAX_PATH];
						GetTempPathA(MAX_PATH, tempPath);
						// Relative path to the output tags file
						std::string tagsFilePath = std::filesystem::path(tempPath).parent_path().string() + "\\tags_" + std::filesystem::path(entry.path().filename()).stem().string();
						// Creating a command to run ctags.exe
						std::string command = ctagsPath + " --c-kinds=+px --fields=+iaS --languages=C -o \"" + tagsFilePath + "\" \"" + file.path().string() + "\"";
						// Setting up the process start information
						STARTUPINFOA si;
						PROCESS_INFORMATION pi;
						ZeroMemory(&si, sizeof(si));
						si.cb = sizeof(si);
						si.dwFlags |= STARTF_USESHOWWINDOW;
						si.wShowWindow = SW_HIDE; // Hide the window
						ZeroMemory(&pi, sizeof(pi));

						// Convert command to LPSTR
						std::vector<char> cmd(command.begin(), command.end());
						cmd.push_back('\0'); // Null-terminate the string

						// Starting the ctags process
						if (!CreateProcessA(NULL, cmd.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
						{
							throw std::runtime_error("Failed to start ctags process.");
						}

						// Waiting for the process to complete
						WaitForSingleObject(pi.hProcess, INFINITE);

						// Closing process and thread handles
						CloseHandle(pi.hProcess);
						CloseHandle(pi.hThread);
						// Checking the existence of the tags file and continuing processing
						if (std::filesystem::exists(tagsFilePath))
						{
							std::vector<std::string> functions = mockingFiles.ParseTagsFile(tagsFilePath);
							functionsForMockingSettings = CommonFunctions::ConvertToManagedList(functions);
							DisplayFunctionsForMockingSettings(functions);
						}
						else
						{
							throw std::runtime_error("Tags file does not exist.");
						}
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
void FilesForm::CheckListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	// First, clear previously added ComboBoxes.
	mockingSettingsForm->flowLayoutPanel->Controls->Clear();
	// Get the index of the selected item.
	int selectedIndex = mockingSettingsForm->checkedListBox->SelectedIndex;
	if (selectedIndex < 0) return;

	std::vector<std::string> functions = CommonFunctions::ConvertToStdVector(functionsForMockingSettings);
	std::string function = functions[selectedIndex];


	std::vector<std::string> functionParts;
	// Allows a function to be read as an input stream
	std::istringstream iss(function);
	std::string part;

	// It reads the function piece by piece, each time it encounters a tab. Each obtained part is added to the functionParts vector
	while (std::getline(iss, part, '\t'))
	{
		functionParts.push_back(part);
	}
	std::string returnType = functionParts[0];
	std::vector<std::string> restParts;
	// Allows functionParts[1](function name and parameters) to be read as an input stream
	std::istringstream iss2(functionParts[1]);
	// Splits functionParts[1] into function name and parameters
	while (std::getline(iss2, part, '('))
	{
		restParts.push_back(part);
	}
	std::string parameters = restParts[1];

	std::vector<std::string> paramList;
	// Allows parameters to be read as an input stream
	std::istringstream iss3(parameters);
	// Splits parameters into parameters by inserting a comma-separated string into the paramList
	while (std::getline(iss3, part, ','))
	{
		paramList.push_back(part);
	}

	// Find the function in the includeSettings list.
	String^ selectedFunctionName = gcnew String(restParts[0].c_str());
	IncludeModuleSettings^ currentModule = nullptr;

	for each (IncludeModuleSettings ^ module in includeSettings)
	{
		if (module->includeName == overlay->checkedListBox->SelectedItem->ToString())
		{
			currentModule = module;
			break;
		}
	}

	FunctionSettings^ existingFunction = nullptr;

	if (currentModule != nullptr)
	{
		for each (FunctionSettings ^ funcSettings in currentModule->functions)
		{
			if (funcSettings->functionName == selectedFunctionName)
			{
				existingFunction = funcSettings;
				break;
			}
		}
	}

	if (!(paramList.size() == 1 && paramList[0].find("void") != std::string::npos))
	{
		for (int i = 0; i < paramList.size(); i++)
		{
			const auto& param = paramList[i];

			// Create a label for the parameter.
			Label^ lbl = gcnew Label();
			lbl->Text = gcnew String(param.c_str()) + ":";
			lbl->AutoSize = true;
			lbl->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			lbl->Margin = System::Windows::Forms::Padding(0, 0, 20, 10);
			mockingSettingsForm->flowLayoutPanel->Controls->Add(lbl);

			// Create a ComboBox.
			ComboBox^ cb = gcnew ComboBox();
			cb->Items->Add("None");
			cb->Items->Add("Ptr");
			cb->Items->Add("Length");

			// Check if there is a saved value for this parameter.
			if (existingFunction != nullptr && i < existingFunction->parameters->Count)
			{
				String^ savedSetting = existingFunction->parameters[i]->setting;
				cb->SelectedItem = savedSetting;
			}
			else
			{
				// Set 'None' as the default value if there is no saved value.
				cb->SelectedIndex = 0;
			}

			cb->Margin = System::Windows::Forms::Padding(0, 0, 0, 10);
			mockingSettingsForm->flowLayoutPanel->Controls->Add(cb);
		}
	}

}
void FilesForm::DisplayFunctionsForMockingSettings(std::vector<std::string> functions)
{
	// First, check if there are already saved functions for this include file.
	String^ currentIncludeName = gcnew String(overlay->checkedListBox->SelectedItem->ToString());
	IncludeModuleSettings^ currentModule = nullptr;

	for each (IncludeModuleSettings ^ module in includeSettings)
	{
		if (module->includeName == currentIncludeName)
		{
			currentModule = module;
			break;
		}
	}
	System::Collections::Generic::Dictionary<System::String^, FunctionSettings^>^ savedFunctionsMap =
		gcnew System::Collections::Generic::Dictionary<System::String^, FunctionSettings^>();

	if (currentModule != nullptr)
	{
		for each (FunctionSettings ^ funcSettings in currentModule->functions)
		{
			savedFunctionsMap[funcSettings->returnType + "\t" + funcSettings->functionName] = funcSettings;
		}
	}

	// Clear existing items to avoid duplicates.
	mockingSettingsForm->checkedListBox->Items->Clear();

	// Add functions from functionsForMockingSettings in the same order.
	for (const auto& func : functions)
	{
		std::string functionName = ParseFunctionName(func);
		if (!functionName.empty())
		{
			String^ managedFunctionName = gcnew String(functionName.c_str());

			// Check if the function exists in the saved functions.
			if (savedFunctionsMap->ContainsKey(managedFunctionName))
			{
				// Add the saved function with a checkmark.
				FunctionSettings^ funcSettings = savedFunctionsMap[managedFunctionName];
				String^ functionDisplay = funcSettings->returnType + "\t" + funcSettings->functionName;
				int index = mockingSettingsForm->checkedListBox->Items->Add(functionDisplay);
				mockingSettingsForm->checkedListBox->SetItemChecked(index, true);
			}
			else
			{
				// Add a function that is not saved.
				String^ functionDisplay = gcnew String(functionName.c_str());
				mockingSettingsForm->checkedListBox->Items->Add(functionDisplay);
			}
		}
	}
}
std::string FilesForm::ParseFunctionName(const std::string& functionSignature) {
	// Find the start of the return type after the tab.
	auto start = functionSignature.find('\t');
	// Find the end of the function name before the parenthesis.
	auto end = functionSignature.find('(');

	// Check if both start and end are valid.
	if (start != std::string::npos && end != std::string::npos) {
		// Return a substring from the start of the return type to the end of the function name.
		return functionSignature.substr(0, end);
	}

	// If parsing is not possible, return an empty function name.
	return "Cannot get name of function from command";
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
System::Void FilesForm::MockingSettingsForm_FormClosed(System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e)
{
	// Set the mockingSettingsForm form to null when it is closed
	mockingSettingsForm = nullptr;
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
	mockedFilesInfo = gcnew List<MockedFileInfo^>();
	// Add each checked item from the checked list box to the vector
	for (int i = 0; i < overlay->checkedListBox->CheckedItems->Count; i++)
	{
		MockedFileInfo^ fileInfo = gcnew MockedFileInfo();
		fileInfo->fileName = overlay->checkedListBox->CheckedItems[i]->ToString();
		fileInfo->isDuplicate = false;
		mockedFilesInfo->Add(fileInfo);
	}
	// Call the function to select the source folder
	if (advancedForm != nullptr)
	{
		advancedForm->Close();
	}

	// Check for existing mock files

	CheckExistingMockedFiles(mockedFilesInfo);

	// If there are files that have already been mocked
	if (existingMockedFiles)
	{
		ShowOverwriteForm();
	}
	else
	{
		SelectSrcFolder();
	}
}
void FilesForm::CheckExistingMockedFiles(List<MockedFileInfo^>^ selectedFiles)
{
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
			std::string fileName = entry.path().filename().string();
			// Check if the file is in the list of selected files
			for each (MockedFileInfo ^ fileInfo in selectedFiles)
			{
				if (fileInfo->fileName == gcnew String(fileName.c_str()))
				{
					fileInfo->isDuplicate = true; // Mark as duplicate


					// We calculate the relative path according to parentPath
					std::filesystem::path relativePath = std::filesystem::relative(entry.path().parent_path(), parentPath);
					// Get the first folder from the relative path
					std::string parentFolderName = relativePath.begin()->string();
					// Check if the new parent folder is `lib'
					if (parentFolderName == "lib")
					{
						fileInfo->parentFolder = gcnew String(parentFolderName.c_str());
						fileInfo->originalPath = gcnew String(entry.path().parent_path().string().c_str());
					}
					// If the current folder is not `lib`, we check if we have already set it to `lib`
					else if (fileInfo->parentFolder != "lib")
					{
						fileInfo->parentFolder = gcnew String(parentFolderName.c_str());
						fileInfo->originalPath = gcnew String(entry.path().parent_path().string().c_str());
					}
					existingMockedFiles = true; // We set the flag to have duplicates
				}
			}
		}
	}

}
void FilesForm::ShowOverwriteForm()
{
	// Create a form to display files that have already been mocked
	overwriteForm = gcnew OverwriteForm();

	overwriteForm->StartPosition = FormStartPosition::Manual;
	overwriteForm->MdiParent = this->MdiParent;

	overwriteForm->overwrite_next_btn->Click += gcnew System::EventHandler(this, &FilesForm::overwrite_next_btn);
	overwriteForm->Dock = System::Windows::Forms::DockStyle::Fill;


	// Display the files in the corresponding CheckedListBoxes based on the parentFolder value
	for each (MockedFileInfo ^ fileInfo in mockedFilesInfo)
	{
		if (fileInfo->isDuplicate)
		{
			if (fileInfo->parentFolder == "cfg")
			{
				// Add files from cfg folder to cfgCheckedListBox
				overwriteForm->cfgCheckedListBox->Items->Add(fileInfo->fileName, true);
			}
			else if (fileInfo->parentFolder == "lib")
			{
				// Add files from lib folder to libCheckedListBox
				overwriteForm->libCheckedListBox->Items->Add(fileInfo->fileName, true);
			}
			else
			{
				// All other files go in otherCheckedListBox
				overwriteForm->otherCheckedListBox->Items->Add(fileInfo->fileName, true);
			}
		}
	}

	// Display the form
	overwriteForm->Show();
}
System::Void FilesForm::overwrite_next_btn(System::Object^ sender, System::EventArgs^ e)
{
	// Create a list of files that are not checked (will not be overwritten)
	List<MockedFileInfo^>^ toRemove = gcnew List<MockedFileInfo^>();

	for each (MockedFileInfo ^ fileInfo in mockedFilesInfo)
	{
		// If the file is a duplicate, check if it is checked in the form
		if (fileInfo->isDuplicate &&
			(!overwriteForm->cfgCheckedListBox->CheckedItems->Contains(fileInfo->fileName) &&
				!overwriteForm->libCheckedListBox->CheckedItems->Contains(fileInfo->fileName) &&
				!overwriteForm->otherCheckedListBox->CheckedItems->Contains(fileInfo->fileName)))
		{
			toRemove->Add(fileInfo); // If it's not checked, add it to the removal list
		}
	}

	// Remove all unchecked files
	for each (MockedFileInfo ^ fileInfo in toRemove)
	{
		mockedFilesInfo->Remove(fileInfo);
	}
	existingMockedFiles = false;
	SelectSrcFolder();
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
void FilesForm::SelectSrcFolder()
{
	if (srcPath == nullptr)
	{
		MessageBox::Show("Now select the source folder that contains the checked files.");
		// Create and open a folder browser dialog
		BetterFolderBrowser^ folderDialog = gcnew BetterFolderBrowser();

		String^ lastSrcPath = LoadLastUsedPath("src");
		folderDialog->RootFolder = (lastSrcPath != nullptr) ? lastSrcPath : Directory::GetCurrentDirectory();
		if (folderDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			srcPath = folderDialog->SelectedPath;
			SaveLastUsedPath("src", folderDialog->SelectedPath);
		}
	}
	if (srcPath)
	{
		overlay->Close();
		if (overwriteForm != nullptr)
		{
			overwriteForm->Close();
		}
		// Show ProgressForm
		progressForm = gcnew ProgressForm();
		progressForm->Show();
		progressForm->StartPosition = FormStartPosition::Manual;
		progressForm->MdiParent = this->MdiParent;
		progressForm->Dock = System::Windows::Forms::DockStyle::Fill;
		progressForm->metroProgressBar1->Maximum = mockedFilesInfo->Count;
		progressForm->metroProgressBar1->Value = 0;
		progressForm->status_lbl->Text = "Mocking files...";
		progressForm->Refresh();
		bgWorker->RunWorkerAsync(srcPath);
	}
	else
	{
		// Handle case where no folder is selected (dialog canceled or closed)
		MessageBox::Show("No folder selected. Please select a source folder to proceed.");
	}
}
void FilesForm::bgWorker_DoWork(Object^ sender, DoWorkEventArgs^ e)
{
	BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
	String^ srcPath = dynamic_cast<String^>(e->Argument); // Receive srcPath as an argument
	std::string standardSrcPath = CommonFunctions::toStandardString(srcPath); // Conversion to std::string

	int progress = 0;
	for each (MockedFileInfo ^ fileInfo in mockedFilesInfo)
	{
		// If duplicate, use original path to overwrite files
		if (fileInfo->isDuplicate)
		{
			ProcessSelectedFileInFolder(fileInfo, standardSrcPath, true); // true means it's a duplicate
		}
		else
		{
			ProcessSelectedFileInFolder(fileInfo, standardSrcPath, false); // false means it's not a duplicate
		}

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

	// Loop through the global list of files (mockedFilesInfo)
	List<FileEntry^>^ duplicateFiles = gcnew List<FileEntry^>();
	for each (MockedFileInfo ^ fileInfo in mockedFilesInfo)
	{
		if (fileInfo->isDuplicate && fileInfo->parentFolder == "lib")
		{
			// Find the relative path from the lib folder onwards
			std::filesystem::path fullPath = CommonFunctions::toStandardString(fileInfo->originalPath);
			std::filesystem::path relativePath = std::filesystem::relative(fullPath, std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string());
			// Add a FileEntry structure that stores both relative and fullPath
			duplicateFiles->Add(gcnew FileEntry(
				gcnew String((relativePath.string() + "\\" + CommonFunctions::toStandardString(fileInfo->fileName)).c_str()),
				gcnew String((fullPath.string() + "\\" + CommonFunctions::toStandardString(fileInfo->fileName)).c_str())));

		}
	}
	// Finish the status of the progress bar
	progressForm->status_lbl->Text = "Mocking finished.";
	progressForm->Close();

	if (duplicateFiles->Count > 0)
	{
		duplicateForm = gcnew DuplicateFilesForm();
		duplicateForm->StartPosition = FormStartPosition::Manual;
		duplicateForm->MdiParent = this->MdiParent;
		duplicateForm->Dock = System::Windows::Forms::DockStyle::Fill;
		duplicateForm->listBox1->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &FilesForm::OnFileDoubleClick);
		// Add all files to ListBox directly
		for each (FileEntry ^ fileEntry in duplicateFiles)
		{
			duplicateForm->listBox1->Items->Add(fileEntry);
		}

		// Show the form
		duplicateForm->Show();

	}
	srcPath = nullptr;

}
void FilesForm::OnFileDoubleClick(Object^ sender, MouseEventArgs^ e)
{
	if (duplicateForm->listBox1->SelectedItem != nullptr)
	{
		// Get the selected FileEntry
		FileEntry^ selectedFile = dynamic_cast<FileEntry^>(duplicateForm->listBox1->SelectedItem);

		if (selectedFile != nullptr && System::IO::File::Exists(selectedFile->fullPath))
		{
			// Open File Explorer and display the file
			System::Diagnostics::Process::Start("explorer.exe", "/select,\"" + selectedFile->fullPath + "\"");
		}
	}
}
void FilesForm::ProcessSelectedFileInFolder(MockedFileInfo^ fileInfo, const std::string& folderPath, bool isDuplicate)
{
	try {
		// Get the parent path of the application root path
		std::string applicationRootPathStd = std::filesystem::path(CommonFunctions::toStandardString(applicationRootPath)).parent_path().string();
		// Find the include mod that matches the file
		IncludeModuleSettings^ inclSettings = nullptr;
		for each (IncludeModuleSettings ^ settings in includeSettings) {
			if (settings->includeName == fileInfo->fileName) {
				inclSettings = settings;
				break;
			}
		}
		// Recursively iterate through the directory to find the selected file
		for (const auto& entry : std::filesystem::recursive_directory_iterator(folderPath))
		{
			if (entry.is_directory() && entry.path().filename() == ".svn")
			{
				continue; // Skip .svn directories
			}
			// If the file name matches, process it
			if (entry.path().filename() == CommonFunctions::toStandardString(fileInfo->fileName))
			{
				// Get the original folder and create a new folder path
				std::string originalFolder = entry.path().parent_path().string();

				if (isDuplicate)
				{
					if (fileInfo->parentFolder == "lib")
					{
						std::string newFolder = applicationRootPathStd + "\\mocks/" + entry.path().parent_path().filename().string();

						// Create new folder if it doesn't exist
						std::filesystem::create_directories(newFolder);

						std::unordered_set<std::string> processedHeaderFiles; // A set that tracks already processed .h files

						for (const auto& file : std::filesystem::directory_iterator(originalFolder))
						{
							// If the file is .c
							if (file.path().extension() == ".c")
							{
								std::string filePathString = file.path().string();

								// Call the function and get mocked functions and macro definitions
								auto [mockedFunctions, macroDefinitions] = mockingFiles.ProcessFileWithSeperateDefines(filePathString, inclSettings);

								// Save mocked functions
								CommonFunctions::SaveMockedFile(newFolder + "/" + file.path().filename().string(), filePathString, mockedFunctions);
								CommonFunctions::SaveMockedFile(CommonFunctions::toStandardString(fileInfo->originalPath) + "/" + file.path().filename().string(), filePathString, mockedFunctions);

								// Check if there is a matching .h file
								std::string headerFilePath = originalFolder + "\\" + file.path().stem().string() + ".h"; // Same name of .h and .c file
								std::string newHeaderFilePath = newFolder + "/" + file.path().stem().string() + ".h"; // Copied .h file
								std::string originalHeaderFilePath = CommonFunctions::toStandardString(fileInfo->originalPath) + "/" + file.path().stem().string() + ".h"; // Original path for .h

								if (std::filesystem::exists(headerFilePath))
								{
									// Copy the .h file to newFolder and originalPath
									std::filesystem::copy(headerFilePath, newHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
									std::filesystem::copy(headerFilePath, originalHeaderFilePath, std::filesystem::copy_options::overwrite_existing);

									// Insert the macro definitions into the copied .h file
									InsertMacrosIntoHeader(newHeaderFilePath, macroDefinitions);
									InsertMacrosIntoHeader(originalHeaderFilePath, macroDefinitions);

									// Add this file to the set as processed
									processedHeaderFiles.insert(headerFilePath);
								}
							}
							// If the file is .h
							else if (file.path().extension() == ".h")
							{
								std::string headerFilePath = file.path().string();
								std::string newHeaderFilePath = newFolder + "/" + file.path().filename().string();
								std::string originalHeaderFilePath = CommonFunctions::toStandardString(fileInfo->originalPath) + "/" + file.path().filename().string();

								// If this .h file has already been processed from the linked .c file, skip
								if (processedHeaderFiles.find(headerFilePath) != processedHeaderFiles.end())
								{
									continue; // It was already copied in the previous processing of the .c file
								}

								// Copy the .h file to both folders
								std::filesystem::copy(file.path(), newHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
								std::filesystem::copy(file.path(), originalHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
							}
						}
					}
					else if (fileInfo->parentFolder == "cfg")
					{
						std::unordered_set<std::string> processedHeaderFiles; // A set that tracks already processed .h files
						for (const auto& file : std::filesystem::directory_iterator(originalFolder))
						{
							std::string fileName = removeExtension(fileInfo->fileName);
							std::string currentFileName = file.path().stem().string(); // Filename without extension
							if (currentFileName == fileName && file.path().extension() == ".c")
							{
								std::string filePathString = file.path().string();

								// Call the function and get mocked functions and macro definitions
								auto [mockedFunctions, macroDefinitions] = mockingFiles.ProcessFileWithSeperateDefines(filePathString, inclSettings);
								CommonFunctions::SaveMockedFile(CommonFunctions::toStandardString(fileInfo->originalPath) + "/" + file.path().filename().string(), filePathString, mockedFunctions);

								// Check if there is a matching .h file
								std::string headerFilePath = originalFolder + "\\" + file.path().stem().string() + ".h";
								std::string originalHeaderFilePath = CommonFunctions::toStandardString(fileInfo->originalPath) + "/" + file.path().stem().string() + ".h"; // Original path for .h

								if (std::filesystem::exists(headerFilePath))
								{
									// Copy the .h file to newFolder and originalPath
									std::filesystem::copy(headerFilePath, originalHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
									InsertMacrosIntoHeader(headerFilePath, macroDefinitions);
									// Add this file to the set as processed
									processedHeaderFiles.insert(headerFilePath);
								}
							}// If the file is .h
							else if (file.path().extension() == ".h")
							{
								std::string fileName = removeExtension(fileInfo->fileName);
								std::string currentFileName = file.path().stem().string(); // filename without extension

								if (currentFileName == fileName && file.path().extension() == ".h")
								{
									std::string headerFilePath = file.path().string();
									std::string originalHeaderFilePath = CommonFunctions::toStandardString(fileInfo->originalPath) + "/" + file.path().filename().string();

									// If this .h file has already been processed from the linked .c file, skip
									if (processedHeaderFiles.find(headerFilePath) != processedHeaderFiles.end())
									{
										continue; // It was already copied in the previous processing of the .c file
									}

									// Copy the .h file to both folders
									std::filesystem::copy(file.path(), originalHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
								}
							}
						}

					}
					else
					{
						std::string newFolder = applicationRootPathStd + "\\mocks/" + entry.path().parent_path().filename().string();

						// Create new folder if it doesn't exist
						std::filesystem::create_directories(newFolder);

						std::unordered_set<std::string> processedHeaderFiles; // A set that tracks already processed .h files
						for (const auto& file : std::filesystem::directory_iterator(originalFolder))
						{
							// Mock .c file
							if (file.path().extension() == ".c")
							{

								std::string filePathString = file.path().string();

								// Call the function and get mocked functions and macro definitions
								auto [mockedFunctions, macroDefinitions] = mockingFiles.ProcessFileWithSeperateDefines(filePathString, inclSettings);
								CommonFunctions::SaveMockedFile(newFolder + "/" + file.path().filename().string(), filePathString, mockedFunctions);

								// Check if there is a matching .h file
								std::string headerFilePath = originalFolder + "\\" + file.path().stem().string() + ".h"; // Same name of .h and .c file
								std::string newHeaderFilePath = newFolder + "/" + file.path().stem().string() + ".h"; // Copied .h file
								if (std::filesystem::exists(headerFilePath))
								{
									// Copy the .h file to newFolder and originalPath
									std::filesystem::copy(headerFilePath, newHeaderFilePath, std::filesystem::copy_options::overwrite_existing);

									InsertMacrosIntoHeader(newHeaderFilePath, macroDefinitions);

									// Add this file to the set as processed
									processedHeaderFiles.insert(headerFilePath);
								}

							}// If the file is .h
							else if (file.path().extension() == ".h")
							{
								std::string headerFilePath = file.path().string();
								std::string newHeaderFilePath = newFolder + "/" + file.path().filename().string();

								// If this .h file has already been processed from the linked .c file, skip
								if (processedHeaderFiles.find(headerFilePath) != processedHeaderFiles.end())
								{
									continue; // It was already copied in the previous processing of the .c file
								}

								// Copy the .h file to both folders
								std::filesystem::copy(file.path(), newHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
							}
						}
					}

				}
				else
				{
					std::string newFolder = applicationRootPathStd + "\\mocks/" + entry.path().parent_path().filename().string();

					// Create new folder if it doesn't exist
					std::filesystem::create_directories(newFolder);

					std::unordered_set<std::string> processedHeaderFiles; // A set that tracks already processed .h files
					for (const auto& file : std::filesystem::directory_iterator(originalFolder))
					{
						// Mock .c file
						if (file.path().extension() == ".c")
						{

							std::string filePathString = file.path().string();

							// Call the function and get mocked functions and macro definitions
							auto [mockedFunctions, macroDefinitions] = mockingFiles.ProcessFileWithSeperateDefines(filePathString, inclSettings);
							CommonFunctions::SaveMockedFile(newFolder + "/" + file.path().filename().string(), filePathString, mockedFunctions);

							// Check if there is a matching .h file
							std::string headerFilePath = originalFolder + "\\" + file.path().stem().string() + ".h"; // Same name of .h and .c file
							std::string newHeaderFilePath = newFolder + "/" + file.path().stem().string() + ".h"; // Copied .h file
							if (std::filesystem::exists(headerFilePath))
							{
								// Copy the .h file to newFolder and originalPath
								std::filesystem::copy(headerFilePath, newHeaderFilePath, std::filesystem::copy_options::overwrite_existing);

								InsertMacrosIntoHeader(newHeaderFilePath, macroDefinitions);

								// Add this file to the set as processed
								processedHeaderFiles.insert(headerFilePath);
							}

						}// If the file is .h
						else if (file.path().extension() == ".h")
						{
							std::string headerFilePath = file.path().string();
							std::string newHeaderFilePath = newFolder + "/" + file.path().filename().string();

							// If this .h file has already been processed from the linked .c file, skip
							if (processedHeaderFiles.find(headerFilePath) != processedHeaderFiles.end())
							{
								continue; // It was already copied in the previous processing of the .c file
							}

							// Copy the .h file to both folders
							std::filesystem::copy(file.path(), newHeaderFilePath, std::filesystem::copy_options::overwrite_existing);
						}
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
System::Void FilesForm::checkedListBox_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
{
	overlay->mockingSettings_btn->Enabled = overlay->checkedListBox->SelectedIndex != -1;
}
std::string FilesForm::removeExtension(String^ fileNameWithExtension) {
	// Find the position of the last point
	int dotIndex = fileNameWithExtension->LastIndexOf('.');

	// If there is a dot, return the part of the string up to the dot
	if (dotIndex > 0) {
		return CommonFunctions::toStandardString(fileNameWithExtension->Substring(0, dotIndex));
	}

	// If there is no dot, return the entire string
	return CommonFunctions::toStandardString(fileNameWithExtension);
}
void FilesForm::InsertMacrosIntoHeader(const std::string& headerFilePath, const std::vector<std::string>& macroDefinitions) {
	std::ifstream headerFile(headerFilePath);
	std::string line;
	std::vector<std::string> fileLines;
	std::string globalDefinitionsMarker = "Global Definitions";
	bool globalDefinitionsFound = false;
	bool skipNextLine = false;  // Flag for skipping the dashed line

	// Load the lines of the file and store them in a vector
	while (std::getline(headerFile, line)) {
		fileLines.push_back(line);

		// Find the marker for "Global Definitions"
		if (line.find(globalDefinitionsMarker) != std::string::npos) {
			globalDefinitionsFound = true;
			skipNextLine = true;  // Prepare to skip next line (dashed line)
		}
		else if (skipNextLine) {
			skipNextLine = false;  // Stop skipping lines
		}
	}
	headerFile.close();

	std::ofstream headerFileOut(headerFilePath); // Open the file for writing
	bool macrosInserted = false; // Check if macros are already inserted

	if (headerFileOut.is_open()) {
		for (size_t i = 0; i < fileLines.size(); ++i) {
			headerFileOut << fileLines[i] << std::endl;  // Write all lines back to file

			// If we found a marker, insert the macros below it
			if (globalDefinitionsFound && fileLines[i].find(globalDefinitionsMarker) != std::string::npos && !macrosInserted) {
				// If there is a next line, insert it (dashed line)
				if (i + 1 < fileLines.size()) {
					headerFileOut << fileLines[i + 1] << std::endl;  // We write the dashed line
					++i;  // Skip the dotted line
				}

				// Insert macros
				for (const auto& macro : macroDefinitions) {
					headerFileOut << macro << std::endl;
				}
				macrosInserted = true;  // We mark that the macros are inserted
			}
		}

		// If the marker is not found, add the macros to the end of the file
		if (!macrosInserted) {
			headerFileOut << std::endl;
			headerFileOut << "// --------------------------------------------------------------------------------------------------------------------" << std::endl;
			headerFileOut << "//\tGlobal Definitions" << std::endl;
			headerFileOut << "// --------------------------------------------------------------------------------------------------------------------" << std::endl;

			for (const auto& macro : macroDefinitions) {
				headerFileOut << macro << std::endl;
			}
		}

		headerFileOut.close();
	}
}

