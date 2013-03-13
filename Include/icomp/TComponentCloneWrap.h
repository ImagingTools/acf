#ifndef icomp_TComponentCloneWrap_included
#define icomp_TComponentCloneWrap_included


#include "icomp/IComponent.h"
#include "icomp/ICompositeComponent.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CComponentContext.h"


namespace icomp
{


template <class BaseClass>
class TComponentCloneWrap: virtual public BaseClass, protected CInterfaceManipBase
{
public:
	// pseudo-reimplemented (istd::IChangeable)
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = BaseClass::CM_WITHOUT_REFS) const;
};


// pseudo-reimplemented (istd::IChangeable)
	
template <class BaseClass>
istd::IChangeable* TComponentCloneWrap<BaseClass>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	const CComponentContext* contextPtr = dynamic_cast<const CComponentContext*>(BaseClass::GetComponentContext());
	if (contextPtr != NULL){
		const ICompositeComponent* parentComponentPtr = BaseClass::GetParentComponent();
		if (parentComponentPtr != NULL){
			istd::TDelPtr<istd::IChangeable> clonedPtr;

			clonedPtr.SetCastedOrRemove(ExtractInterface<istd::IChangeable>(parentComponentPtr->CreateSubcomponent(contextPtr->GetContextId())));

			if (clonedPtr.IsValid()){
				if (clonedPtr->CopyFrom(*this, mode)){
					return clonedPtr.PopPtr();
				}
			}
		}
	}

	return NULL;
}


} // namespace icomp


#endif //!icomp_TComponentCloneWrap_included


