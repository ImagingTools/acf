#ifndef ixln_CXmlReadArchiveBase_included
#define ixln_CXmlReadArchiveBase_included


#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax/InputSource.hpp>

#include "iser/CTextReadArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"

#include "ixln/CXercesBase.h"


namespace ixln
{


/**
	Base class for XML reading archives.
*/
class CXmlReadArchiveBase: public iser::CTextReadArchiveBase, public CXercesBase, public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CTextReadArchiveBase BaseClass;

    virtual ~CXmlReadArchiveBase();

	virtual bool IsValid() const;

	// reimplemented (iser::IArchive)
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(std::string& data);
	virtual bool Process(istd::CString& data);

protected:
	CXmlReadArchiveBase();

	void Init(xercesc::InputSource& inputSource);
    void Reset();

private:
	bool m_isValid;

    xercesc::DOMBuilder* m_builder;
};


} // namespace ixln


#endif // !ixln_CXmlReadArchiveBase_included


