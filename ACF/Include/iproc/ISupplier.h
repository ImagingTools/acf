#ifndef iproc_ISupplier_included
#define iproc_ISupplier_included


#include "iser/ISerializable.h"

#include "iprm/IParamsSet.h"

#include "iproc/iproc.h"


namespace iproc
{


/**
	Base interface for suppliers providing synchrone processing of single object.
	Derivated interfaces should provide access to intern stored produced object for specified object ID.
	For example image supplier should provide method \c{const iimg::IBitmap* GetBitmap(const I_DWORD* objectIdPtr) const}.
	The internal stored object should be created on demand.
*/
class ISupplier: virtual public iser::ISerializable
{
public:
	enum WorkStatus
	{
		/**
			No status is provided, no work was done.
		*/
		WS_NONE,
		/**
			Work was done correctly and no error occure.
		*/
		WS_OK,
		/**
			Work was canceled.
		*/
		WS_CANCELED,
		/**
			Work was done, but there were warnings.
		*/
		WS_WARNING,
		/**
			Work was done, but there were errors.
		*/
		WS_ERROR,
		/**
			Work cannot be done.
		*/
		WS_CRITICAL
	};

	enum ChangeFlags
	{
		CF_SUPPLIER_RESULTS = 0x20000
	};

	/**
		Called to signalize to entering of new object.
		Becouse of all produced object are accessible on demand and must be cached, there is needed signal to clear this cache.
		\param	objectId	ID of inspected object.
							This ID can be used to synchronize with other suppliers to ensure processing the same object.
	*/
	virtual void BeginNextObject(I_DWORD objectId) = 0;

	/**
		Ensure that all objects are produced.
		\param	objectId	ID of inspected object. \sa	BeginNextObject.
	*/
	virtual void EnsureWorkFinished(I_DWORD objectId) = 0;

	/**
		Get status of last work.
		\param	objectId	ID of inspected object. \sa	BeginNextObject.
		\return	work status defined in iproc::ISupplier::WorkStatus.
	*/
	virtual int GetWorkStatus(I_DWORD objectId) const = 0;

	/**
		Get duration time of work.
		\param	objectId	ID of inspected object. \sa	BeginNextObject.
		\return	time of duration or negative value if this time is unknown.
	*/
	virtual double GetWorkDurationTime(I_DWORD objectId) const = 0;

	/**
		Get parameter set using by this supplier.
	*/
	virtual iprm::IParamsSet* GetModelParametersSet() const = 0;
};


} // namespace iproc


#endif // !iproc_ISupplier_included


