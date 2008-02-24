#ifndef istd_IHierarchical_included
#define istd_IHierarchical_included


#include "istd/IPolymorphic.h"


namespace istd
{		


/**
	Common interface to define the hierarchical structures.
*/
class IHierarchical: virtual public istd::IPolymorphic
{
public:
	virtual int GetChildCount() const = 0;
	virtual istd::IPolymorphic* GetChild(int index) const = 0;
	virtual istd::IHierarchical* GetParent() const = 0;
};


} // namespace istd


#endif // !istd_IHierarchical_included

