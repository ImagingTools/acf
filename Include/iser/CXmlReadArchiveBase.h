#ifndef iser_CXmlReadArchiveBase_included
#define iser_CXmlReadArchiveBase_included


#include "iser/CTextReadArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


class CXmlReadArchiveBase: public iser::CTextReadArchiveBase, public CXmlDocumentInfoBase
{
public:
	typedef iser::CReadArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const CArchiveTag& tag);
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count);
	virtual bool EndTag(const CArchiveTag& tag);
	virtual bool Process(std::string& value);
	virtual bool Process(istd::CString& value);
	using BaseClass::Process;

protected:
	CXmlReadArchiveBase(const iser::CArchiveTag& rootTag);

	virtual bool SerializeXmlHeader();

	// abstract methods
	/**
		Read input stream till one of specified delimeters is found.
		\param	delimeters		list of delimeters.
		\param	result			result string object. White spaces will be trimmed at the begin and end of this string.
								Delimeter will be not excluded from this string.
		\param	skipDelimeter	if it is true, delimeter will be not parsed by next call of this method.
		\param	foundDelimeter	optional found delimeter will be returned in this character (it means 1 byte, it is not C string).
	*/
	virtual bool ReadToDelimeter(
				const std::string& delimeters,
				std::string& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL) = 0;

private:
	iser::CArchiveTag m_rootTag;
};


} // namespace iser


#endif // !iser_CXmlReadArchiveBase_included


