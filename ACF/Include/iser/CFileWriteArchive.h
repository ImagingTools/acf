#ifndef iser_CFileWriteArchive_included
#define iser_CFileWriteArchive_included


#include <fstream>

#include "iser/CBinaryWriteArchiveBase.h"


namespace iser
{


class CFileWriteArchive: public CBinaryWriteArchiveBase
{
public:
	typedef CBinaryWriteArchiveBase BaseClass;

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
					const istd::CString& filePath,
					bool supportTagSkipping = true,
					const IVersionInfo* versionInfoPtr = NULL,
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
		I_DWORD tagBinaryId;
		I_DWORD endFieldPosition;
	};
	
private:
	std::ofstream m_stream;

	bool m_supportTagSkipping;

	typedef std::vector<TagStackElement> TagStack;

	TagStack m_tagStack;
};


} // namespace iser


#endif // iser_CFileWriteArchive_included

