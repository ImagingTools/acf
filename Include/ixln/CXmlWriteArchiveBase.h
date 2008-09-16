#ifndef ixln_CXmlWriteArchiveBase_included
#define ixln_CXmlWriteArchiveBase_included


#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/framework/XMLFormatter.hpp>

#include "istd/TDelPtr.h"

#include "iser/CTextWriteArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"

#include "ixln/CXercesBase.h"


namespace ixln
{


/**
	Base class for XERCES based write archive.
*/
class CXmlWriteArchiveBase: public iser::CTextWriteArchiveBase, public CXercesBase, public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;

	virtual ~CXmlWriteArchiveBase();

	bool IsValid();
	bool Flush();

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(std::string& data);
	virtual bool Process(istd::CString& data);

protected:
	CXmlWriteArchiveBase(const iser::IVersionInfo* versionInfoPtr);

	void Init(xercesc::XMLFormatTarget* formTarget);
	void Reset();

private:
	bool m_isFlushed;
	bool m_isFirstParamInTag;

	xercesc::DOMWriter* m_writerPtr;
	istd::TDelPtr<xercesc::XMLFormatTarget> m_formTargetPtr;
};


} // namespace ixln


#endif // !ixln_CXmlWriteArchiveBase_included


