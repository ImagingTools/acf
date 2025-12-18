#ifndef iser_CCompactXmlMemReadArchive_included
#define iser_CCompactXmlMemReadArchive_included


// Qt includes
#include <QtCore/QBuffer>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include <iser/CCompactXmlReadArchiveBase.h>


namespace iser
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CCompactXmlMemReadArchive: public CCompactXmlReadArchiveBase
{
public:
	typedef CCompactXmlReadArchiveBase BaseClass;

	/**
		Constructor initializing archive from memory.
	*/
	CCompactXmlMemReadArchive(
				const QByteArray& inputString,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const override;

private:
	QBuffer m_buffer;
};


} // namespace iser


#endif // !iser_CCompactXmlMemReadArchive_included


