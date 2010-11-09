#ifndef iproc_ISupplier_included
#define iproc_ISupplier_included


#include "iser/ISerializable.h"

#include "iprm/IParamsSet.h"

#include "iproc/iproc.h"


namespace iproc
{


/**
	Base interface for suppliers providing synchrone processing of single object with <em>"pull data" model</em>.
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
			New work is initialized.
		*/
		WS_INIT,
		/**
			Supplier is locked.
			This state can occured during processing.
		*/
		WS_LOCKED,
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
		Because of all produced object are accessible on demand and must be cached, there is needed signal to clear this cache.
		\param	thisOnly	normally dependent suppliers will be initialized,
							but if this flag is true only this single supplier will be initilized.
	*/
	virtual void InitNewWork(bool thisOnly = false) = 0;

	/**
		Ensure that all objects are produced.
	*/
	virtual void EnsureWorkFinished(const istd::IPolymorphic* outputObjectPtr = NULL) = 0;

	/**
		Remove all stored work results.
		This set also work state to \c WS_INIT.
	*/
	virtual void ClearWorkResults() = 0;

	/**
		Get status of last work.
		\return	work status defined in iproc::ISupplier::WorkStatus.
	*/
	virtual int GetWorkStatus() const = 0;

	/**
		Get duration time of work.
		\return	time of duration or negative value if this time is unknown.
	*/
	virtual double GetWorkDurationTime() const = 0;

	/**
		Get parameter set using by this supplier.
	*/
	virtual iprm::IParamsSet* GetModelParametersSet() const = 0;

	/**
		Register output object for unanonymous supplying.
	*/
	virtual void OnOutputSubscribed(istd::IPolymorphic* outputObjectPtr) = 0;
	/**
		Unregister output object for unanonymous supplying.
	*/
	virtual void OnOutputUnsubscribed(const istd::IPolymorphic* outputObjectPtr) = 0;
};


} // namespace iproc


#endif // !iproc_ISupplier_included


