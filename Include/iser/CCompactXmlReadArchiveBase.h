#ifndef iser_CCompactXmlReadArchiveBase_included
#define iser_CCompactXmlReadArchiveBase_included


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include "iser/CXmlDocumentInfoBase.h"
#include "iser/CTextReadArchiveBase.h"

#include "iser/iser.h"


namespace iser
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CCompactXmlReadArchiveBase: public iser::CTextReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CCompactXmlReadArchiveBase(const iser::CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);
	using BaseClass::Process;

protected:
	bool ReadStringNode(QString& text);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text);

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

	QDomDocument m_document;
	QDomElement m_currentParent;

private:
	QByteArray m_currentAttribute;

	iser::CArchiveTag m_rootTag;

	typedef QList<const iser::CArchiveTag*> TagsList;
	TagsList m_tagsStack;
};


} // namespace iser


#endif // !iser_CCompactXmlReadArchiveBase_included


