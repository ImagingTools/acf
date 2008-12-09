#ifndef iser_IFileLoader_included
#define iser_IFileLoader_included


#include "istd/IPolymorphic.h"
#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Provide loading and saveing of objects.
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
		Returns \c true if object \c dataObject can be loaded/saved.
		\param	dataObjectPtr	optional pointer to data object should be loaded/stored.
								It can be NULL if any object is meant.
		\param	filePathPtr		optional pointer to file should be loaded/stored.
								It can be NULL if any file is meant.
		\param	forLoading		true, if this question is related to loading.
		\param	forSaving		true, if this question is related to saveing.
		\param	beQuiet			if true, no user message output is allowed.
	*/
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true,
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
		Get file extensions supported by this loader.
	*/
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const = 0;
};


} // namespace iser


#endif // !iser_IFileLoader_included


