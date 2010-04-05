#ifndef istd_CString_included
#define istd_CString_included


// STL includes
#include <string>
#include <vector>
#include <sstream>


// ACF includes
#include "istd/istd.h"


namespace istd
{


/**
	Standard ACF string definition.
	This class is a wrapper for wchar_t data type from the standard C-library.

	\ingroup Main
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
	bool IsEqual(const CString& str, int compareFlags = 0) const;

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
	std::vector<CString> Split(const CString& separator, bool caseSensitive = true, bool ignoreEmpty = false);

	bool IsEqualNoCase(const CString& str) const;

	// template methods
	template <typename NumberType>
	NumberType ToNumber();

	// static methods
	static const CString& GetEmpty();

	// template static methods
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


inline bool CString::IsEqualNoCase(const CString& str) const
{
	return ToUpper() == str.ToUpper();
}


// template methods

template <typename NumberType>
typename NumberType CString::ToNumber()
{
	NumberType retVal;

	std::istringstream stringStream(ToString());
	stringStream >> retVal;

	return retVal;
}


// template static methods

template <typename NumberType>
CString CString::FromNumber(NumberType number)
{
	std::ostringstream stringStream;
	stringStream << number;

	return stringStream.str();
}


typedef std::vector<CString> CStringList;


#define W(string) istd::CString(string)


} // namespace istd


#endif // !istd_CString_included
