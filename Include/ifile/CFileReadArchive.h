#ifndef ifile_CFileReadArchive_included
#define ifile_CFileReadArchive_included


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QFile>

// ACF includes
#include "iser/CBinaryReadArchiveBase.h"
#include "ifile/CFileArchiveInfo.h"


namespace ifile
{


/**
	Simple implementation of archive reading from own ACF format binary file.
	This imlementation is very fast and efficient and should be used if any standardized file format is needed.

	\ingroup Persistence
*/
class CFileReadArchive:
			public iser::CBinaryReadArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef iser::CBinaryReadArchiveBase BaseClass;
	typedef CFileArchiveInfo BaseClass2;

	/**
		Contructor.
		\param	filePath			name of file.
		\param	supportTagSkipping	if it is true skipping of tags on EndTag is supported.
									Please note that supporting of tag skipping store additional data in file
									and it is not compatible with files created without tag skipping.
									\sa	EndTag and IsTagSkippingSupported.
		\param	serializeHeader		if it is true (default) archive header will be serialized.
	*/
	CFileReadArchive(const QString& filePath, bool supportTagSkipping = true, bool serializeHeader = true);

	// reimplemented (ifile::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool ProcessData(void* data, int size);
	
protected:
	struct TagStackElement
	{
		quint32 tagBinaryId;
		quint32 endPosition;
		bool useTagSkipping;
	};

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				istd::IInformationProvider::InformationCategory category,
				int id,
				int flags,
				QString& message,
				QString& messageSource) const;

	// reimplemented (iser::CArchiveBase)
	virtual int GetMaxStringLength() const;

private:
	QFile m_file;

	bool m_supportTagSkipping;

	typedef QVector<TagStackElement> TagStack;

	TagStack m_tagStack;
};


} // namespace ifile


#endif // !ifile_CFileReadArchive_included


