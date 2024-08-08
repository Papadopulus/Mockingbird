#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
ref class CommonFunctions
{
public:
	// A function that casts String^ to std::string
	static std::string toStandardString(System::String^ string);
	// Saves the mocked functions to a new file.
	static void SaveMockedFile(const std::string& mockedFilePath, const std::string& a_originalFilePath, const std::vector<std::string>& a_mockFunctions);
	
};