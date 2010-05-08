#ifndef icomp_TComponentCloneWrap_included
#define icomp_TComponentCloneWrap_included


#include "icomp/IComponent.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CComponentContext.h"


namespace icomp
{


template <class BaseClass>
class TComponentCloneWrap: virtual public BaseClass, protected icomp::CInterfaceManipBase
{
public:
	// pseudo-reimplemented (istd::IChangeable)
	virtual istd::IChangeable* CloneMe() const;
};


// pseudo-reimplemented (istd::IChangeable)
	
template <class BaseClass>
istd::IChangeable* TComponentCloneWrap<BaseClass>::CloneMe() const
{
	const icomp::CComponentContext* contextPtr = dynamic_cast<const icomp::CComponentContext*>(BaseClass::GetComponentContext());
	if (contextPtr != NULL){
		const icomp::IComponent* parentComponentPtr = BaseClass::GetParentComponent();
		if (parentComponentPtr != NULL){
			istd::TDelPtr<istd::IChangeable> clonedPtr;
			
			clonedPtr.SetCastedOrRemove(ExtractInterface<istd::IChangeable>(parentComponentPtr->CreateSubcomponent(contextPtr->GetContextId())));

			if (clonedPtr.IsValid()){
				if (clonedPtr->CopyFrom(*this)){
					return clonedPtr.PopPtr();
				}
			}
		}
	}

	return NULL;
}


} // namespace icomp


#endif //!icomp_TComponentCloneWrap_included


