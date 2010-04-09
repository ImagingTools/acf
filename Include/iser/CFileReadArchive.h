#ifndef iser_CFileReadArchive_included
#define iser_CFileReadArchive_included


// STL includes
#include <fstream>


// ACF includes
#include "iser/CBinaryReadArchiveBase.h"


namespace iser
{


/**
	Simple implementation of archive reading from own ACF format binary file.
	This imlementation is very fast and efficient and should be used if any standardized file format is needed.

	\ingroup Persistence
*/
class CFileReadArchive: public CBinaryReadArchiveBase
{
public:
	typedef CBinaryReadArchiveBase BaseClass;

	/**
		Contructor.
		\param	filePath			name of file.
		\param	supportTagSkipping	if it is true skipping of tags on EndTag is supported.
									Please note that supporting of tag skipping store additional data in file
									and it is not compatible with files created without tag skipping.
									\sa	EndTag and IsTagSkippingSupported.
		\param	serializeHeader		if it is true (default) archive header will be serialized.
	*/
	CFileReadArchive(const istd::CString& filePath, bool supportTagSkipping = true, bool serializeHeader = true);

	// reimplemented (iser::IArchive)
	virtual bool IsTagSkippingSupported() const;
	virtual bool BeginTag(const CArchiveTag& tag);
	virtual bool EndTag(const CArchiveTag& tag);
	virtual bool ProcessData(void* data, int size);
	
protected:
	struct TagStackElement
	{
		I_DWORD tagBinaryId;
		I_DWORD endPosition;
		bool useTagSkipping;
	};

	// reimplemented (istd::ILogger)
	virtual void DecorateMessage(
				MessageCategory category,
				int id,
				int flags,
				istd::CString& message,
				istd::CString& messageSource) const;

private:
	istd::CString m_filePath;
	std::ifstream m_stream;

	bool m_supportTagSkipping;

	typedef std::vector<TagStackElement> TagStack;

	TagStack m_tagStack;
};


} // namespace iser


#endif // !iser_CFileReadArchive_included

