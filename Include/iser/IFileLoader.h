#ifndef iser_IFileLoader_included
#define iser_IFileLoader_included


// Qt includes
#include <QString>


// ACF includes
#include "istd/IChangeable.h"

#include "iser/IFileTypeInfo.h"


namespace iser
{


/**
	Provide loading and saving of objects.

	\ingroup Persistence
*/
class IFileLoader: virtual public IFileTypeInfo
{
public:
	/**
		Result of operation.
	*/
	enum OperationState
	{
		/**
			Operation was successfull.
		*/
		StateOk,
		/**
			Operation was aborted by user.
		*/
		StateAborted,
		/**
			Operation failed.
		*/
		StateFailed
	};

	/**
		List of possible message ids used in context of this interface.
	*/
	enum MessageId
	{
		MI_BAD_OBJECT_TYPE = 0xabf0,
		MI_CANNOT_LOAD,
		MI_CANNOT_SAVE
	};

	/**
		Returns \c true if object \c dataObject can be loaded/saved.
		\param	dataObjectPtr	optional pointer to data object should be loaded/stored.
								It can be NULL if any object is meant.
		\param	filePathPtr		optional pointer to file should be loaded/stored.
								It can be NULL if any file is meant.
								If it points at empty string, anonymous loading is mean.
		\param	flags			combination of flags defined in \c QueryFlags and \c iser::IFileTypeInfo::QueryFlags.
		\param	beQuiet			if true, no user message output is allowed.
	*/
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const QString* filePathPtr = NULL,
				int flags = -1,
				bool beQuiet = true) const = 0;

	/**
		This function loads data \c data from file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int LoadFromFile(istd::IChangeable& data, const QString& filePath = QString()) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int SaveToFile(const istd::IChangeable& data, const QString& filePath = QString()) const = 0;
};


} // namespace iser


#endif // !iser_IFileLoader_included


