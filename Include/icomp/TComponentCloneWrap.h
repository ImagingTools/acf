#ifndef icomp_TComponentCloneWrap_included
#define icomp_TComponentCloneWrap_included


#include "icomp/IComponent.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CComponentContext.h"
#include "icomp/CCompositeComponent.h"
#include "icomp/CCompositeComponentContext.h"


namespace icomp
{


template <class BaseClass>
class TComponentCloneWrap: virtual public BaseClass, protected CInterfaceManipBase
{
public:
	// pseudo-reimplemented (istd::IChangeable)
	virtual istd::IChangeable* CloneMe(istd::IChangeable::CompatibilityMode mode = istd::IChangeable::CM_WITHOUT_REFS) const;
};


// pseudo-reimplemented (istd::IChangeable)

template <class BaseClass>
istd::IChangeable* TComponentCloneWrap<BaseClass>::CloneMe(istd::IChangeable::CompatibilityMode mode) const
{
	const CComponentContext* contextPtr = dynamic_cast<const CComponentContext*>(BaseClass::GetComponentContext());
	if (contextPtr != NULL){
		QByteArray contextId = contextPtr->GetContextId();

		const ICompositeComponent* parentComponentPtr = BaseClass::GetParentComponent();
		if (parentComponentPtr != NULL){
			if ((mode == istd::IChangeable::CM_WITH_REFS) || (mode == istd::IChangeable::CM_CONVERT)){
				// we have to check if our owner has a parent composite component.
				// In this case we have to factorize not our component, but the complete parent composite component:
				const CCompositeComponent* parentCompositeComponentPtr = dynamic_cast<const CCompositeComponent*>(parentComponentPtr->GetParentComponent());
				if (parentCompositeComponentPtr != NULL){
					const CCompositeComponentContext* contextPtr = dynamic_cast<const CCompositeComponentContext*>(parentCompositeComponentPtr->GetComponentContext());

					const IRegistry& registry = contextPtr->GetRegistry();
					IRegistry::Ids elementIds = registry.GetElementIds();

					for (IRegistry::Ids::ConstIterator elemIter = elementIds.constBegin(); elemIter != elementIds.constEnd(); ++elemIter){
						const icomp::ICompositeComponent* subComponentPtr = dynamic_cast<const icomp::ICompositeComponent*>(parentCompositeComponentPtr->GetSubcomponent(*elemIter));
						if (subComponentPtr != NULL){
							const icomp::IComponent* sourceComponentPtr = subComponentPtr->GetSubcomponent(contextId);
							bool isCompostite = (dynamic_cast<const icomp::ICompositeComponent*>(sourceComponentPtr) != NULL);
							if (isCompostite && (sourceComponentPtr != NULL)){
								contextId = *elemIter;

								parentComponentPtr = parentCompositeComponentPtr;
								break;
							}
						}
					}
				}
			}

			istd::TDelPtr<istd::IChangeable> clonedPtr;

			clonedPtr.SetCastedOrRemove(ExtractInterface<istd::IChangeable>(parentComponentPtr->CreateSubcomponent(contextId)));

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


