#ifndef istd_CString_included
#define istd_CString_included


#include "istd/istd.h"


#include <string>
#include <xstring>
#include <vector>
#include <sstream>


namespace istd
{
	

/**
	This class is a wrapper for wchar_t data type from the standard C-library.
*/
class CString: public std::wstring
{
public:
	enum CompareFlags{
		CF_CASE_INSENSITIVE = 0x01
	};

	typedef std::wstring BaseClass;

	CString();
	CString(const wchar_t* str);
	CString(const char* str);
	CString(const std::string& str);
	CString(const std::wstring& str);
	CString(const CString& str);

	/**
		This function returns standard 8-bit string.
	*/
	const std::string& ToString() const;

	/**
		This function returns standard 8-bit string.
	*/
	bool IsEmpty() const;

	/**
		Compare two strings.
		\return	true, if this strings are equal.
	*/
	bool IsEqual(const istd::CString& str, int compareFlags = 0) const;

	/**
	This function makes this string empty.
	*/
	void Reset();

	bool operator==(const CString& str) const;
	bool operator==(const char* str) const;
	bool operator==(const wchar_t* str) const;
	bool operator==(const std::string& str) const;
	bool operator!=(const char* str) const;
	bool operator!=(const wchar_t* str) const;
	bool operator!=(const CString& str) const;
	bool operator!=(const std::string& str) const;

	CString operator+(const CString& str) const;

	CString& operator=(const char* str);
	CString& operator=(const wchar_t* str);
	CString& operator=(const CString& str);
	CString& operator=(const std::string& str);

	CString& operator+=(const CString& str);

	CString ToUpper() const;
	CString ToLower() const;
	
	template <typename NumberType>
	static CString FromNumber(NumberType number);

protected:
	void Copy(const CString& other);
	void ConvertToChar() const;

private:
	mutable std::string m_stdString;
	mutable bool m_isModified;
};


// inline methods

inline bool CString::IsEmpty() const
{
	return empty();
}


// public static methods

template <typename NumberType>
CString CString::FromNumber(NumberType number)
{
	std::ostringstream stringStream;
	stringStream << number << std::ends;

	return stringStream.str();
}


typedef std::vector<istd::CString> CStringList;


#define W(string) istd::CString(string)


} // namespace istd


#endif // !istd_CString_included
