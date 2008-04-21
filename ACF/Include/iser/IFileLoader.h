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
		Returns \c true if object \c dataObject can be loaded/saved.
		\param	dataObjectPtr	optional pointer to data object should be loaded/stored.
								It can be NULL if any object is meant.
		\param	filePathPtr		optional pointer to file should be loaded/stored.
								It can be NULL if any file is meant.
		\param	forLoading		true, if this question is related to loading.
		\param	forSaving		true, if this question is related to saveing.
	*/
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true) const = 0;

	/**
		This function loads data \c data from file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const = 0;

	/**
		Returns the last opened file name.
		\returns serialization state. \sa SerializationState
	*/
	virtual const istd::CString& GetLastLoadFileName() const = 0;

	/**
		Returns the last saved file name.
		\returns serialization state. \sa SerializationState
	*/
	virtual const istd::CString& GetLastSaveFileName() const = 0;
};


} // namespace iser


#endif // iser_IFileLoader_included


