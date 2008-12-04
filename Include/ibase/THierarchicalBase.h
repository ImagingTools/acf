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
	This class is a pseudo-implementation of istd::TIHierarchical template interface.
*/
template <class Base, class Interface = typename Base::InterfaceType>
class THierarchicalBase: public istd::TChangeDelegator<Base>
{
public:
	typedef istd::TChangeDelegator<Base> BaseClass;

	THierarchicalBase();

	virtual void SetParentPtr(Interface* parentPtr);

	// pseudo-reimplemented (istd::TIHierarchical<Interface>)
	virtual int GetHierarchicalFlags() const;
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
		BaseClass::SetSlavePtr(changeablePtr);
	}
}


// peudeo-reimplemented (istd::TIHierarchical<Interface>)

template <class Base, class Interface>
int THierarchicalBase<Base, Interface>::GetHierarchicalFlags() const
{
	return BaseClass::HF_CHILDS_SUPPORTED | BaseClass::HF_PARENT_SUPPORTED;
}


template <class Base, class Interface>
int THierarchicalBase<Base, Interface>::GetChildsCount() const
{
	return 0;
}


template <class Base, class Interface>
Interface* THierarchicalBase<Base, Interface>::GetChild(int /*index*/) const
{
	return NULL;
}


template <class Base, class Interface>
Interface* THierarchicalBase<Base, Interface>::GetParent() const
{
	return m_parentPtr;
}


} // namespace ibase


#endif // ibase_THierarchicalBase_included


