#ifndef ifile_IFileNameParam_included
#define ifile_IFileNameParam_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <iser/ISerializable.h>
#include <ifile/ifile.h>


namespace ifile
{


/**
	Common interface for any file system item name (e.g directory, file or URL path)
*/
class IFileNameParam: virtual public iser::ISerializable
{
public:
	enum PathType
	{
		PT_UNKNOWN,
		PT_FILE,
		PT_DIRECTORY,
		PT_URL
	};

	/**
		Get intended type of the path.
		\sa PathType
	*/
	virtual int GetPathType() const = 0;

	/**
		Get path.
	*/
	virtual const QString& GetPath() const = 0;

	/**
		Set path.
	*/
	virtual void SetPath(const QString& path) = 0;
};


} // namespace ifile


#endif // !ifile_IFileNameParam_included


