#ifndef iser_CFileWriteArchive_included
#define iser_CFileWriteArchive_included


// STL includes
#include <fstream>
#include <vector>


// ACF includes
#include "iser/CBinaryWriteArchiveBase.h"
#include "iser/CFileArchiveInfo.h"


namespace iser
{


/**
	Simple implementation of archive writing to own ACF format binary file.
	This imlementation is very fast and efficient and should be used if any standarized file format is needed.

	\ingroup Persistence
*/
class CFileWriteArchive:
			public CBinaryWriteArchiveBase,
			public CFileArchiveInfo
{
public:
	typedef CBinaryWriteArchiveBase BaseClass;
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
					const IVersionInfo* versionInfoPtr = NULL,
					bool supportTagSkipping = true,
					bool serializeHeader = true);

	/**
		Force internal stream object to flush.
	*/
	void Flush();

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const CArchiveTag& tag);
	virtual bool EndTag(const CArchiveTag& tag);
	virtual bool ProcessData(void* data, int size);

protected:
	struct TagStackElement
	{
		quint32 tagBinaryId;
		quint32 endFieldPosition;
	};
	
private:
	std::ofstream m_stream;

	bool m_supportTagSkipping;

	typedef std::vector<TagStackElement> TagStack;

	TagStack m_tagStack;
};


} // namespace iser


#endif // !iser_CFileWriteArchive_included

