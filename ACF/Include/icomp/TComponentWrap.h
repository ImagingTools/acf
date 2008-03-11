#ifndef icomp_TComponentWrap_included
#define icomp_TComponentWrap_included


#include "icomp/IComponentContext.h"


namespace icomp
{


/**
	Wrapper of end component implementation used to correct control of component life-cycle.
*/
template <class Component>
class TComponentWrap: public Component
{
public:
	TComponentWrap(const IComponentContext* contextPtr);
	virtual ~TComponentWrap();
};


// public methods

template <class Component>
TComponentWrap<Component>::TComponentWrap(const IComponentContext* contextPtr)
{
	I_ASSERT(contextPtr != NULL);

	SetComponentContext(contextPtr);

	Component::InitStaticInfo(this);

	OnComponentCreated();
}


template <class Component>
TComponentWrap<Component>::~TComponentWrap()
{
	OnComponentDestroyed();

	SetComponentContext(NULL);
}


}//namespace icomp


#endif // !icomp_TComponentWrap_included


