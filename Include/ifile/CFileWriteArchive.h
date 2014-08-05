#ifndef ifile_CFileWriteArchive_included
#define ifile_CFileWriteArchive_included


// Qt includes
#include <QtCore/QVector>
#include <QtCore/QFile>

// ACF includes
#include "iser/CBinaryWriteArchiveBase.h"
#include "ifile/CFileArchiveInfo.h"


namespace ifile
{


/**
	Simple implementation of archive writing to own ACF format binary file.
	This imlementation is very fast and efficient and should be used if any standarized file format is needed.

	\ingroup Persistence
*/
class CFileWriteArchive:
			public iser::CBinaryWriteArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef iser::CBinaryWriteArchiveBase BaseClass;
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
	CFileWriteArchive(
					const QString& filePath,
					const iser::IVersionInfo* versionInfoPtr = NULL,
					bool supportTagSkipping = true,
					bool serializeHeader = true);

	/**
		Return \c true if the archive is valid (e.g. the file medium can be accessed)
	*/
	bool IsArchiveValid() const;

	/**
		Force internal stream object to flush.
	*/
	void Flush();

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const iser::CArchiveTag& tag);
	virtual bool EndTag(const iser::CArchiveTag& tag);
	virtual bool ProcessData(void* data, int size);

protected:
	struct TagStackElement
	{
		quint32 tagBinaryId;
		quint32 endFieldPosition;
	};
	
private:
	QFile m_file;

	bool m_supportTagSkipping;

	typedef QVector<TagStackElement> TagStack;

	TagStack m_tagStack;

	bool m_isValid;
};


// public inline methods

inline bool CFileWriteArchive::IsArchiveValid() const
{
	return m_isValid;
}


} // namespace ifile


#endif // !ifile_CFileWriteArchive_included

