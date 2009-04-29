#ifndef icomp_TMultiFactoryMember_included
#define icomp_TMultiFactoryMember_included


#include "istd/TDelPtr.h"

#include "icomp/IComponentContext.h"
#include "icomp/TMultiAttributePtr.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TMultiFactoryMember: public TMultiAttributePtr<CMultiFactoryAttribute>, public CInterfaceManipBase
{
public:
	typedef TMultiAttributePtr<CFactoryAttribute> BaseClass;
	typedef CInterfaceManipBase BaseClass2;
	typedef Interface InterfaceType;

	TMultiFactoryMember();

	void Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo);

	/**
		Check if this factory can be resolved.
		Please note, that it gives no guarantee, that objects can be factorised.
	*/
	bool IsValid() const;

	/**
		Create instance of interface for specified index.
	*/
	Interface* CreateInstance(int index) const;

protected:
	TMultiFactoryMember(const TMultiFactoryMember& ptr);

private:
	const IComponent* m_definitionComponentPtr;
};


// public methods

template <class Interface>
TMultiFactoryMember<Interface>::TMultiFactoryMember()
:	m_definitionComponentPtr(NULL)
{
}


template <class Interface>
void TMultiFactoryMember<Interface>::Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_definitionComponentPtr);
}


template <class Interface>
bool TMultiFactoryMember<Interface>::IsValid() const
{
	return (m_definitionComponentPtr != NULL) && BaseClass::IsValid();
}



template <class Interface>
Interface* TMultiFactoryMember<Interface>::CreateInstance(int index) const
{
	if ((m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const IComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			const std::string& componentId = BaseClass::operator[](index);

			std::string baseId;
			std::string subId;
			BaseClass2::SplitId(componentId, baseId, subId);
			I_ASSERT(subId.empty());	// explicit subelement ID are not implemented correctly

			IComponent* newComponnentPtr = parentPtr->CreateSubcomponent(baseId);
			if (newComponnentPtr != NULL){
				Interface* retVal = BaseClass2::ExtractInterface<Interface>(newComponnentPtr);
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
TMultiFactoryMember<Interface>::TMultiFactoryMember(const TMultiFactoryMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr)
{
}


}//namespace icomp


#endif // !icomp_TMultiFactoryMember_included


