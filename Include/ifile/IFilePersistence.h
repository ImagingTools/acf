#ifndef ifile_IFilePersistence_included
#define ifile_IFilePersistence_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IChangeable.h>
#include <ifile/IFileTypeInfo.h>


namespace ibase
{
	class IProgressManager;
}


namespace ifile
{


/**
	Interface providing loading and saving of data objects.

	\ingroup Persistence
*/
class IFilePersistence: virtual public IFileTypeInfo
{
public:
	/**
		Result of operation.
	*/
	enum OperationState
	{
		/**
			Operation was successful.
		*/
		OS_OK,

		/**
			Operation was aborted by user.
		*/
		OS_CANCELED,

		/**
			Operation failed.
		*/
		OS_FAILED
	};

	/**
		Possible file I/O errors.
	*/
	enum ErrorType
	{
		/**
			File could not be written, no write access granted.
		*/
		ET_NO_WRITE_PERMISSIONS,

		/**
			File could not be read, no read access granted.
		*/
		ET_NO_READ_PERMISSIONS,

		/**
			File doesn't exist.
		*/
		ET_FILE_NOT_EXIST
	};

	/**
		List of possible message IDs used in context of this interface.
	*/
	enum MessageId
	{
		/**
			Data object is not supported.
		*/
		MI_BAD_OBJECT_TYPE = 0xabf0,

		/**
			Data object could not be loaded.
		*/
		MI_CANNOT_LOAD,

		/**
			Data object could not be saved.
		*/
		MI_CANNOT_SAVE,

		/**
			File path has an unsupported file exptension.
		*/
		MI_BAD_EXTENSION,

		/**
			File or directory not exists.
		*/
		MI_FILE_NOT_EXIST,
		/*
			Version of file in no more supported.
		*/
		MI_UNSUPPORTED_VERSION
	};

	/**
		Returns \c true if object \c dataObject can be loaded/saved.
		\param	dataObjectPtr	optional pointer to data object should be loaded/stored.
								It can be NULL if any object is meant.
		\param	filePathPtr		optional pointer to file should be loaded/stored.
								It can be NULL if any file is meant.
								If it points at empty string, anonymous loading is mean.
		\param	flags			combination of flags defined in \c QueryFlags and \c ifile::IFileTypeInfo::QueryFlags.
		\param	beQuiet			if true, no user message output is allowed.
	*/
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const = 0;

	/**
		This function loads data \c data from file \c filePath
		\returns File loading state. \sa OperationState
	*/
	virtual int LoadFromFile(
				istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns File saving state. \sa OperationState
	*/
	virtual int SaveToFile(
				const istd::IChangeable& data,
				const QString& filePath = QString(),
				ibase::IProgressManager* progressManagerPtr = NULL) const = 0;
};


} // namespace ifile


#endif // !ifile_IFilePersistence_included


