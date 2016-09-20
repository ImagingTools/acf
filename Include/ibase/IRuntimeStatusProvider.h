#ifndef ibase_IRuntimeStatusProvider_included
#define ibase_IRuntimeStatusProvider_included


// ACF includes
#include <istd/IChangeable.h>


namespace ibase
{		


/**
	Provider of the run-time status.
*/
class IRuntimeStatusProvider: virtual public istd::IChangeable
{
public:
	enum RuntimeStatus
	{
		/**
			No status
		*/
		RS_NONE,

		/**
			Process starting up.
		*/
		RS_STARTING,

		/**
			Process is running.
		*/
		RS_RUNNING,

		/**
			Process is in shutdown phase
		*/
		RS_SHUTDOWN
	};

	/**
		Get the runtime status.
	*/
	virtual RuntimeStatus GetRuntimeStatus() const = 0;
};


} // namespace ibase


#endif // !ibase_IRuntimeStatusProvider_included


