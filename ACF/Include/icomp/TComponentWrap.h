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
	TComponentWrap(const IComponentContext* contextPtr = NULL);
	virtual ~TComponentWrap();

protected:
	virtual void SetComponentContext(const icomp::IComponentContext* contextPtr);
};


// public methods

template <class Component>
TComponentWrap<Component>::TComponentWrap(const IComponentContext* contextPtr)
{
	if (contextPtr != NULL){
		SetComponentContext(contextPtr);
	}
}


// protected methods

template <class Component>
void TComponentWrap<Component>::SetComponentContext(const IComponentContext* contextPtr)
{
	Component::SetComponentContext(contextPtr);

	if (contextPtr != NULL){
		Component::InitStaticInfo(this);

		OnComponentCreated();
	}
}


template <class Component>
TComponentWrap<Component>::~TComponentWrap()
{
	const IComponentContext* contextPtr = GetComponentContext();
	if (contextPtr != NULL){
		OnComponentDestroyed();
	}

	SetComponentContext(NULL);
}


}//namespace icomp


#endif // !icomp_TComponentWrap_included


