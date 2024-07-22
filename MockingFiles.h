#pragma once
#include <string>
#include <vector>

/**
 * @brief The MockingFiles class is responsible for processing C source files
 *        and generating mocked C files using CMocka.
 */
class MockingFiles {
public:

	/**
	 * @brief Processes the given C source file using ctags (programming tool that
	 *		  generates an index file (or tag file)) and passes it to the following
	 *		  functions for processing.
	 * 
	 * @param a_filePath The path to the C source file.
	 */
	std::vector<std::string> ProcessFile(const std::string& a_filePath);
private:
	/**
	 * @brief Parses a ctags file to extract function signatures.
	 *
	 * @param a_tagsFilePath The path to the ctags file.
	 * @return A list of function signatures.
	 */
	std::vector<std::string> ParseTagsFile(const std::string& a_tagsFilePath);

	/**
	 * @brief Mocks non-static functions extracted from the ctags file.
	 *
	 * @param a_functions A list of function signatures.
	 * @return A list of mocked functions.
	 */
	std::vector<std::string> MockNonStaticFunctions(const std::vector<std::string>& a_functions);


	/**
	 * @brief Retrieves the path of the currently running executable.
	 * 
	 * This function uses the WinAPI function GetModuleFileNameA to get the full path
	 * of the executable, then extracts the directory portion of the path.
	 * 
	 * @return A string containing the full path to the executable.
	 */
	std::string GetExecutablePath();
};