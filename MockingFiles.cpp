#include "MockingFiles.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <filesystem>
#include <Windows.h>

std::vector<std::string> MockingFiles::ProcessFile(const std::string& a_filePath)
{
	// Getting the path to the executable file
	std::string exePath = GetExecutablePath();
	// Relative path to ctags.exe
	std::string ctagsPath = exePath + "\\ctags\\ctags.exe";
	// Get the path to the temporary folder
	char tempPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tempPath);
	// Relative path to the output tags file
	std::string tagsFilePath = std::filesystem::path(tempPath).parent_path().string() + "\\tags_" + std::filesystem::path(a_filePath).stem().string();
	// Creating a command to run ctags.exe
	std::string command = ctagsPath + " --c-kinds=+px --fields=+iaS --languages=C -o \"" + tagsFilePath + "\" \"" + a_filePath + "\"";

	// Starting the ctags process
	system(command.c_str());
	// Checking the existence of the tags file and continuing processing
	if (std::filesystem::exists(tagsFilePath))
	{
		std::vector<std::string> functions = ParseTagsFile(tagsFilePath);
		std::vector<std::string> mockFunctions = MockNonStaticFunctions(functions);
		// Delete the temporary tags file
		std::filesystem::remove(tagsFilePath);
		return mockFunctions;

	}
	else
	{
		throw std::runtime_error("Tags file does not exist.");
	}
}

std::vector<std::string> MockingFiles::ParseTagsFile(const std::string& a_tagsFilePath)
{
	std::vector<std::string> functions;
	// Opens a file for reading
	std::ifstream file(a_tagsFilePath);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open tags file.");
	}
	std::string line;

	// Reading the contents of a file line by line
	while (std::getline(file, line))
	{
		if (line[0] == '!')
			continue;
		// Regex looks for a tab but does not take tabs that are between ( and ;
		std::regex re("\t(?![^\(]*\;)");

		// Iterates through the parts of the line that do not match the regex from the beginning to the end of the line and -1 means 
		// that those parts of the line that do not match the regex should be returned
		std::sregex_token_iterator iter(line.begin(), line.end(), re, -1);
		std::sregex_token_iterator end;

		// Fills parts with all strings obtained from iter (in range from iter to end)
		std::vector<std::string> parts(iter, end);

		if (parts.size() < 5)
			continue;

		std::string functionName = parts[0];
		std::string fileName = parts[1];
		std::string exCommand = parts[2];
		std::string extra = parts[3];
		std::string functionParameters = parts[4];

		// If the sub-string is not found it returns std::string::npos
		if (exCommand.find("static") != std::string::npos || exCommand.find("STATIC") != std::string::npos)
			continue;
		if (extra == "f")
		{
			std::vector<std::string> exCommandParts1;
			// Allows the exCommand string to be read as an input stream
			std::istringstream iss(exCommand);
			std::string token;

			// Reads the exCommand string part by part, each time it encounters a ^ character. Each received part (token) is added to the exCommandParts1 vector
			while (std::getline(iss, token, '^'))
			{
				exCommandParts1.push_back(token);
			}
			std::vector<std::string> exCommandParts2;
			// It uses the second part of exCommandParts1 (i.e. exCommandParts1[1]) as the input stream
			std::istringstream iss2(exCommandParts1[1]);

			// Reads the exCommandParts1[1] string part by part, separating it by spaces. Each part (token) is added to the exCommandParts2 vector.
			while (iss2 >> token)
			{
				exCommandParts2.push_back(token);
			}
			std::string returnType = "";
			// The first non-empty part (token) is considered the return type of the function and is assigned to the returnType variable.
			for (const auto& part : exCommandParts2)
			{
				if (!part.empty())
				{
					if (part == "const") 
						continue;

					returnType = part;
					break;
				}
			}

			size_t paramStart = functionParameters.find('(');
			size_t paramEnd = functionParameters.rfind(')');
			if (paramStart != std::string::npos && paramEnd != std::string::npos)
			{
				// Extracts the function parameters as a substring and appends the entire function to the array
				std::string parameters = functionParameters.substr(paramStart + 1, paramEnd - paramStart - 1);
				functions.push_back(returnType + "\t" + functionName + "(" + parameters);
			}
		}
	}
	return functions;
}

