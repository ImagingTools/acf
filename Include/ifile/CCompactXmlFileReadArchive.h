#ifndef ifile_CCompactXmlFileReadArchive_included
#define ifile_CCompactXmlFileReadArchive_included


// Qt includes
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
class CCompactXmlFileReadArchive: public iser::CCompactXmlReadArchiveBase
{
public:
	typedef iser::CCompactXmlReadArchiveBase BaseClass;

	/**
		Constructor initializing archive to work with file.
	*/
	CCompactXmlFileReadArchive(
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
	bool m_serializeHeader;
	QString m_openFileName;
};


} // namespace ifile


#endif // !ifile_CCompactXmlFileReadArchive_included


