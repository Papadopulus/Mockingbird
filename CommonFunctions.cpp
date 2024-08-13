#include "CommonFunctions.h"

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

