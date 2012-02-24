#ifndef isys_IFileInfo_included
#define isys_IFileInfo_included


/// Qt includes
#include <QString>


// ACF includes
#include "istd/IPolymorphic.h"
#include "istd/IChangeable.h"
#
#include "isys/IDateTime.h"


namespace isys
{


/**
	Provide info about a file

	\ingroup Persistence
*/
class IFileInfo: virtual public iser::ISerializable
{
public:
	/**
		Get file path.
	*/
	virtual QString GetFilePath() const = 0;

	/**
		Get the file permissions.
	*/
	virtual int GetPermissions() const = 0;
	
	/**
		Get the last modification time.
	*/
	virtual const isys::IDateTime& GetModificationTime() const = 0;
};


} // namespace isys


#endif // !isys_IFileInfo_included


