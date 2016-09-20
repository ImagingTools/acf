#ifndef icomp_TMultiFactoryMember_included
#define icomp_TMultiFactoryMember_included


// ACF includes
#include <istd/TDelPtr.h>

#include <icomp/TMultiAttributeMember.h>
#include <icomp/CInterfaceManipBase.h>
#include <icomp/CMultiFactoryAttribute.h>


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TMultiFactoryMember: public TMultiAttributeMember<CMultiFactoryAttribute>, public CInterfaceManipBase
{
public:
	typedef TMultiAttributeMember<CMultiFactoryAttribute> BaseClass;
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
		Create component for specified index without extracting any interface.
	*/
	IComponent* CreateComponent(int index) const;

	/**
		Create instance of interface for specified index.
		This is combination of the methods \c CreateComponent and \c ExtractInterface
		provided for convinience, if only one interface is asked in factorisied objects.
	*/
	Interface* CreateInstance(int index) const;

	/**
		Extract interface from some component.
		Type of extracted interface is specified by template parameter of this class.
		If you want to force some factory to support more interfaces, you should simply define
		multiply I_FACT members with the same ID, factorise instances with any of them and extract
		specified interfaces using this method.
		\param	componentPtr	pointer to component object, typically returned by method CreateComponent().
		\param	subId			optionally ID parameter identifing subcomponent.
		\return	pointer to interface or NULL, if such interface could not be extracted.
	*/
	static Interface* ExtractInterface(IComponent* componentPtr, const QByteArray& subId = "");

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
	BaseClass::InitInternal(ownerPtr, staticInfo, &m_definitionComponentPtr);
}


template <class Interface>
bool TMultiFactoryMember<Interface>::IsValid() const
{
	return (m_definitionComponentPtr != NULL) && BaseClass::IsValid();
}



template <class Interface>
IComponent* TMultiFactoryMember<Interface>::CreateComponent(int index) const
{
	if ((m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const ICompositeComponent* parentPtr = m_definitionComponentPtr->GetParentComponent();
		if (parentPtr != NULL){
			const QByteArray& componentId = BaseClass::operator[](index);

			QByteArray baseId;
			QByteArray subId;
			BaseClass2::SplitId(componentId, baseId, subId);
			Q_ASSERT(subId.isEmpty());	// explicit subelement ID are not implemented correctly

			return parentPtr->CreateSubcomponent(baseId);
		}
		else{
			qCritical("Component %s is defined, but definition component has no parent", BaseClass::operator[](index).constData());
		}
	}

	return NULL;
}


template <class Interface>
Interface* TMultiFactoryMember<Interface>::CreateInstance(int index) const
{
	istd::TDelPtr<IComponent> newComponentPtr(CreateComponent(index));
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
Interface* TMultiFactoryMember<Interface>::ExtractInterface(IComponent* componentPtr, const QByteArray& subId)
{
	return BaseClass2::ExtractInterface<Interface>(componentPtr, subId);
}


// protected methods

template <class Interface>
TMultiFactoryMember<Interface>::TMultiFactoryMember(const TMultiFactoryMember& ptr)
:	BaseClass(ptr),
	m_definitionComponentPtr(ptr.m_definitionComponentPtr)
{
}


} // namespace icomp


#endif // !icomp_TMultiFactoryMember_included


