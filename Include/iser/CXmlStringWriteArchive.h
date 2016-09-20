#ifndef iser_CXmlStringWriteArchive_included
#define iser_CXmlStringWriteArchive_included


// Qt includes
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CXmlWriteArchiveBase.h>


namespace iser
{


/**
	Simple implementation of archive for writing of XML given as a string.

	\ingroup Persistence
*/
class CXmlStringWriteArchive: public CXmlWriteArchiveBase
{
public:
	typedef CXmlWriteArchiveBase BaseClass;

	CXmlStringWriteArchive(
				const IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const CArchiveTag& rootTag = s_acfRootTag);
	virtual ~CXmlStringWriteArchive();

	/**
		Close the archive and get the created XML string.
	*/
	virtual const QByteArray& GetString() const;

protected:
	void EnsureArchiveClosed();

	// reimplemented (iser::CXmlWriteArchiveBase)
	virtual bool WriteString(const QByteArray& value);

private:
	QTextStream m_stream;
	QBuffer m_buffer;
};


} // namespace iser


#endif // !iser_CXmlStringWriteArchive_included


