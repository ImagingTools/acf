#ifndef ibase_THierarchicalBase_included
#define ibase_THierarchicalBase_included


#include "ibase/ibase.h"

#include "istd/TIHierarchical.h"
#include "istd/TChangeDelegator.h"


namespace ibase
{


/**
	\ingroup ibase

	Template based implementation of a heriarchical object. 
	This class is a pseudo-implementation of istd::IHierarchical interface.
*/
template <class Base, class Interface = Base::InterfaceType>
class THierarchicalBase: public istd::TChangeDelegator<Base>
{
public:
	typedef istd::TChangeDelegator<Base> BaseClass;

	THierarchicalBase();

	virtual void SetParentPtr(Interface* parentPtr);

	// pseudo-reimplemented (istd::TIHierarchical<Interface>)
	virtual int GetChildsCount() const;
	virtual Interface* GetChild(int index) const;
	virtual Interface* GetParent() const;

protected:
	Interface* m_parentPtr;
};


// public methods

template <class Base, class Interface>
THierarchicalBase<Base, Interface>::THierarchicalBase()
{
	m_parentPtr = NULL;
}


template <class Base, class Interface>
void THierarchicalBase<Base, Interface>::SetParentPtr(Interface* parentPtr)
{
	m_parentPtr = parentPtr;

	istd::IChangeable* changeablePtr = dynamic_cast<istd::IChangeable*>(parentPtr);
	if (changeablePtr){
		SetSlavePtr(changeablePtr);
	}
}


// peudeo-reimplemented (istd::TIHierarchical<Interface>)

template <class Base, class Interface>
int THierarchicalBase<Base, Interface>::GetChildsCount() const
{
	return 0;
}


template <class Base, class Interface>
typename Interface* THierarchicalBase<Base, Interface>::GetChild(int /*index*/) const
{
	return NULL;
}


template <class Base, class Interface>
typename Interface* THierarchicalBase<Base, Interface>::GetParent() const
{
	return m_parentPtr;
}


} // namespace ibase


#endif // ibase_THierarchicalBase_included


