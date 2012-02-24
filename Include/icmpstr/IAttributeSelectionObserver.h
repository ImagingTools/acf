#ifndef icmpstr_IAttributeSelectionObserver_included
#define icmpstr_IAttributeSelectionObserver_included


#include "icomp/IAttributeStaticInfo.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class IAttributeSelectionObserver: virtual public istd::IPolymorphic
{
public:
	virtual void OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr) = 0;
};


} // namespace icmpstr


#endif // !IAttributeSelectionObserver_included


