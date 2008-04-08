#ifndef istd_IFileEngine_included
#define istd_IFileEngine_included


#include "istd/IPolymorphic.h"

#include "istd/CString.h"


namespace istd
{


class IFileEngine: virtual public istd::IPolymorphic
{
public:
	/**
		This function loads data \c data from file \c filePath.
	*/
	virtual bool Load(IPolymorphic& data, const istd::CString& filePath) const = 0;

	/**
		This function saves data \c data to file \c filePath.
	*/
	virtual bool Save(IPolymorphic& data, const istd::CString& filePath) const = 0;
};


} // namespace istd


#endif // !istd_IFileEngine_included

