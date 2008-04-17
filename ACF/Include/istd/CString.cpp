#include <locale>


#include "istd/CString.h"


namespace istd
{


// public methods

CString::CString()
{
	m_isModified = false;
}


CString::CString(const wchar_t* str)
:	BaseClass(str)
{

}


CString::CString(const char* str)
{
	if (str == NULL){
		return;
	}

	int length = strlen(str) + 1;
	wchar_t* wideStringBuffer = new wchar_t[length];
	memset(wideStringBuffer, 0, length * sizeof(wchar_t));
	mbstowcs(wideStringBuffer, str, length);
	*this = std::wstring(wideStringBuffer);

	delete [] wideStringBuffer;
}


CString::CString(const CString& str)
{
	Copy(str);
}


CString::CString(const std::string& str)
{
	*this = CString(str.c_str());
}


CString::CString(const std::wstring& wideString)
:	BaseClass(wideString)
{

}


const std::string& CString::ToString() const
{
	if ((m_stdString.empty() && !empty()) || m_isModified){
		ConvertToChar();

		m_isModified = false;
	}

	return m_stdString;
}


bool CString::IsEqual(const istd::CString& str, int compareFlags) const
{
	if ((compareFlags & CF_CASE_INSENSITIVE) == 0){
		return *this == str;
	}
	else{
		return ToLower() == str.ToLower();
	}
}


void CString::Reset()
{
	m_stdString.clear();

	clear();

	m_isModified = true;
}

	
bool CString::operator==(const CString& str) const
{
	const std::wstring& thisRef = *this;

	return thisRef == (static_cast<const std::wstring&>(str));
}


bool CString::operator==(const char* str) const
{
	return *this == CString(str);
}


bool CString::operator==(const wchar_t* str) const
{
	return *this == CString(str);
}


bool CString::operator==(const std::string& str) const
{
	return *this == CString(str);
}


bool CString::operator!=(const char* str) const
{
	return *this != CString(str);
}


bool CString::operator!=(const wchar_t* str) const
{
	return *this != CString(str);
}


bool CString::operator!=(const CString& str) const
{
	return static_cast<const BaseClass&>(*this) != str;
}


bool CString::operator!=(const std::string& str) const
{
	return *this != CString(str);
}



CString CString::operator+(const CString& str) const
{
	return CString(static_cast<const BaseClass&>(*this) + str);
}


CString& CString::operator=(const char* str)
{
	Copy(CString(str));

	m_isModified = true;

	return *this;
}


CString& CString::operator=(const wchar_t* str)
{
	Copy(CString(str));

	m_isModified = true;

	return *this;
}


CString& CString::operator=(const CString& str)
{
	Copy(str);

	m_isModified = true;

	return *this;
}


CString& CString::operator=(const std::string& str)
{
	Copy(CString(str));

	m_isModified = true;

	return *this;
}


CString& CString::operator+=(const CString& str)
{
	BaseClass::operator+=(str);

	m_isModified = true;

	return *this;
}


CString CString::ToUpper() const
{
	CString upperString;
	for (CString::const_iterator iter = begin(); iter != end();iter++){
		upperString.push_back(std::toupper(*iter, std::locale()));
	}

	return upperString;
}


CString CString::ToLower() const
{
	CString lowerString;
	for(CString::const_iterator iter = begin(); iter != end();iter++){
		lowerString.push_back(std::tolower(*iter, std::locale()));
	}

	return lowerString;
}


// protected methods

void CString::Copy(const CString& str)
{
	clear();
	resize(str.size());

	std::copy(str.begin(), str.end(), begin());

	ConvertToChar();
}


void CString::ConvertToChar() const
{
	int length  = wcslen(c_str())+1;
	char* newAsciiBuffer = new char[length];
	wcstombs(newAsciiBuffer,c_str(), length);
	m_stdString = std::string(newAsciiBuffer);

	delete [] newAsciiBuffer;
}



} // namespace istd
