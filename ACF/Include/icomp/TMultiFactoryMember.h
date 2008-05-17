#ifndef icomp_TMultiFactoryMember_included
#define icomp_TMultiFactoryMember_included


#include "istd/TDelPtr.h"

#include "icomp/IComponentContext.h"
#include "icomp/TMultiAttributePtr.h"
#include "icomp/CMultiFactoryAttribute.h"


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TMultiFactoryMember: public TMultiAttributePtr<CMultiFactoryAttribute>
{
public:
	typedef TSingleAttributePtr<CFactoryAttribute> BaseClass;
	typedef Interface InterfaceType;

	TMultiFactoryMember();

	void Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo);

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
	const IComponentContext* m_realContextPtr;
};


// public methods

template <class Interface>
TMultiFactoryMember<Interface>::TMultiFactoryMember()
:	m_realContextPtr(NULL)
{
}


template <class Interface>
void TMultiFactoryMember<Interface>::Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_realContextPtr);
}


template <class Interface>
bool TMultiFactoryMember<Interface>::IsValid() const
{
	return (m_realContextPtr != NULL) && BaseClass::IsValid();
}



template <class Interface>
typename Interface* TMultiFactoryMember<Interface>::CreateInstance(int index) const
{
	istd::TDelPtr<Interface> retVal;

	if ((m_realContextPtr != NULL) && BaseClass::IsValid()){
		const IComponentContext* parentPtr = m_realContextPtr->GetParentContext();
		if (parentPtr != NULL){
			const std::string& componentId = BaseClass::operator[](index);

			retVal.SetCastedOrRemove(parentPtr->CreateSubcomponent(componentId));
		}
	}

	return retVal.PopPtr();
}


// protected methods

template <class Interface>
TMultiFactoryMember<Interface>::TMultiFactoryMember(const TMultiFactoryMember& ptr)
:	BaseClass(ptr),
	m_realContextPtr(ptr.m_realContextPtr)
{
}


}//namespace icomp


#endif // !icomp_TMultiFactoryMember_included


