#pragma once


// ACF includes
#include <iprm/IOptionsList.h>
#include <ifile/IFileTypeInfo.h>


namespace ifile
{


/**
	Interface for describing constraints of file types supported by the file repository implementation.

	The user of this inteface can obtain all informations about supported resource types using iprm::IOptionsList interface.

	\interface IFileResourceTypeConstraints
*/
class IFileResourceTypeConstraints: virtual public iprm::IOptionsList
{
public:
	/**
		Get file type information for a given resource type.
	*/
	virtual const ifile::IFileTypeInfo* GetFileTypeInfo(int resourceTypeIndex) const = 0;
};


} // namespace ifile

