#ifndef IAttributeSelectionObserver_included
#define IAttributeSelectionObserver_included


#include "istd/CString.h"

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


