#ifndef icomp_TFactoryMember_included
#define icomp_TFactoryMember_included


#include "istd/TDelPtr.h"

#include "icomp/IComponentContext.h"
#include "icomp/TSingleAttributePtr.h"
#include "icomp/CFactoryAttribute.h"


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TFactoryMember: public TSingleAttributePtr<CFactoryAttribute>
{
public:
	typedef TSingleAttributePtr<CFactoryAttribute> BaseClass;
	typedef Interface InterfaceType;

	TFactoryMember();

	void Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo);

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
	const IComponentContext* m_realContextPtr;
};


// public methods

template <class Interface>
TFactoryMember<Interface>::TFactoryMember()
:	m_realContextPtr(NULL)
{
}


template <class Interface>
void TFactoryMember<Interface>::Init(const IComponent* ownerPtr, const IAttributeStaticInfo& staticInfo)
{
	BaseClass::Init(ownerPtr, staticInfo, &m_realContextPtr);
}


template <class Interface>
bool TFactoryMember<Interface>::IsValid() const
{
	return (m_realContextPtr != NULL) && BaseClass::IsValid();
}



template <class Interface>
typename Interface* TFactoryMember<Interface>::CreateInstance() const
{
	istd::TDelPtr<Interface> retVal;

	if ((m_realContextPtr != NULL) && BaseClass::IsValid()){
		const IComponentContext* parentPtr = m_realContextPtr->GetParentContext();
		if (parentPtr != NULL){
			const std::string& componentId = BaseClass::operator*();

			retVal.SetCastedOrRemove(parentPtr->CreateSubcomponent(componentId));
		}
	}

	return retVal.PopPtr();
}


// protected methods

template <class Interface>
TFactoryMember<Interface>::TFactoryMember(const TFactoryMember& ptr)
:	BaseClass(ptr),
	m_realContextPtr(ptr.m_realContextPtr)
{
}


}//namespace icomp


#endif // !icomp_TFactoryMember_included


