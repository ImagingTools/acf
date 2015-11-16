#ifndef iser_CCompactXmlWriteArchiveBase_included
#define iser_CCompactXmlWriteArchiveBase_included


#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QFile>

// ACF includes
#include "iser/CTextWriteArchiveBase.h"
#include "iser/CXmlDocumentInfoBase.h"

#include "iser/iser.h"


namespace iser
{


/**
	Base class of compact XML write archive.
*/
class CCompactXmlWriteArchiveBase:
			public iser::CTextWriteArchiveBase,
			public iser::CXmlDocumentInfoBase
{
public:
	typedef iser::CTextWriteArchiveBase BaseClass;

	/**
		Close the archive and get the created XML string.
	*/
	virtual QByteArray GetString() const;

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& subTag, int& count);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool Process(QString& value);
	using BaseClass::Process;

protected:
	CCompactXmlWriteArchiveBase(
				const iser::IVersionInfo* versionInfoPtr,
				bool serializeHeader,
				const iser::CArchiveTag& rootTag);

	bool WriteStringNode(const QString& text);

	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text);

	QDomDocument m_document;
	QDomElement m_currentParent;

private:
	QByteArray m_currentAttribute;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;

	QList<const iser::CArchiveTag*> m_tagsStack;
};


} // namespace iser


#endif // !iser_CCompactXmlWriteArchiveBase_included


