#ifndef icomp_TFactoryMember_included
#define icomp_TFactoryMember_included


#include "istd/TDelPtr.h"

#include "icomp/IComponentContext.h"
#include "icomp/TSingleAttributePtr.h"
#include "icomp/TInterfaceManipBase.h"
#include "icomp/CFactoryAttribute.h"


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TFactoryMember: public TSingleAttributePtr<CFactoryAttribute>, public TInterfaceManipBase<Interface>
{
public:
	typedef TSingleAttributePtr<CFactoryAttribute> BaseClass;
	typedef Interface InterfaceType;

	TFactoryMember();

	void Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo);

	/**
		Check if this factory can be resolved.
		Please note, that it gives no guarantee, that objects can be factorised.
	*/
	bool IsValid() const;

	/**
		Create instance of this interface.
	*/
	Interface* CreateInstance() const;

protected:
	TFactoryMember(const TFactoryMember& ptr);

private:
	const IComponent* m_definitionComponentPtr;
};


// public methods

template <class Interface>
TFactoryMember<Interface>::TFactoryMember()
:	m_definitionComponentPtr(NULL)
{
}


template <class Interface>
void TFactoryMember<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_definitionComponentPtr);
}


template <class Interface>
bool TFactoryMember<Interface>::IsValid() const
{
	return (m_definitionComponentPtr != NULL) && BaseClass::IsValid();
}



template <class Interface>
typename Interface* TFactoryMember<Interface>::CreateInstance() const
{
	if ((m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const IComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			const std::string& componentId = BaseClass::operator*();

			std::string baseId;
			std::string subId;
			SplitId(componentId, baseId, subId);
			I_ASSERT(subId.empty());	// explicit subelement ID are not implemented correctly

			IComponent* newComponnentPtr = parentPtr->CreateSubcomponent(baseId);
			if (newComponnentPtr != NULL){
				Interface* retVal = ExtractInterface(newComponnentPtr);
				if (retVal != NULL){
					return retVal;
				}

				delete newComponnentPtr;
			}
		}
	}

	return NULL;
}


// protected methods

template <class Interface>
TFactoryMember<Interface>::TFactoryMember(const TFactoryMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr)
{
}


}//namespace icomp


#endif // !icomp_TFactoryMember_included


