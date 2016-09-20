#ifndef ifile_IFileSystemInfoProvider_included
#define ifile_IFileSystemInfoProvider_included


// ACF includes
#include <istd/IChangeable.h>
#include <istd/CSystem.h>
#include <iprm/IOptionsList.h>


namespace ifile
{


/**
	Provider of file system related informations.
*/
class IFileSystemInfoProvider: virtual public istd::IChangeable
{
public:
	/**
		Get the list of drives available on the local machine.
	*/
	virtual const iprm::IOptionsList& GetDriveList() const = 0;

	/**
		Get info about a given file drive.
		\param driveIndex	Index of the file drive specified as index of the element inside of the list returned by GetDriveList.
							Negative values mean that information about all available file drives will be cummulated.
	*/
	virtual const istd::CSystem::FileDriveInfo* GetFileDriveInfo(int driveIndex = -1) const = 0;
};


} // namespace ifile


#endif // !ifile_IFileSystemInfoProvider_included


