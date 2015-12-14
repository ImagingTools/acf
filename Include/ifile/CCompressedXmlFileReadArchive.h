#ifndef ifile_CCompressedXmlFileReadArchive_included
#define ifile_CCompressedXmlFileReadArchive_included


// Qt includes
#include <QtCore/QBuffer>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>

// ACF includes
#include "iser/CCompactXmlReadArchiveBase.h"


namespace ifile
{


/**
	Qt-based implementation of archive reading from XML file.

	\ingroup Persistence
*/
class CCompressedXmlFileReadArchive: public iser::CCompactXmlReadArchiveBase
{
public:
	typedef iser::CCompactXmlReadArchiveBase BaseClass;

	/**
		Constructor initializing archive to work with file.
	*/
	CCompressedXmlFileReadArchive(
				const QString& filePath = "",
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	bool OpenFile(const QString& filePath);

protected:
	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

private:
	QString m_openFileName;
	QBuffer m_buffer;
};


} // namespace ifile


#endif // !ifile_CCompressedXmlFileReadArchive_included


