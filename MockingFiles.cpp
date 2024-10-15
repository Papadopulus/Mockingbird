#include "MockingFiles.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <filesystem>
#include <Windows.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include "CommonFunctions.h"

std::vector<std::string> MockingFiles::ProcessFileStatic(const std::string& a_filePath)
{
	// Getting the path to the executable file
	std::string exePath = CommonFunctions::GetExecutablePath();
	std::string ctagsPath = exePath + "\\ctags\\ctags.exe";
	char tempPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tempPath);
	std::string tagsFilePath = std::filesystem::path(tempPath).parent_path().string() + "\\tags_" + std::filesystem::path(a_filePath).stem().string();

	// Creating a command to run ctags.exe
	std::string command = ctagsPath + " --c-kinds=+px --fields=+iaS --languages=C -o \"" + tagsFilePath + "\" \"" + a_filePath + "\"";

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
		std::vector<std::string> staticElements = ParseTagsFileStatic(tagsFilePath);
		// Remove temporary file tag
		std::filesystem::remove(tagsFilePath);
		return staticElements;
	}
	else
	{
		throw std::runtime_error("Tags file does not exist.");
	}
}
std::vector<std::string> MockingFiles::ParseTagsFileStatic(const std::string& a_tagsFilePath)
{
	std::vector<std::string> staticElements;
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
		std::regex re("\t(?![^()]*\\))");

		// Iterates through the parts of the line that do not match the regex from the beginning to the end of the line and -1 means 
		// that those parts of the line that do not match the regex should be returned
		std::sregex_token_iterator iter(line.begin(), line.end(), re, -1);
		std::sregex_token_iterator end;

		// Fills parts with all strings obtained from iter (in range from iter to end)
		std::vector<std::string> parts(iter, end);

		if (parts.size() < 4)
			continue;

		std::string functionName = parts[0];
		std::string fileName = parts[1];
		std::string exCommand = parts[2];
		std::string extra = parts[3];
		std::string functionParameters;

		// If the sub-string is not found it returns std::string::npos
		if (exCommand.find("static") != std::string::npos || exCommand.find("STATIC") != std::string::npos)
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
			if (extra == "f" || extra == "p")
			{
				if (extra == "p")
				{
					functionParameters = parts[5];
				}
				else
				{
					functionParameters = parts[4];
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
						if (part == "const" || part.find("STATIC") != std::string::npos)
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
					staticElements.push_back("extern " + returnType + " " + functionName + "(" + parameters + ");");
				}
			}
			else if (extra == "v")
			{
				size_t equalSignPos = exCommandParts1[1].find('=');
				if (equalSignPos != std::string::npos)
				{
					// If there is an assignment, we take the part before the '=' sign
					std::string variableDeclaration = exCommandParts1[1].substr(0, equalSignPos);
					// Remove any variation of "STATIC", such as "STATIC", "STATIC_INLINE", etc.
					variableDeclaration = std::regex_replace(variableDeclaration, std::regex("\\b\\w*STATIC\\w*\\b\\s*"), "");

					staticElements.push_back("extern " + variableDeclaration + ";"); // Add only the declaration
				}
				else
				{
					// If there is no assignment, we check for a $ character and remove everything after it
					std::string variableDeclaration = exCommandParts1[1];
					// Remove any variation of "STATIC", such as "STATIC", "LIBTUNMGR_STATIC", "STATIC_INLINE", etc.
					variableDeclaration = std::regex_replace(variableDeclaration, std::regex("\\b\\w*STATIC\\w*\\b\\s*"), "");

					size_t dollarSignPos = variableDeclaration.find('$');
					if (dollarSignPos != std::string::npos)
					{
						// Remove everything from the $ sign on
						variableDeclaration = variableDeclaration.substr(0, dollarSignPos);
					}
					// We remove any spaces/tabs at the end of the line
					variableDeclaration = std::regex_replace(variableDeclaration, std::regex("\\s+$"), "");

					staticElements.push_back("extern " + variableDeclaration);
				}
			}
		}
	}
	return staticElements;
}
std::vector<std::string> MockingFiles::ProcessFile(const std::string& a_filePath, IncludeModuleSettings^ moduleSettings)
{
	// Getting the path to the executable file
	std::string exePath = CommonFunctions::GetExecutablePath();
	// Relative path to ctags.exe
	std::string ctagsPath = exePath + "\\ctags\\ctags.exe";
	// Get the path to the temporary folder
	char tempPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tempPath);
	// Relative path to the output tags file
	std::string tagsFilePath = std::filesystem::path(tempPath).parent_path().string() + "\\tags_" + std::filesystem::path(a_filePath).stem().string();
	// Creating a command to run ctags.exe
	std::string command = ctagsPath + " --c-kinds=+px --fields=+iaS --languages=C -o \"" + tagsFilePath + "\" \"" + a_filePath + "\"";

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

		std::vector<std::string> functions = ParseTagsFile(tagsFilePath);
		std::vector<std::string> mockFunctions;
		if (moduleSettings != nullptr) {
			std::vector<std::string> mockNormalFunctions;
			std::vector<std::string> mockFunctionsWithSettings;
			

			for (const auto& function : functions) {
				std::string functionName = CommonFunctions::ExtractFunctionNameSimple(function);
				bool isSelected = false;
				for each (FunctionSettings ^ funcSettings in  moduleSettings->functions) {
					if (CommonFunctions::toStandardString(funcSettings->functionName) == functionName) {
						isSelected = true;
						mockFunctionsWithSettings.push_back(GenerateMockFunction(funcSettings));
						break;
					}
				}

				if (!isSelected) {
					mockNormalFunctions.push_back(function);
				}
			}
			std::vector<std::string> generatedNormalFunctions = MockNonStaticFunctions(mockNormalFunctions);
			mockFunctions.insert(mockFunctions.end(), generatedNormalFunctions.begin(), generatedNormalFunctions.end());

			mockFunctions.insert(mockFunctions.end(), mockFunctionsWithSettings.begin(), mockFunctionsWithSettings.end());
		}
		else
		{
			mockFunctions = MockNonStaticFunctions(functions);
		}
		
		// Delete the temporary tags file
		std::filesystem::remove(tagsFilePath);
		return mockFunctions;
	}
	else
	{
		throw std::runtime_error("Tags file does not exist.");
	}
}
std::pair<std::vector<std::string>, std::vector<std::string>> MockingFiles::ProcessFileWithSeperateDefines(const std::string& a_filePath, IncludeModuleSettings^ moduleSettings)
{
	// Getting the path to the executable file
	std::string exePath = CommonFunctions::GetExecutablePath();
	std::string ctagsPath = exePath + "\\ctags\\ctags.exe";
	char tempPath[MAX_PATH];
	GetTempPathA(MAX_PATH, tempPath);
	std::string tagsFilePath = std::filesystem::path(tempPath).parent_path().string() + "\\tags_" + std::filesystem::path(a_filePath).stem().string();

	// Creating a command to run ctags.exe
	std::string command = ctagsPath + " --c-kinds=+px --fields=+iaS --languages=C -o \"" + tagsFilePath + "\" \"" + a_filePath + "\"";

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
		std::vector<std::string> functions = ParseTagsFile(tagsFilePath);
		std::vector<std::string> mockFunctions;
		std::vector<std::string> mockMacros;  // Vector for macros

		if (moduleSettings != nullptr) {
			std::vector<std::string> mockNormalFunctions;
			std::vector<std::string> mockFunctionsWithSettings;

			for (const auto& function : functions) {
				std::string functionName = CommonFunctions::ExtractFunctionNameSimple(function);
				bool isSelected = false;
				for each (FunctionSettings ^ funcSettings in moduleSettings->functions) {
					if (CommonFunctions::toStandardString(funcSettings->functionName) == functionName) {
						isSelected = true;
						mockFunctionsWithSettings.push_back(GenerateMockFunction(funcSettings));
						break;
					}
				}

				if (!isSelected) {
					mockNormalFunctions.push_back(function);
				}
			}

			// Call the MockNonStaticFunctions function that returns two vectors
			auto [generatedNormalFunctions, generatedMacros] = MockNonStaticFunctionsWithSeparateDefines(mockNormalFunctions);
			mockFunctions.insert(mockFunctions.end(), generatedNormalFunctions.begin(), generatedNormalFunctions.end());
			mockMacros.insert(mockMacros.end(), generatedMacros.begin(), generatedMacros.end());

			// Insert mock functions with settings
			mockFunctions.insert(mockFunctions.end(), mockFunctionsWithSettings.begin(), mockFunctionsWithSettings.end());
		}
		else
		{
			// If there are no settings, we call without additional filtering
			auto [generatedFunctions, generatedMacros] = MockNonStaticFunctionsWithSeparateDefines(functions);
			mockFunctions.insert(mockFunctions.end(), generatedFunctions.begin(), generatedFunctions.end());
			mockMacros.insert(mockMacros.end(), generatedMacros.begin(), generatedMacros.end());
		}

		// Remove temporary file tag
		std::filesystem::remove(tagsFilePath);

		// Return both vectors: mock functions and macros
		return std::make_pair(mockFunctions, mockMacros);
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

		// It reads the function piece by piece, each time it encounters a tab. Each obtained part is added to the functionParts vector
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

		if (!(paramList.size() == 1 && paramList[0].find("void") != std::string::npos))
		{
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

		// Append mock function to the list
		mockFunctions.push_back(mockFunction.str());

		if (!(paramList.size() == 1 && paramList[0].find("void") != std::string::npos))
		{
			// Generate macro
			std::ostringstream macro;
			macro << "#define MOCK_" << functionName << "(";

			for (size_t i = 0; i < paramList.size(); ++i)
			{
				std::vector<std::string> paramNameParts;
				std::istringstream iss4(paramList[i]);
				std::string paramName;
				while (iss4 >> paramName)
				{
					paramNameParts.push_back(paramName);
				}
				paramName = paramNameParts.back();
				if (paramName.find('*') != std::string::npos)
				{
					paramName.erase(std::remove(paramName.begin(), paramName.end(), '*'), paramName.end());
				}
				std::string macroParam = "a_" + paramName;
				macro << macroParam;
				if (i < paramList.size() - 1)
				{
					macro << ", ";
				}
			}

			if (returnType != "void")
			{
				macro << ", a_ret";
			}

			macro << ")\\\n";

			for (size_t i = 0; i < paramList.size(); ++i)
			{
				std::vector<std::string> paramNameParts;
				std::istringstream iss4(paramList[i]);
				std::string paramName;
				while (iss4 >> paramName)
				{
					paramNameParts.push_back(paramName);
				}
				std::string actualParam = paramNameParts.back();
				if (actualParam.find('*') != std::string::npos)
				{
					actualParam.erase(std::remove(actualParam.begin(), actualParam.end(), '*'), actualParam.end());
				}
				if (i == paramList.size() - 1 && returnType == "void")
				{
					macro << "\texpect_value(" << functionName << ", " << actualParam << ", a_" << actualParam << ")\\\n";
				}
				else
				{
					macro << "\texpect_value(" << functionName << ", " << actualParam << ", a_" << actualParam << ");\\\n";
				}

			}

			if (returnType != "void")
			{
				macro << "\twill_return(" << functionName << ", a_ret)\\\n";
			}
			else {

			}

			macro << "\n";

			// Append macro to the list
			mockFunctions.push_back(macro.str());
		}

	}
	return mockFunctions;
}
std::pair<std::vector<std::string>, std::vector<std::string>> MockingFiles::MockNonStaticFunctionsWithSeparateDefines(const std::vector<std::string>& a_functions)
{
	std::vector<std::string> mockFunctions;
	std::vector<std::string> macroDefinitions;

	// Step through each function in the function list
	for (const auto& function : a_functions)
	{
		std::vector<std::string> functionParts;
		std::istringstream iss(function);
		std::string part;

		// Split function by tab
		while (std::getline(iss, part, '\t'))
		{
			functionParts.push_back(part);
		}

		std::string returnType = functionParts[0];
		std::vector<std::string> restParts;
		std::istringstream iss2(functionParts[1]);

		// Split function name and parameters
		while (std::getline(iss2, part, '('))
		{
			restParts.push_back(part);
		}

		std::string functionName = restParts[0];
		std::string parameters = restParts[1];

		std::vector<std::string> paramList;
		std::istringstream iss3(parameters);

		// Split parameters by comma
		while (std::getline(iss3, part, ','))
		{
			paramList.push_back(part);
		}

		std::ostringstream mockFunction;
		mockFunction << returnType << " " << functionName << "(";

		for (size_t i = 0; i < paramList.size(); ++i)
		{
			mockFunction << paramList[i];
			if (i < paramList.size() - 1)
			{
				mockFunction << ", ";
			}
		}

		mockFunction << ")\n{\n";

		if (!(paramList.size() == 1 && paramList[0].find("void") != std::string::npos))
		{
			for (const auto& param : paramList)
			{
				std::vector<std::string> paramNameParts;
				std::istringstream iss4(param);
				std::string paramName;

				while (iss4 >> paramName)
				{
					paramNameParts.push_back(paramName);
				}

				paramName = paramNameParts.back();

				if (param.find('*') != std::string::npos)
				{
					paramName.erase(std::remove(paramName.begin(), paramName.end(), '*'), paramName.end());
					mockFunction << "\tcheck_expected_ptr(" << paramName << ");\n";
				}
				else
				{
					mockFunction << "\tcheck_expected(" << paramName << ");\n";
				}
			}
		}

		if (returnType != "void")
		{
			if (returnType != "void*" && returnType.find('*') != std::string::npos)
			{
				std::string cleanReturnType = returnType;
				cleanReturnType.erase(std::remove(cleanReturnType.begin(), cleanReturnType.end(), '*'), cleanReturnType.end());
				mockFunction << "\treturn mock_type_ptr(" << returnType << ");\n";
			}
			else
			{
				mockFunction << "\treturn (" << returnType << ")mock();\n";
			}
		}

		mockFunction << "}\n";

		// Append mock function to the list
		mockFunctions.push_back(mockFunction.str());

		// Create macro definition if there are parameters
		if (!(paramList.size() == 1 && paramList[0].find("void") != std::string::npos))
		{
			std::ostringstream macro;
			macro << "#define MOCK_" << functionName << "(";

			for (size_t i = 0; i < paramList.size(); ++i)
			{
				std::vector<std::string> paramNameParts;
				std::istringstream iss4(paramList[i]);
				std::string paramName;

				while (iss4 >> paramName)
				{
					paramNameParts.push_back(paramName);
				}

				paramName = paramNameParts.back();

				if (paramName.find('*') != std::string::npos)
				{
					paramName.erase(std::remove(paramName.begin(), paramName.end(), '*'), paramName.end());
				}

				std::string macroParam = "a_" + paramName;
				macro << macroParam;

				if (i < paramList.size() - 1)
				{
					macro << ", ";
				}
			}

			if (returnType != "void")
			{
				macro << ", a_ret";
			}

			macro << ")\\\n";

			for (size_t i = 0; i < paramList.size(); ++i)
			{
				std::vector<std::string> paramNameParts;
				std::istringstream iss4(paramList[i]);
				std::string paramName;

				while (iss4 >> paramName)
				{
					paramNameParts.push_back(paramName);
				}

				std::string actualParam = paramNameParts.back();

				if (actualParam.find('*') != std::string::npos)
				{
					actualParam.erase(std::remove(actualParam.begin(), actualParam.end(), '*'), actualParam.end());
				}

				macro << "\texpect_value(" << functionName << ", " << actualParam << ", a_" << actualParam << ");\\\n";
			}

			if (returnType != "void")
			{
				macro << "\twill_return(" << functionName << ", a_ret)\\\n";
			}

			// Append macro to the list
			macroDefinitions.push_back(macro.str());
		}
	}

	return std::make_pair(mockFunctions, macroDefinitions);
}

