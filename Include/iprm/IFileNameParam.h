#ifndef iprm_IFileNameParam_included
#define iprm_IFileNameParam_included


#include <QString>

#include "iser/ISerializable.h"

#include "iprm/iprm.h"


namespace iprm
{


/**
	Common interface for a file system item settings.
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


} // namespace iprm


#endif // !iprm_IFileNameParam_included


