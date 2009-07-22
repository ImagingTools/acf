#ifndef icomp_TFactoryMember_included
#define icomp_TFactoryMember_included


#include "istd/TDelPtr.h"

#include "icomp/IComponentContext.h"
#include "icomp/TSingleAttributePtr.h"
#include "icomp/CInterfaceManipBase.h"
#include "icomp/CFactoryAttribute.h"


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TFactoryMember: public TSingleAttributePtr<CFactoryAttribute>, public CInterfaceManipBase
{
public:
	typedef TSingleAttributePtr<CFactoryAttribute> BaseClass;
	typedef CInterfaceManipBase BaseClass2;
	typedef Interface InterfaceType;

	TFactoryMember();

	void Init(const IComponent* ownerPtr, const IRealAttributeStaticInfo& staticInfo);

	/**
		Check if this factory can be resolved.
		Please note, that it gives no guarantee, that objects can be factorised.
	*/
	bool IsValid() const;

	/**
		Create component without extracting any interface.
	*/
	IComponent* CreateComponent() const;

	/**
		Create instance of this interface.
		This is combination of the methods CreateComponent() and ExtractInterface()
		provided for convinience, if only one interface is asked in factorisied objects.
	*/
	Interface* CreateInstance() const;

	/**
		Extract interface from some component.
		Type of extracted interface is specified by template parameter of this class.
		If you want to force some factory to support more interfaces, you should simply define
		multiply I_FACT members with the same ID, factorise instances with any of them and extract
		specified interfaces using this method.
		@param	componentPtr	pointer to component object, typically returned by method CreateComponent().
		@param	subId			optionally ID parameter identifing subcomponent.
		@return	pointer to interface or NULL, if such interface could not be extracted.
	*/
	static Interface* ExtractInterface(IComponent* componentPtr, const std::string& subId = "");

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
IComponent* TFactoryMember<Interface>::CreateComponent() const
{
	if ((m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const IComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			const std::string& componentId = BaseClass::operator*();

			std::string baseId;
			std::string subId;
			BaseClass2::SplitId(componentId, baseId, subId);
			I_ASSERT(subId.empty());	// explicit subelement ID are not implemented correctly

			return parentPtr->CreateSubcomponent(baseId);
		}
	}

	return NULL;
}


template <class Interface>
Interface* TFactoryMember<Interface>::CreateInstance() const
{
	istd::TDelPtr<IComponent> newComponentPtr(CreateComponent());
	if (newComponentPtr.IsValid()){
		Interface* retVal = BaseClass2::ExtractInterface<Interface>(newComponentPtr.GetPtr());
		if (retVal != NULL){
			newComponentPtr.PopPtr();

			return retVal;
		}
	}

	return NULL;
}


// static methods

template <class Interface>
Interface* TFactoryMember<Interface>::ExtractInterface(IComponent* componentPtr, const std::string& subId)
{
	return BaseClass2::ExtractInterface<Interface>(componentPtr, subId);
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


