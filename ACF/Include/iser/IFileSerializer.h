#ifndef iser_IFileSerializer_included
#define iser_IFileSerializer_included


#include "istd/IPolymorphic.h"

#include "istd/CString.h"


namespace iser
{


class ISerializable;


class IFileSerializer: virtual public istd::IPolymorphic
{
public:

	enum SerializationState
	{
		StateOk,
		StateAborted,
		StateFailed
	};

	/**
		This function loads data \c data from file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int LoadFromFile(ISerializable& data, const istd::CString& filePath) const = 0;

	/**
		This function saves data \c data to file \c filePath
		\returns serialization state. \sa SerializationState
	*/
	virtual int SaveToFile(const ISerializable& data, const istd::CString& filePath) const = 0;
	
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


} // namespace iser


#endif // iser_IFileSerializer_included

