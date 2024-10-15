#pragma once
#include <string>
#include <vector>
#include "IncludeModuleSettings.h"
#include <utility> // Za std::pair
/**
 * @brief The MockingFiles class is responsible for processing C source files
 *        and generating mocked C files using CMocka.
 */
public ref class MockingFiles {
public:
	/**
	 * @brief Processes the given C source file using ctags (programming tool that
	 *		  generates an index file (or tag file)) for the purpose of obtaining static elements
	 *	      and passes it to the following functions for processing.
	 *		 
	 * @param a_filePath The path to the C source file.
	 */
	std::vector<std::string> ProcessFileStatic(const std::string& a_filePath);


	/**
	 * @brief Processes the given C source file using ctags (programming tool that
	 *		  generates an index file (or tag file)) and passes it to the following
	 *		  functions for processing.
	 * 
	 * @param a_filePath The path to the C source file.
	 */
	std::vector<std::string> ProcessFile(const std::string& a_filePath, IncludeModuleSettings^ moduleSettings);

	/**
	 * @brief Processes the given C source file using ctags (programming tool that
	 *		  generates an index file (or tag file)) for the purpose of obtaining functions and macros from C source file 
	 *		  and passes it to the following functions for processing.	  
	 *
	 * @param a_filePath The path to the C source file.
	 */
	std::pair<std::vector<std::string>, std::vector<std::string>> ProcessFileWithSeperateDefines(const std::string& a_filePath, IncludeModuleSettings^ moduleSettings);

	/**
	 * @brief Parses a ctags file to extract function signatures.
	 *
	 * @param a_tagsFilePath The path to the ctags file.
	 * @return A list of function signatures.
	 */
	std::vector<std::string> ParseTagsFile(const std::string& a_tagsFilePath);

private:
	/**
	 * @brief Parses a ctags file to extract static elements.
	 *
	 * @param a_tagsFilePath The path to the ctags file.
	 * @return A list of static elements.
	 */
	std::vector<std::string> ParseTagsFileStatic(const std::string& a_tagsFilePath);

	/**
	 * @brief Mocks non-static functions extracted from the ctags file.
	 *
	 * @param a_functions A list of function signatures.
	 * @return A list of mocked functions.
	 */
	std::vector<std::string> MockNonStaticFunctions(const std::vector<std::string>& a_functions);

	/**
	 * @brief Mocks non-static functions extracted from the ctags file with separating macroes and functions.
	 *
	 * @param a_functions A list of function signatures.
	 * @return A pair list of mocked functions and list of macros.
	 */
	std::pair<std::vector<std::string>, std::vector<std::string>> MockNonStaticFunctionsWithSeparateDefines(const std::vector<std::string>& a_functions);

	/**
	 * @brief Generates a mock implementation for a given function based on its settings.
	 *
	 * This function takes the function settings, including its name, return type, and parameters,
	 * and generates a mock function that can be used for testing purposes. It handles the parameters
	 * marked as "Ptr" and "Length" and appropriately sets up the function body to mock expected behaviors.
	 *
	 * @param funcSettings A managed object containing settings for the function to be mocked.
	 * @return A string representing the generated mock function.
	*/
	std::string GenerateMockFunction(FunctionSettings^ funcSettings);
};