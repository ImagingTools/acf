#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <ibase/IProgressManager.h>
#include <idoc/IDocumentMetaInfo.h>


namespace ifile
{


/**
	Interface for acquiring meta-information of a file.
*/
class IFileMetaInfoProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Create meta information of an existing file.
		\param filePath					Path of the file.
		\param progressManagerPtr		Optional progess manager for the meta-info extraction operation.
		\return Pointer to the meta-info instance or \c NULL, if no information could be provided.
		\note The caller of this method is responsible for the memory management of the created meta-info object.
	*/
	virtual idoc::MetaInfoPtr GetFileMetaInfo(const QString& filePath, ibase::IProgressManager* progressManagerPtr = NULL) const = 0;
};


} // namespace ifile


