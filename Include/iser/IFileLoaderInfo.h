#ifndef iser_IFileLoaderInfo_included
#define iser_IFileLoaderInfo_included


#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iser/iser.h"


namespace iser
{


/**
	Provilde some information about file loader state.
*/
class IFileLoaderInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Returns the last opened file name.
	*/
	virtual istd::CString GetLastLoadFileName() const = 0;

	/**
		Returns the last saved file name.
	*/
	virtual istd::CString GetLastSaveFileName() const = 0;
};


} // namespace iser


#endif // !iser_IFileLoaderInfo_included


