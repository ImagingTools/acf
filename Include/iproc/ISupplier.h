#ifndef iproc_ISupplier_included
#define iproc_ISupplier_included


#include "istd/IChangeable.h"

#include "iprm/IParamsSet.h"

#include "iproc/iproc.h"


namespace iproc
{


/**
	Base interface for suppliers providing synchrone processing of single object with <em>"pull data" model</em>.
	Additional interfaces (providers) should provide access to intern stored produced objects.
	For example image provider should provide method \c{const iimg::IBitmap* GetBitmap(const I_DWORD* objectIdPtr) const}.
	The internal stored object should be created on demand.
*/
class ISupplier: virtual public istd::IChangeable
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
			Supplier is locked becouse it is doing processing step.
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
		CF_SUPPLIER_RESULTS = 1 << 21
	};

	/**
		Called to signalize that this supplier is invalid.
		This signal will be transfered to all supplier which are registered as output.
	*/
	virtual void InvalidateSupplier() = 0;

	/**
		Ensure that all objects are produced.
	*/
	virtual void EnsureWorkFinished() = 0;

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
		Get parameter set using by this supplier.
	*/
	virtual iprm::IParamsSet* GetModelParametersSet() const = 0;
};


} // namespace iproc


#endif // !iproc_ISupplier_included


