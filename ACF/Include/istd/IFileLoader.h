#ifndef istd_IFileLoader_included
#define istd_IFileLoader_included


#include "istd/IPolymorphic.h"

#include "istd/CString.h"


namespace istd
{


class IChangeable;


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
	*/
	virtual bool IsObjectSupported(const istd::IChangeable& dataObject) const = 0;

	/**
		Returns \c true if file \c filePath can be loaded/saved.
	*/
	virtual bool IsFileSupported(const istd::CString& filePath) const = 0;

	/**
		This function loads data \c data from file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int LoadFromFile(IChangeable& data, const istd::CString& filePath) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int SaveToFile(const IChangeable& data, const istd::CString& filePath) const = 0;
	
	/**
		Returns the last saved file name.
		\returns serialization state. \sa SerializationState
	*/
	virtual const istd::CString& GetLastSaveFileName() const = 0;

	/**
		Returns the last opened file name.
		\returns serialization state. \sa SerializationState
	*/
	virtual const istd::CString& GetLastOpenFileName() const = 0;
};


} // namespace istd


#endif // istd_IFileLoader_included

