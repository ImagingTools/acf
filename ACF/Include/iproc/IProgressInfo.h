#ifndef iproc_IProgessInfo_included
#define iproc_IProgessInfo_included


#include "istd/IPolymorphic.h"


namespace iproc
{


/**
	Common inteface for processing progress.
*/
class IProgessInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Returns processing progress.
	*/
	virtual double GetProgress() const = 0;
};


} // namespace iproc


#endif // !iproc_IProgessInfo_included

