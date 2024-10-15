#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
ref class CommonFunctions
{
public:
	/**
	 * @brief Retrieves the path of the currently running executable.
	 *
	 * This function uses the WinAPI function GetModuleFileNameA to get the full path
	 * of the executable, then extracts the directory portion of the path.
	 *
	 * @return A string containing the full path to the executable.
	 */
	static std::string GetExecutablePath();
	// A function that casts String^ to std::string
	static std::string toStandardString(System::String^ string);
	// Saves the mocked functions to a new file.
	static void SaveMockedFile(const std::string& mockedFilePath, const std::string& a_originalFilePath, const std::vector<std::string>& a_mockFunctions);

	// Converts a managed List<System::String^> to a std::vector<std::string>.
	static std::vector<std::string> ConvertToStdVector(System::Collections::Generic::List<System::String^>^ list);

	// Converts a std::vector<std::string> to a managed List<System::String^>.
	static System::Collections::Generic::List<System::String^>^ ConvertToManagedList(const std::vector<std::string>& vec);

	// Extracts the function name from a full function string representation.
	static std::string ExtractFunctionNameSimple(const std::string& fullFunctionString);

	// Extracts the parameter name from a parameter string.
	static std::string ExtractParameterName(const std::string& param);

	// Extracts the parameter type from a parameter string.
	static std::string ExtractParameterType(const std::string& param);
	
};