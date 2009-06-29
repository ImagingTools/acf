#include "iwin/CApplicationEnvironment.h"

#include <windows.h>


EXTERN_C IMAGE_DOS_HEADER __ImageBase;


namespace iwin
{


// public methods
	
// reimplemented (isys::IApplicationEnvironment)

istd::CString CApplicationEnvironment::GetTempDirPath() const
{
	wchar_t tempPath[MAX_PATH] = {0};
	::GetTempPathW(MAX_PATH, tempPath);

	int length = wcslen(tempPath);

	if (tempPath[length - 1] == '\\'){
		tempPath[length - 1] = '\0';
	}

	return istd::CString(tempPath);
}


istd::CString CApplicationEnvironment::GetWorkingDirectory() const
{
	wchar_t workingDirectory[MAX_PATH] = {0};

	if (::GetCurrentDirectoryW(MAX_PATH, workingDirectory) != 0){
		return istd::CString(workingDirectory);
	}

	return istd::CString();
}


istd::CStringList CApplicationEnvironment::GetApplicationArguments() const
{
	istd::CStringList applicationArguments;

	LPWSTR commandLinePtr = GetCommandLineW();
	if (commandLinePtr != NULL){
		int argumentsCount = 0;
		LPWSTR* applicationArgumentsPtr = CommandLineToArgvW(commandLinePtr, &argumentsCount);
		if (applicationArgumentsPtr != NULL){
			for (int argumentIndex = 0; argumentIndex < argumentsCount; argumentIndex++){
				applicationArguments.push_back(applicationArgumentsPtr[argumentIndex]);
			}
		}
	}

	return applicationArguments;
}


istd::CString CApplicationEnvironment::GetModulePath(bool useApplicationModule /*= false*/, bool onlyDirectory /*= false*/) const
{
	WCHAR moduleFileName[MAX_PATH] = {0};

	HINSTANCE moduleBasePtr = useApplicationModule ? NULL : (HINSTANCE)&__ImageBase;

	::GetModuleFileNameW(moduleBasePtr, moduleFileName, MAX_PATH);

	if (onlyDirectory){
		for (int i = MAX_PATH - 1; i >= 0; i--)   {   
			if (moduleFileName[i] == '\\' || moduleFileName[i] == '/'){   
				moduleFileName[i] = '\0';
				break;
			}
		}   	
	}

	return istd::CString(moduleFileName);
}


CApplicationEnvironment::EnvironmentVariables CApplicationEnvironment::GetEnvironmentVariables() const
{
	EnvironmentVariables environmentVariables;

	LPWCH environmentStringsPtr = GetEnvironmentStringsW();
	if (environmentStringsPtr != NULL){
		LPCWSTR lpszVariable = (LPCWSTR)environmentStringsPtr;
		while (*lpszVariable){
			istd::CString varPair = lpszVariable;
			istd::CString::size_type separatorIndex = varPair.rfind('=');
			if (separatorIndex != istd::CString::npos){
				istd::CString variableName = varPair.substr(0, separatorIndex);
				istd::CString variableValue = varPair.substr(separatorIndex + 1, varPair.length());
			
				environmentVariables[variableName] = variableValue;
			}

			lpszVariable += lstrlenW(lpszVariable) + 1;
		}

		FreeEnvironmentStringsW(environmentStringsPtr);
	}

	return environmentVariables;
}


void CApplicationEnvironment::SetEnvironmentVariableValue(const istd::CString& variableName, const istd::CString& value)
{
	::SetEnvironmentVariable(variableName.ToString().c_str(), value.ToString().c_str());
}


} // namespace iwin


