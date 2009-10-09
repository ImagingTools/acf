#ifndef iser_IFileLoader_included
#define iser_IFileLoader_included


#include "istd/IPolymorphic.h"
#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Provide loading and saving of objects.

	\ingroup Persistence
*/
class IFileLoader: virtual public istd::IPolymorphic
{
public:

	enum OperationState
	{
		StateOk,
		StateAborted,
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
		Describe detailed options of query.
	*/
	enum QueryFlags
	{
		/**
			No loading operation should be considered.
		*/
		QF_NO_LOADING = 0x0001,
		/**
			No saving operation should be considered.
		*/
		QF_NO_SAVING = 0x0002,
		/**
			Only operations with specified file name should be considered.
		*/
		QF_NAMED_ONLY = 0x0004,
		/**
			Only operations without specified file name (anonymous) should be considered.
		*/
		QF_ANONYMOUS_ONLY = 0x0008
	};

	/**
		Returns \c true if object \c dataObject can be loaded/saved.
		\param	dataObjectPtr	optional pointer to data object should be loaded/stored.
								It can be NULL if any object is meant.
		\param	filePathPtr		optional pointer to file should be loaded/stored.
								It can be NULL if any file is meant.
								If it points at empty string, anonymous loading is mean.
		\param	flags			combination of flags defined in QueryFlags.
		\param	beQuiet			if true, no user message output is allowed.
	*/
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const = 0;

	/**
		This function loads data \c data from file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const = 0;

	/**
		Get file extensions supported by this loader
		\param	result		list of extensions, e.g. {"txt", "doc"}.
		\param	flags		set of flags \sa QueryFlags.
		\param	doAppend	if true, list of extensions should be appended to existing list.
	*/
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const = 0;
	/**
		Get description of object type associated with single extension.
	*/
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const = 0;
};


} // namespace iser


#endif // !iser_IFileLoader_included


