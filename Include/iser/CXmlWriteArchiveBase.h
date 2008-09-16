#ifndef iser_CXmlWriteArchiveBase_included
#define iser_CXmlWriteArchiveBase_included


#include "iser/CTextWriteArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"


namespace iser
{


/**
	Base class for XML storing archives.
*/
class CXmlWriteArchiveBase: public CTextWriteArchiveBase, public CXmlDocumentInfoBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const CArchiveTag& tag);
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count);
	virtual bool EndTag(const CArchiveTag& tag);
	virtual bool Process(std::string& value);
	virtual bool Process(istd::CString& value);

protected:
	CXmlWriteArchiveBase(const IVersionInfo* versionInfoPtr, const iser::CArchiveTag& rootTag);

	bool Flush();
	bool MakeIndent();

	bool SerializeXmlHeader();

	// abstract methods
	virtual bool WriteString(const std::string& value) = 0;

private:
	iser::CArchiveTag m_rootTag;

	int m_indent;
	bool m_isHeaderSerialized;

	bool m_isSeparatorNeeded;
};


} // namespace iser


#endif // !iser_CXmlWriteArchiveBase_included


