#ifndef istd_CString_included
#define istd_CString_included


#include "istd/istd.h"


#include <string>
#include <xstring>
#include <vector>


namespace istd
{
	

/**
	This class is a wrapper for wchar_t data type from the standard C-library.
*/
class CString: public ::std::wstring
{
public:
	typedef ::std::wstring BaseClass;

	CString();
	CString(const wchar_t* str);
	CString(const char* str);
	CString(const ::std::string& str);
	CString(const ::std::wstring& str);
	CString(const CString& str);

	/**
		This function returns standard 8-bit string.
	*/
	const ::std::string& ToString() const;

	/**
		This function returns standard 8-bit string.
	*/
	bool IsEmpty() const;

	/**
	This function makes this string empty.
	*/
	void Reset();

	bool operator ==(const CString& other) const;
	bool operator ==(const char*) const;
	bool operator ==(const wchar_t*) const;
	bool operator ==(const ::std::string&) const;
	bool operator !=(const char*) const;
	bool operator !=(const wchar_t*) const;
	bool operator !=(const CString&) const;
	bool operator !=(const ::std::string&) const;

	CString& operator =(const char*);
	CString& operator =(const wchar_t*);
	CString& operator =(const CString&);
	CString& operator =(const ::std::string&);

	CString ToUpper() const;
	CString ToLower() const;

protected:
	void Copy(const CString& other);
	void ConvertToChar() const;

private:
	mutable ::std::string m_stdString;
	mutable bool m_isModified;
};


typedef ::std::vector<istd::CString> CStringList;


#define W(string) istd::CString(string)


} // namespace istd


#endif // !istd_CString_included
