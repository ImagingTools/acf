#pragma once


// Qt includes
#include <QtCore/QByteArray>
#include <QtCore/QList>

// ACF includes
#include <istd/TSmartPtr.h>
#include <ilog/IMessageConsumer.h>
#include <iprm/IOptionsList.h>
#include <idoc/IDocumentMetaInfo.h>
#include <ifile/IFileMetaInfoProvider.h>


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
		Status of the resource license.
	*/
	enum LicenseStatus
	{
		/**
			The resource is not licensed. It means, the resource can be used and edited without any restrictions.
		*/
		LS_NONE,

		/**
			The resource is licensed and the user of the repository has all neccessary rights for using it.
		*/
		LS_OK,

		/**
			The resource is licensed, but the user of the repository has no valid licenes for using it.
		*/
		LS_NOT_VALID
	};


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
		Get data object for the given resource.
	*/
	virtual DataObjectPtr GetDataObject(const QByteArray& fileId) const = 0;

	/**
		Get meta info of an existing file resource.
		\param resourceId			ID of the file resource.
		\return Pointer to the meta-info instance or \c NULL, if no information could be provided.
	*/
	virtual ifile::IFileMetaInfoProvider::MetaInfoPtr GetFileMetaInfo(const QByteArray& resourceId) const = 0;

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

	/**
		Get the license status of the given resource.
		\sa LicenseStatus
	*/
	virtual LicenseStatus GetLicenseStatus(
				const QByteArray& resourceId,
				const QByteArray& resourceTypeId = QByteArray(),
				ilog::IMessageConsumer* messageConsumerPtr = NULL) const = 0;
};


} // namespace ifile


