#ifndef iser_CXmlReadArchiveBase_included
#define iser_CXmlReadArchiveBase_included


// ACF includes
#include "iser/CTextReadArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


/**
	Base class for XML based reading archives.
*/
class CXmlReadArchiveBase:
			public CTextReadArchiveBase,
			public CXmlDocumentInfoBase
{
public:
	typedef CReadArchiveBase BaseClass;

	enum MessageId
	{
		MI_TAG_ERROR = 0x3f320a0,
		MI_TAG_SKIPPED
	};

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const CArchiveTag& tag);
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count);
	virtual bool EndTag(const CArchiveTag& tag);
	virtual bool Process(QByteArray& value);
	virtual bool Process(QString& value);
	using BaseClass::Process;

protected:
	CXmlReadArchiveBase(const CArchiveTag& rootTag);

	/**
		Extended implementation of EndTag() with additional flag signalizing that tag is skipped.
		\param	tag				serializing tag should be ended.
		\param	wasTagSkipped	if tag is skipped it will be set, otherwise state is not changed.
	*/
	bool InternEndTag(const CArchiveTag& tag, bool& wasTagSkipped);

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
				const QByteArray& delimeters,
				QByteArray& result,
				bool skipDelimeter = true,
				char* foundDelimeterPtr = NULL) = 0;

private:
	CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;
	bool m_isTagEmpty;
};


} // namespace iser


#endif // !iser_CXmlReadArchiveBase_included


