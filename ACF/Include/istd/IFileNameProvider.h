#ifndef istd_IFileNameProvider_included
#define istd_IFileNameProvider_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"


namespace istd
{


/**
	Interface for getting a file or directory name
*/
class IFileNameProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get the file or directory name.
	*/
	virtual istd::CString GetFileName() const = 0;
};


} // namespace istd


#endif // !istd_IFileNameProvider_included