std::string MockingFiles::GenerateMockFunction(FunctionSettings^ funcSettings) {
	std::string functionName = CommonFunctions::toStandardString(funcSettings->functionName);
	std::string returnType = CommonFunctions::toStandardString(funcSettings->returnType);
	std::ostringstream mockFunction;

	// Generating the function signature.
	mockFunction << returnType << " " << functionName << "(";

	for (int i = 0; i < funcSettings->parameters->Count; i++) {
		ParameterSettings^ param = funcSettings->parameters[i];
		std::string paramName = CommonFunctions::CommonFunctions::toStandardString(param->paramName);

		// If it is not the first parameter, add a comma.
		if (i > 0) {
			mockFunction << ", ";
		}

		mockFunction << paramName;
	}

	mockFunction << ")\n{\n";

	// First, iterate through all parameters to identify Ptr and Length.
	std::string ptrParam;
	std::string lengthParam;
	std::string ptrParamType;
	std::string lengthParamType;

	// Then, generate the function body.
	for (int i = 0; i < funcSettings->parameters->Count; i++) 
	{
		ParameterSettings^ param = funcSettings->parameters[i];
		std::string paramName = CommonFunctions::ExtractParameterName(CommonFunctions::toStandardString(param->paramName));
		std::string paramType = CommonFunctions::ExtractParameterType(CommonFunctions::toStandardString(param->paramName)); // Extract the parameter type.
		std::string setting = CommonFunctions::toStandardString(param->setting);

		size_t starPos = paramName.find('*');
		if (starPos != std::string::npos) {
			paramName = paramName.substr(starPos + 1); // Removing the pointer from the beginning of the name.
			paramType += "*";
		}

		if (setting == "None") {
			mockFunction << "    check_expected(" << paramName << ");\n";
		}

		if (setting == "Ptr") {
			ptrParam = paramName;
			ptrParamType = paramType;
		}
		else if (setting == "Length") {
			lengthParam = paramName;
			lengthParamType = paramType;
		}
	}
	if (!ptrParam.empty() && !lengthParam.empty()) 
	{
		mockFunction << "    " << ptrParamType << " cpy_ptr = mock_ptr_type(" << ptrParamType << ");\n";
		mockFunction << "   " << lengthParamType << " cpy_length = (" << lengthParamType << ")mock();\n";
		mockFunction << "    if(cpy_ptr != NULL && cpy_length > 0U)\n";
		mockFunction << "    {\n";
		mockFunction << "        memcpy(" << ptrParam << ", cpy_ptr, cpy_length);\n";
		mockFunction << "    }\n";
	}
	// Adding the return value.
	mockFunction << "    return (" << returnType << ")mock();\n";
	mockFunction << "}\n";

	return mockFunction.str();
}


