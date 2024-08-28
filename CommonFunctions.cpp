#include "CommonFunctions.h"
#include <windows.h>

std::string CommonFunctions::toStandardString(System::String^ string)
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
void CommonFunctions::SaveMockedFile(const std::string& mockedFilePath, const std::string& a_originalFilePath, const std::vector<std::string>& a_mockFunctions)
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
std::string CommonFunctions::GetExecutablePath()
{
	// Buffer to hold the path
	char buffer[MAX_PATH];
	// Retrieve the full path of the executable
	GetModuleFileNameA(NULL, buffer, MAX_PATH);
	// Find the last occurrence of a backslash or forward slash
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	// Extract and return the directory path
	return std::string(buffer).substr(0, pos);
}
std::vector<std::string> CommonFunctions::ConvertToStdVector(System::Collections::Generic::List<System::String^>^ list) {
	// Converts a managed List<System::String^> to a std::vector<std::string>.
	std::vector<std::string> vec;

	// Iterate through each managed string in the list.
	for each (System::String ^ str in list) {
		// Convert the managed System::String^ to a native C++ string (const char*) using StringToHGlobalAnsi.
		System::IntPtr ptr = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
		const char* chars = static_cast<const char*>(ptr.ToPointer());

		// Add the converted string to the std::vector.
		vec.push_back(std::string(chars));

		// Free the unmanaged memory allocated for the native string.
		System::Runtime::InteropServices::Marshal::FreeHGlobal(ptr);
	}
	return vec;
}
System::Collections::Generic::List<System::String^>^ CommonFunctions::ConvertToManagedList(const std::vector<std::string>& vec) {
	// Converts a std::vector<std::string> to a managed List<System::String^>.
	auto list = gcnew System::Collections::Generic::List<System::String^>();

	// Iterate through each std::string in the vector.
	for (const auto& str : vec) {
		// Convert each std::string to a managed System::String^ and add it to the managed list.
		list->Add(gcnew System::String(str.c_str()));
	}
	return list;
}

std::string CommonFunctions::ExtractFunctionNameSimple(const std::string& fullFunctionString)
{
	// Extracts the function name from a full function string representation.
	// Find the first tab character and move one position ahead.
	size_t start = fullFunctionString.find_first_of('\t') + 1;

	// Find the position of the opening parenthesis '('.
	size_t end = fullFunctionString.find('(');

	// Return the substring between the tab character and the opening parenthesis.
	return fullFunctionString.substr(start, end - start);
}
std::string CommonFunctions::ExtractParameterName(const std::string& param)
{
	// Find the last space in the string
		size_t lastSpace = param.find_last_of(' ');

	if (lastSpace != std::string::npos) {
		// Return the part of the string after the last space, which represents the parameter name
		return param.substr(lastSpace + 1);
	}
	else {
		// If there is no space, return the whole string (which should be the parameter name)
		return param;
	}
}

std::string CommonFunctions::ExtractParameterType(const std::string& param) {
	// Find the last space in the string, since the parameter name comes after it
	size_t lastSpace = param.find_last_of(' ');

	if (lastSpace != std::string::npos) {
		// Return the part of the string before the last space, which represents the parameter type
		return param.substr(0, lastSpace);
	}
	else {
		// If there is no space, it means there is no parameter name, return the whole string
		return param;
	}
}

