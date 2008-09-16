#ifndef istd_TIHierarchical_included
#define istd_TIHierarchical_included


#include "istd/IPolymorphic.h"


namespace istd
{		


/**
	Common interface to define the hierarchical structures.
*/
template <class Interface>
class TIHierarchical: virtual public Interface
{
public:
	typedef Interface InterfaceType;

	virtual int GetChildsCount() const = 0;
	virtual Interface* GetChild(int index) const = 0;
	virtual Interface* GetParent() const = 0;
};


typedef TIHierarchical<IPolymorphic> IHierarchical;


} // namespace istd


#endif // !istd_TIHierarchical_included

