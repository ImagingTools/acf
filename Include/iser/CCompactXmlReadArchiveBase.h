#ifndef iser_CCompactXmlReadArchiveBase_included
#define iser_CCompactXmlReadArchiveBase_included


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include <iser/CXmlDocumentInfoBase.h>
#include <iser/CTextReadArchiveBase.h>

#include <iser/iser.h>


namespace iser
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CCompactXmlReadArchiveBase: public iser::CTextReadArchiveBase, public iser::CXmlDocumentInfoBase
{
public:
	CCompactXmlReadArchiveBase(
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const override;
	virtual bool BeginTag(const iser::CArchiveTag& tag) override;
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count) override;
	virtual bool EndTag(const iser::CArchiveTag& tag) override;
	virtual bool Process(QString& value) override;
	using BaseClass::Process;

protected:
	bool ReadStringNode(QString& text);

	bool SetContent(QIODevice* devicePtr);

	// reimplemented (iser::CTextReadArchiveBase)
	virtual bool ReadTextNode(QByteArray& text) override;

	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

	QDomDocument m_document;
	QDomElement m_currentParent;

private:
	QByteArray m_currentAttribute;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isNewFormat;			// idicate that new format is enabled
	bool m_allowAttribute;		// indicate if attribute outputting is allowed now

	typedef QList<const iser::CArchiveTag*> TagsList;
	TagsList m_tagsStack;
};


} // namespace iser


#endif // !iser_CCompactXmlReadArchiveBase_included


