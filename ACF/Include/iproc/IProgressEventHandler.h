#ifndef iproc_IProgressEventHandler_included
#define iproc_IProgressEventHandler_included


#include "istd/IPolymorphic.h"


namespace iproc
{


class IProgressEventHandler: virtual public istd::IPolymorphic
{
public:
	/**
		Callback function for an progress event.
	*/
	virtual void OnProgress(double currentProgress) = 0;
};


} // namespace iproc


#endif // !iproc_IProgressEventHandler_included

