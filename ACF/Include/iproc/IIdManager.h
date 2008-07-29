#ifndef iproc_IIdManager_included
#define iproc_IIdManager_included


#include "isys/ITimer.h"


namespace iproc
{


/**
	Manage processing ID's used in \c iproc::ISupplier.
*/
class IIdManager: virtual public istd::IPolymorphic
{
public:
	/**
		Force to skip to next ID.
		This ID you can get using GetCurrentId().
		\return	true, if next ID could be generated.
		\param	result	result ID after skipping. \sa GetCurrentId
	*/
	virtual bool SkipToNextId(I_DWORD& result) = 0;

	/**
		Get current processed ID.
	*/
	virtual bool GetCurrentId(I_DWORD& result) const = 0;

	/**
		Get timestamp of specified processing ID.
		\param	id		processing ID.
	*/
	virtual isys::ITimer* GetTimeStamp(I_DWORD id) const = 0;

	/**
		Get access to previous processing ID's.
		\param	offset	previous offset, if it is 1 the previous will be returned, if it is 2 the previous of previous etc.
		\param	result	object stored returned ID.
		\return	true, if ID could be retrieved, false otherwise.
	*/
	virtual bool GetPreviousId(int offset, I_DWORD& result) const = 0;
};


} // namespace iproc


#endif // !iproc_IIdManager_included


