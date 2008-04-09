#ifndef ibase_THierarchicalBase_included
#define ibase_THierarchicalBase_included


#include "ibase/ibase.h"

#include "istd/IHierarchical.h"
#include "istd/TChangeDelegator.h"


namespace ibase
{


/**
	\ingroup ibase

	Template based implementation of a heriarchical object. 
	This class is a pseudo-implementation of istd::IHierarchical interface.
*/
template <class BaseClass>
class THierarchicalBase: public istd::TChangeDelegator<BaseClass>
{
public:
	typedef istd::TChangeDelegator<BaseClass> BaseImpl;

	THierarchicalBase();

	virtual void SetParentPtr(istd::IHierarchical* parentPtr);

	// peudeo-reimplemented (istd::IHierarchical)
	virtual int GetChildCount() const;
	virtual istd::IPolymorphic* GetChild(int index) const;
	virtual istd::IHierarchical* GetParent() const;

protected:
	istd::IHierarchical* m_parentPtr;
};


// public methods

template <class BaseClass>
THierarchicalBase<BaseClass>::THierarchicalBase()
{
	m_parentPtr = NULL;
}


template <class BaseClass>
void THierarchicalBase<BaseClass>::SetParentPtr(istd::IHierarchical* parentPtr)
{
	m_parentPtr = parentPtr;

	istd::IChangeable* changeablePtr = dynamic_cast<istd::IChangeable*>(parentPtr);
	if (changeablePtr){
		SetSlavePtr(changeablePtr);
	}
}


// peudeo-reimplemented (istd::IHierarchical)

template <class BaseClass>
int THierarchicalBase<BaseClass>::GetChildCount() const
{
	return 0;
}


template <class BaseClass>
istd::IPolymorphic* THierarchicalBase<BaseClass>::GetChild(int /*index*/) const
{
	return NULL;
}


template <class BaseClass>
istd::IHierarchical* THierarchicalBase<BaseClass>::GetParent() const
{
	return m_parentPtr;
}


} // namespace ibase


#endif // ibase_THierarchicalBase_included
