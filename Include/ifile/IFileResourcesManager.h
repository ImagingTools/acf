#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QList>

// ACF includes
#include <istd/TSmartPtr.h>
#include <iprm/IOptionsList.h>
#include <idoc/IDocumentMetaInfo.h>


namespace ifile
{


class IFileResourceTypeConstraints;


/**
	Manager of file-based resources.
*/
class IFileResourcesManager: virtual public istd::IChangeable
{
public:
	typedef istd::TSmartPtr<istd::IChangeable> DataObjectPtr;
	typedef QList<QByteArray> Ids;

	/**
		Get list of file/resource IDs in the repository.
	*/
	virtual Ids GetFileIds() const = 0;

	/**
		Get type contraints describing each supported type of resource.
		\return	NULL, if no type constraints are known.
	*/
	virtual const IFileResourceTypeConstraints* GetResourceTypeConstraints() const = 0;

	/**
		Get type ID if the resource.
		\resourceId	ID of the file resource in the storage.
		\return Type ID of the resource or an empty string, if the resource was not found or the type is unknown.
	*/
	virtual QByteArray GetResourceTypeId(const QByteArray& resourceId) const = 0;

	/**
		Create new file resource.
		\param ID of the resource (file) type.
		\param Initial state of the data object. Can be \c NULL.
		\param File path of the new resource.
		\param Name (alais) of the resource.
		\return ID of the created resource or an empty string if the operation failed.
	*/
	virtual QByteArray CreateNewFile(
				const QByteArray& fileTypeId,
				const istd::IChangeable* dataObjectPtr,
				const QString& filePath = QString(),
				const QString& resourceName = QString()) = 0;

	/**
		Remove file from the resources storage.
		\param resourceId ID of the file. 
	*/
	virtual bool RemoveFile(const QByteArray& fileId) = 0;

	/**
		Get data object if the given resource.
	*/
	virtual DataObjectPtr GetDataObject(const QByteArray& fileId) const = 0;

	/**
		Get meta info of an existing file resource.
		\param resourceId			ID of the file resource.
		\return Pointer to the meta-info instance or \c NULL, if no information could be provided.
	*/
	virtual const idoc::IDocumentMetaInfo* GetFileMetaInfo(const QByteArray& resourceId) const = 0;

	/**
		Get the name of the file resource in the storage.
		\param resourceId	ID of the file in the storage.
	*/
	virtual QString GetResourceName(const QByteArray& resourceId)  const = 0;

	/**
		Set the name of the file resource in the storage.
		\param resourceId	ID of the file in the storage.
		\param name			Name of the file resource.
	*/
	virtual bool SetResourceName(const QByteArray& resourceId, const QString& name) = 0;

	/**
		Get the description of the file resource in the storage.
		\param resourceId	ID of the file in the storage.
	*/
	virtual QString GetResourceDescription(const QByteArray& resourceId)  const = 0;

	/**
		Set the description of the file resource in the storage.
		\param resourceId	ID of the file in the storage.
		\param description	Description of the file resource.
	*/
	virtual bool SetResourceDescription(const QByteArray& resourceId, const QString& description) = 0;
};


} // namespace ifile


