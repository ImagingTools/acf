#ifndef iser_CCompactXmlWriteArchiveBase_included
#define iser_CCompactXmlWriteArchiveBase_included


// Qt includes
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtCore/QXmlStreamWriter>
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

	virtual ~CCompactXmlWriteArchiveBase();

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
	bool InitArchive(QIODevice* devicePtr);
	bool Flush();

	bool WriteStringNode(const QString& text);

	// reimplemented (iser::CTextWriteArchiveBase)
	virtual bool WriteTextNode(const QByteArray& text);

	QXmlStreamWriter m_xmlWriter;

private:
	QByteArray m_currentAttribute;

	bool m_serializeHeader;
	iser::CArchiveTag m_rootTag;

	bool m_isSeparatorNeeded;	// idicate that separator must be added before something is outputted
	bool m_allowAttribute;		// indicate if attribute outputting is allowed now

	QList<const iser::CArchiveTag*> m_tagsStack;
};


} // namespace iser


#endif // !iser_CCompactXmlWriteArchiveBase_included


