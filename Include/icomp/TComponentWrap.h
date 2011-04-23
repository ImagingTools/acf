#ifndef icomp_TComponentWrap_included
#define icomp_TComponentWrap_included


#include "icomp/IComponent.h"
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
	typedef Component BaseClass;

	TComponentWrap();
	virtual ~TComponentWrap();

	// pseudo-reimplemented (icomp::IComponent)
	virtual void SetComponentContext(
				const IComponentContext* contextPtr,
				const IComponent* parentPtr,
				bool isParentOwner);
};


// public methods

template <class Component>
TComponentWrap<Component>::TComponentWrap()
{
}


template <class Component>
TComponentWrap<Component>::~TComponentWrap()
{
	IComponent* parentPtr = const_cast<IComponent*>(BaseClass::GetParentComponent(true));
	if (parentPtr != NULL){
		parentPtr->OnSubcomponentDeleted(this);
	}

	this->SetComponentContext(NULL, NULL, false);
}


// pseudo-reimplemented (icomp::IComponent)

template <class Component>
void TComponentWrap<Component>::SetComponentContext(
			const IComponentContext* contextPtr,
			const IComponent* parentPtr,
			bool isParentOwner)
{
	if (BaseClass::GetComponentContext() != NULL){
		BaseClass::OnComponentDestroyed();
	}

	BaseClass::SetComponentContext(contextPtr, parentPtr, isParentOwner);

	if (contextPtr != NULL){
		BaseClass::InitStaticInfo(this);

		BaseClass::OnComponentCreated();
	}
}


}//namespace icomp


#endif // !icomp_TComponentWrap_included


