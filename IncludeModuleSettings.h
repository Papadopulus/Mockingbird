#pragma once
#include <string>
ref struct ParameterSettings
{
	System::String^ paramName;
	System::String^ setting; // "None", "Ptr", or "Length"
};
ref struct FunctionSettings
{
	System::String^ functionName;
	System::String^ returnType;
	System::Collections::Generic::List<ParameterSettings^>^ parameters;
	FunctionSettings() {
		this->parameters = gcnew System::Collections::Generic::List<ParameterSettings^>();
	}
};
ref class IncludeModuleSettings {
public:
	System::String^ includeName;
	System::Collections::Generic::List<FunctionSettings^>^ functions;
	IncludeModuleSettings() {
		functions = gcnew System::Collections::Generic::List<FunctionSettings^>();
	}
};