std::vector<std::string> MockingFiles::MockNonStaticFunctions(const std::vector<std::string>& a_functions)
{
	std::vector<std::string> mockFunctions;
	// Step through each function in the function list
	for (const auto& function : a_functions)
	{
		std::vector<std::string> functionParts;
		// Allows a function to be read as an input stream
		std::istringstream iss(function);
		std::string part;

		// It reads the function piece by piece, each time it encounters a tab. Each obtained part is added to the functionParts vector.
		while (std::getline(iss, part, '\t'))
		{
			functionParts.push_back(part);
		}
		// Extracts the return type of a function
		std::string returnType = functionParts[0];
		std::vector<std::string> restParts;
		// Allows functionParts[1](function name and parameters) to be read as an input stream
		std::istringstream iss2(functionParts[1]);
		// Splits functionParts[1] into function name and parameters
		while (std::getline(iss2, part, '('))
		{
			restParts.push_back(part);
		}
		std::string functionName = restParts[0];
		std::string parameters = restParts[1];

		std::vector<std::string> paramList;
		// Allows parameters to be read as an input stream
		std::istringstream iss3(parameters);
		// Splits parameters into parameters by inserting a comma-separated string into the paramList
		while (std::getline(iss3, part, ','))
		{
			paramList.push_back(part);
		}

		std::ostringstream mockFunction;
		// An output stream is created and the following is written into it
		mockFunction << returnType << " " << functionName << "(";
		// Parameters of the function are entered in output stream
		for (size_t i = 0; i < paramList.size(); ++i)
		{
			mockFunction << paramList[i];
			if (i < paramList.size() - 1)
			{
				mockFunction << ", ";
			}
		}
		mockFunction << ")\n{\n";

		// Each parameter is passed through and a corresponding mock function is created for it
		for (const auto& param : paramList)
		{
			std::vector<std::string> paramNameParts;
			// Allows param to be read as an input stream
			std::istringstream iss4(param);
			std::string paramName;
			// The param is split into each ' ' (space character) and inserted into paramNameParts
			while (iss4 >> paramName)
			{
				paramNameParts.push_back(paramName);
			}
			// Takes the last element from the paramNameParts vector and assigns it to the paramName variable, which represents the name of the function
			paramName = paramNameParts.back();

			if (param.find('*') != std::string::npos)
			{
				// If there is a * in the name, it must be deleted before adding it to the mock function
				if (paramName.find('*') != std::string::npos)
				{
					// Deleting the * from the function name to add it to the mock function
					// Remove moves all * to the end of paramName while erase deletes all * that were moved to the end of paramName
					paramName.erase(std::remove(paramName.begin(), paramName.end(), '*'), paramName.end());
					mockFunction << "\tcheck_expected_ptr(" << paramName << ");\n";
				}
				// In case there is a * but not in the function name
				else
				{
					mockFunction << "\tcheck_expected_ptr(" << paramName << ");\n";
				}
			}
			else
			{
				mockFunction << "\tcheck_expected(" << paramName << ");\n";
			}
		}

		if (returnType != "void")
		{
			// If different from void and from void*
			if (returnType != "void*" && returnType.find('*') != std::string::npos)
			{
				std::string cleanReturnType = returnType;
				// Deleting * from return type to add to mock function
				cleanReturnType.erase(std::remove(cleanReturnType.begin(), cleanReturnType.end(), '*'), cleanReturnType.end());
				mockFunction << "\treturn mock_type_ptr(" << returnType << ");\n";
			}
			else
			{
				mockFunction << "\treturn (" << returnType << ")mock();\n";
			}
		}
		mockFunction << "}\n";

		mockFunctions.push_back(mockFunction.str());

	}
	return mockFunctions;
}

std::string MockingFiles::GetExecutablePath()
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
