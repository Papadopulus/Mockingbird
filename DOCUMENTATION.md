# Mockingbird - Detailed Documentation

## Table of Contents
1. [Introduction to Mocking](#introduction-to-mocking)
2. [Mocking in C/C++](#mocking-in-cc)
3. [CMocka Framework](#cmocka-framework)
4. [Mockingbird Architecture](#mockingbird-architecture)
5. [Component Analysis](#component-analysis)
6. [Usage Examples](#usage-examples)

---

## Introduction to Mocking

### What is Mocking?

**Mocking** is a software testing technique where you create **fake implementations** of functions or objects instead of real ones. This enables:

- **Isolation of code under test** - test only one function without dependencies
- **Behavior control** - define what the "fake" function will return
- **Call verification** - verify that a function was called with expected parameters
- **Error simulation** - test how your code reacts to errors

### Why is Mocking Important?

Imagine you have a function that reads data from a database:

```c
// Original function
int get_user_age(int user_id) {
    // Connects to database
    // Executes SQL query
    // Returns user's age
}
```

Without mocking, every test would:
- Need to connect to a real database
- Be slow
- Fail if database is unavailable
- Potentially modify data in the database

With mocking, you can:
- Simulate database response without real connection
- Test quickly and reliably
- Test edge cases (e.g., what if user doesn't exist?)

---

## Mocking in C/C++

### Basic Concepts

In C (which Mockingbird supports), mocking is done by:

1. **Creating a function with the same signature** as the original
2. **Controlling the return value** instead of executing real logic
3. **Verifying input parameters** to ensure the function is called correctly

### Manual Mocking Example

Original function:
```c
// hardware.h
int read_sensor(int sensor_id);
```

Original implementation:
```c
// hardware.c
int read_sensor(int sensor_id) {
    // Reads actual value from hardware sensor
    return actual_hardware_read(sensor_id);
}
```

Mocked version for testing:
```c
// hardware_mock.c
int read_sensor(int sensor_id) {
    // Instead of reading real value, returns predefined one
    return 25;  // Simulated temperature
}
```

### Problems with Manual Mocking

Writing mock functions manually is:
- **Tedious** - you must write a mock version for each function
- **Error-prone** - easy to make mistakes in signatures
- **Hard to maintain** - if original function changes, you must update the mock

**Mockingbird solves this problem by automatically generating mock functions!**

---

## CMocka Framework

### What is CMocka?

**CMocka** is a C testing framework that provides:
- Parameter verification functions (`check_expected`, `check_expected_ptr`)
- Return value functions (`mock()`, `mock_type_ptr()`)
- Expected value macros (`expect_value`, `will_return`)

### CMocka API - Explanation

#### 1. `check_expected(param)` / `check_expected_ptr(param)`
Verifies that the parameter passed to function equals the expected value.

```c
void my_mock_function(int value) {
    check_expected(value);  // Verifies 'value' is as expected
}

// In test:
expect_value(my_mock_function, value, 42);  // We expect 'value' to be 42
my_mock_function(42);  // Test passes
my_mock_function(10);  // Test fails!
```

#### 2. `mock()` / `mock_ptr_type(type)`
Returns a predefined value from the mock function.

```c
int get_data(void) {
    return (int)mock();  // Returns value defined with will_return
}

// In test:
will_return(get_data, 100);  // When get_data is called, it will return 100
int result = get_data();  // result = 100
```

#### 3. `expect_value(func, param, value)`
Defines expected parameter value.

```c
expect_value(read_sensor, sensor_id, 5);  // We expect sensor_id = 5
```

#### 4. `will_return(func, value)`
Defines what the mock function will return.

```c
will_return(read_sensor, 25);  // read_sensor will return 25
```

### Complete CMocka Example

```c
// ===== mock_sensor.c =====
#include <cmocka.h>

// Mock function
int read_sensor(int sensor_id) {
    check_expected(sensor_id);      // Verify parameter
    return (int)mock();             // Return predefined value
}

// ===== test_temperature.c =====
#include <cmocka.h>

// Function we're testing
int get_average_temperature(void) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += read_sensor(i);
    }
    return sum / 3;
}

// Test
static void test_average_temperature(void **state) {
    // Expect three calls with sensor_id = 0, 1, 2
    expect_value(read_sensor, sensor_id, 0);
    will_return(read_sensor, 20);
    
    expect_value(read_sensor, sensor_id, 1);
    will_return(read_sensor, 25);
    
    expect_value(read_sensor, sensor_id, 2);
    will_return(read_sensor, 30);
    
    // Call function under test
    int avg = get_average_temperature();
    
    // Verify result
    assert_int_equal(avg, 25);  // (20 + 25 + 30) / 3 = 25
}
```

---

## Mockingbird Architecture

### System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                    MOCKINGBIRD APPLICATION                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│  ┌───────────────┐    ┌──────────────┐    ┌─────────────────┐   │
│  │   MainForm    │───▶│   FileForm   │    │   FilesForm     │   │
│  │ (Navigation)  │    │ (Single file)│    │(Multiple files) │   │
│  └───────────────┘    └──────────────┘    └─────────────────┘   │
│          │                   │                    │              │
│          │                   ▼                    ▼              │
│          │           ┌──────────────────────────────────┐        │
│          │           │         MockingFiles             │        │
│          │           │    (Mock File Generation)        │        │
│          │           └──────────────────────────────────┘        │
│          │                         │                             │
│          │                         ▼                             │
│          │           ┌──────────────────────────────────┐        │
│          │           │           ctags.exe              │        │
│          │           │   (C Function Parsing)           │        │
│          │           └──────────────────────────────────┘        │
│          │                         │                             │
│          ▼                         ▼                             │
│  ┌───────────────┐    ┌──────────────────────────────────┐       │
│  │ CommonFunctions│    │     Generated Mock Files        │       │
│  │ (Helper funcs) │    │  (.c files with CMocka code)    │       │
│  └───────────────┘    └──────────────────────────────────┘       │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

### Main Classes

| Class | Responsibility |
|-------|----------------|
| `MainForm` | Main application window, navigation between forms |
| `FileForm` | Mock a single C file |
| `FilesForm` | Mock all project dependencies |
| `MockingFiles` | Core logic for generating mock functions |
| `CommonFunctions` | Utility functions (string conversion, file saving) |
| `IncludeModuleSettings` | Configuration for advanced mock settings |

---

## Component Analysis

### 1. MockingFiles Class

This is the **heart of the application**. This class:
1. Calls `ctags.exe` to parse C files
2. Analyzes ctags output to extract functions
3. Generates CMocka mock functions

#### Method: `ProcessFile`

```cpp
std::vector<std::string> MockingFiles::ProcessFile(
    const std::string& a_filePath, 
    IncludeModuleSettings^ moduleSettings)
```

**What it does:**
1. Builds command for ctags:
   ```
   ctags.exe --c-kinds=+px --fields=+iaS --languages=C -o tags_file source.c
   ```
2. Runs ctags as a process
3. Parses generated tags file
4. For each function generates mock version

**Example input and output:**

Input file `sensor.c`:
```c
int read_temperature(int sensor_id) {
    return hardware_read(sensor_id);
}

void set_threshold(int value, float* buffer) {
    *buffer = value * 1.5;
}
```

Generated mock file:
```c
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include "cmocka.h"
#include "sensor.h"

// Mocked functions
int read_temperature(int sensor_id)
{
    check_expected(sensor_id);
    return (int)mock();
}

#define MOCK_read_temperature(a_sensor_id, a_ret)\
    expect_value(read_temperature, sensor_id, a_sensor_id);\
    will_return(read_temperature, a_ret)\

void set_threshold(int value, float* buffer)
{
    check_expected(value);
    check_expected_ptr(buffer);
}

#define MOCK_set_threshold(a_value, a_buffer)\
    expect_value(set_threshold, value, a_value);\
    expect_value(set_threshold, buffer, a_buffer)\

```

#### Method: `MockNonStaticFunctions`

```cpp
std::vector<std::string> MockingFiles::MockNonStaticFunctions(
    const std::vector<std::string>& a_functions)
```

**Step by step explanation:**

1. **Parses each function:**
   ```cpp
   // Input: "int\tread_sensor(int sensor_id"
   // Output: returnType = "int", functionName = "read_sensor", parameters = "int sensor_id"
   ```

2. **Generates mock function signature:**
   ```cpp
   mockFunction << returnType << " " << functionName << "(";
   // Adds parameters...
   mockFunction << ")\n{\n";
   ```

3. **For each parameter adds verification:**
   ```cpp
   if (param.find('*') != std::string::npos) {
       // Pointer parameter
       mockFunction << "\tcheck_expected_ptr(" << paramName << ");\n";
   } else {
       // Regular parameter
       mockFunction << "\tcheck_expected(" << paramName << ");\n";
   }
   ```

4. **Adds return value:**
   ```cpp
   if (returnType != "void") {
       if (returnType.find('*') != std::string::npos) {
           // Pointer return type
           mockFunction << "\treturn mock_ptr_type(" << returnType << ");\n";
       } else {
           mockFunction << "\treturn (" << returnType << ")mock();\n";
       }
   }
   ```

5. **Generates macro for easier use:**
   ```cpp
   #define MOCK_function(a_param1, a_param2, a_ret)\
       expect_value(function, param1, a_param1);\
       expect_value(function, param2, a_param2);\
       will_return(function, a_ret)\
   ```

### 2. Parsing with ctags

#### What is ctags?

`ctags` is a tool that generates a "tags" file - an index of all symbols (functions, variables, types) in source code.

#### Example ctags output

For file:
```c
int calculate_sum(int a, int b) {
    return a + b;
}

static void helper_function(void) {
    // ...
}
```

ctags generates:
```
calculate_sum   source.c   /^int calculate_sum(int a, int b)$/;    f   signature:(int a, int b)
helper_function source.c   /^static void helper_function(void)$/;  f   signature:(void)
```

#### How Mockingbird Parses This Output

```cpp
std::vector<std::string> MockingFiles::ParseTagsFile(const std::string& a_tagsFilePath)
{
    // 1. Reads file line by line
    while (std::getline(file, line)) {
        // 2. Skips comments (lines starting with !)
        if (line[0] == '!')
            continue;
        
        // 3. Splits line parts by tab character
        std::regex re("\t(?![^\(]*\;)");
        std::sregex_token_iterator iter(line.begin(), line.end(), re, -1);
        
        // 4. Extracts information
        std::string functionName = parts[0];     // Function name
        std::string fileName = parts[1];         // File name
        std::string exCommand = parts[2];        // Function signature
        std::string extra = parts[3];            // Type (f = function)
        std::string functionParameters = parts[4]; // Parameters
        
        // 5. Skips static functions
        if (exCommand.find("static") != std::string::npos)
            continue;
        
        // 6. Extracts return type and builds string for mock
        functions.push_back(returnType + "\t" + functionName + "(" + parameters);
    }
}
```

### 3. Handling Static Functions

Mockingbird has special functionality for static functions:

#### The Problem
Static functions are only visible within the file where they're defined. To test them, you must:
1. Remove the `static` keyword
2. Or use a macro trick

#### Mockingbird Solution

The application can insert the following into source code:

```c
#ifdef UNIT_TESTING
#define STATIC
#define STATIC_INLINE
#else
#define STATIC static
#define STATIC_INLINE static inline
#endif
```

Then replaces `static` with `STATIC`:

```c
// Before processing
static int helper(int x) { return x * 2; }

// After processing
STATIC int helper(int x) { return x * 2; }
```

When you compile with `-DUNIT_TESTING`, `STATIC` becomes empty, so the function is visible for tests!

### 4. CommonFunctions Class

#### String Conversion

```cpp
std::string CommonFunctions::toStandardString(System::String^ string)
{
    // Converts .NET String^ to std::string
    using System::Runtime::InteropServices::Marshal;
    System::IntPtr pointer = Marshal::StringToHGlobalAnsi(string);
    char* charPointer = reinterpret_cast<char*>(pointer.ToPointer());
    std::string returnString(charPointer, string->Length);
    Marshal::FreeHGlobal(pointer);
    return returnString;
}
```

This is needed because the application uses C++/CLI which combines .NET and native C++.

#### Saving Mock File

```cpp
void CommonFunctions::SaveMockedFile(
    const std::string& mockedFilePath,
    const std::string& a_originalFilePath,
    const std::vector<std::string>& a_mockFunctions)
{
    std::ofstream writer(mockedFilePath);
    
    // 1. Adds CMocka headers
    writer << "#include <stdarg.h>\n";
    writer << "#include <stddef.h>\n";
    writer << "#include <setjmp.h>\n";
    writer << "#include <string.h>\n";
    writer << "#include \"cmocka.h\"\n";
    
    // 2. Finds and adds related .h files
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.path().extension() == ".h") {
            writer << "#include \"" << entry.path().filename().string() << "\"\n";
        }
    }
    
    // 3. Writes mock functions
    writer << "\n// Mocked functions\n";
    for (const auto& mockFunction : a_mockFunctions) {
        writer << mockFunction;
    }
}
```

### 5. Advanced Settings (IncludeModuleSettings)

For complex scenarios, users can configure how mock functions are generated:

```cpp
ref struct ParameterSettings
{
    System::String^ paramName;   // Parameter name
    System::String^ setting;     // "None", "Ptr", or "Length"
};

ref struct FunctionSettings
{
    System::String^ functionName;
    System::String^ returnType;
    System::Collections::Generic::List<ParameterSettings^>^ parameters;
};
```

This enables special handling for functions that copy data:

```cpp
// Original function
int read_data(uint8_t* buffer, size_t length);

// With settings Ptr=buffer, Length=length, generates:
int read_data(uint8_t* buffer, size_t length)
{
    uint8_t* cpy_ptr = mock_ptr_type(uint8_t*);
    size_t cpy_length = (size_t)mock();
    if(cpy_ptr != NULL && cpy_length > 0U)
    {
        memcpy(buffer, cpy_ptr, cpy_length);
    }
    return (int)mock();
}
```

This enables testing functions that return data through pointer parameters!

---

## Usage Examples

### Example 1: Mocking a Single File

1. Launch Mockingbird
2. Click on "Mock One File"
3. Select a C file (e.g., `network.c`)
4. Select save location
5. You get `network.c` with mock functions

### Example 2: Mocking All Dependencies

1. Click on "Mock Files"
2. Select folder with your MUT (Module Under Test)
3. Application finds all `#include` directives
4. Check which dependencies you want to mock
5. Select source folder where original files are located
6. Application generates mocks folder with all mock files

### Example 3: Testing with Generated Mocks

```c
// ===== test_my_module.c =====
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

// Include your module
#include "my_module.h"

// Function from my_module.c that we're testing
extern int process_sensor_data(void);

// Test
static void test_process_sensor_data(void **state) {
    // Use generated macro
    MOCK_read_sensor(0, 100);  // sensor_id=0, returns 100
    MOCK_read_sensor(1, 200);  // sensor_id=1, returns 200
    
    int result = process_sensor_data();
    
    assert_int_equal(result, 150);  // We expect average
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_process_sensor_data),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```

---

## Conclusion

Mockingbird automates a process that would otherwise be manual, tedious, and error-prone:

1. **Parses** C source code using ctags
2. **Analyzes** function signatures
3. **Generates** CMocka-compatible mock functions
4. **Creates** macros for easier use in tests

This automation significantly speeds up unit test development for C projects and reduces the possibility of human error when manually writing mock functions.
