#pragma once


// ACF includes
#include <istd/TInterfaceFactory.h>
#include <istd/TDelPtr.h>

#include <icomp/TAttributeMember.h>
#include <icomp/CInterfaceManipBase.h>
#include <icomp/CFactoryAttribute.h>


namespace icomp
{


/**
	Factory of components used as component member.
	Don't use direct this class, use macros \c I_FACT and \c I_ASSIGN instead.
*/
template <class Interface>
class TFactoryMember:
			public TAttributeMember<CFactoryAttribute>,
			public CInterfaceManipBase,
			virtual public istd::TInterfaceFactory<Interface>
{
public:
	typedef TAttributeMember<CFactoryAttribute> BaseClass;
	typedef CInterfaceManipBase BaseClass2;
	typedef Interface InterfaceType;
	typedef istd::IFactoryInfo::KeyList KeyList;

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
	IComponentUniquePtr CreateComponent() const;

	/**
		Extract interface from some component.
		Type of extracted interface is specified by template parameter of this class.
		If you want to force some factory to support more interfaces, you should simply define
		multiply I_FACT members with the same ID, factorise instances with any of them and extract
		specified interfaces using this method.
		\param	instancePtr	pointer to component object, typically returned by method CreateComponent().
				If it is NULL, this function return also NULL value.
		\param	subId			optionally ID parameter identifing subcomponent.
		\return	pointer to interface or NULL, if such interface could not be extracted.
	*/
	static Interface* ExtractInterface(istd::IPolymorphic* instancePtr, const QByteArray& subId = "");

	// reimplemented (TInterfaceFactory)
	virtual istd::TUniqueInterfacePtr<Interface> CreateInstance(const QByteArray& typeId = QByteArray()) const override;

	// reimplemented (istd::IFactoryInfo)
	virtual KeyList GetFactoryKeys() const override;

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
	BaseClass::InitInternal(ownerPtr, staticInfo, &m_definitionComponentPtr);
}


template <class Interface>
bool TFactoryMember<Interface>::IsValid() const
{
	return (m_definitionComponentPtr != NULL) && BaseClass::IsValid();
}



template <class Interface>
IComponentUniquePtr TFactoryMember<Interface>::CreateComponent() const
{
	if ((m_definitionComponentPtr != NULL) && BaseClass::IsValid()){
		const ICompositeComponent* parentPtr = dynamic_cast<const ICompositeComponent*>(m_definitionComponentPtr->GetParentComponent());
		if (parentPtr != NULL){
			const QByteArray& componentId = BaseClass::operator*();

			QByteArray baseId;
			QByteArray subId;
			BaseClass2::SplitId(componentId, baseId, subId);
			Q_ASSERT(subId.isEmpty());	// explicit subelement ID are not implemented correctly

			return parentPtr->CreateSubcomponent(baseId);
		}
		else{
			qCritical("Component %s is defined, but definition component has no parent", BaseClass::operator*().constData());
		}
	}

	return nullptr;
}


template <class Interface>
istd::TUniqueInterfacePtr<Interface> TFactoryMember<Interface>::CreateInstance(const QByteArray& /*typeId*/) const
{
	IComponentUniquePtr newComponentPtr = CreateComponent();
	if (newComponentPtr != nullptr){
		Interface* retVal = BaseClass2::ExtractInterface<Interface>(newComponentPtr.get());
		if (retVal != NULL) {
			return istd::TUniqueInterfacePtr<Interface>(newComponentPtr.release(), retVal);
		}
	}

	return istd::TUniqueInterfacePtr<Interface>();
}


// reimplemented (istd::IFactoryInfo)

template <class Interface>
istd::IFactoryInfo::KeyList TFactoryMember<Interface>::GetFactoryKeys() const
{
	static KeyList defaultList;
	if (defaultList.isEmpty()){
		defaultList << "Component";
	}

	return defaultList;
}


// static methods

template <class Interface>
Interface* TFactoryMember<Interface>::ExtractInterface(istd::IPolymorphic* instancePtr, const QByteArray& subId)
{
	if (instancePtr != NULL){
		icomp::IComponent* componentPtr = dynamic_cast<icomp::IComponent*>(instancePtr);
		Q_ASSERT(componentPtr != NULL);	// Only objects returned by \b CreateComponent should be used as input

		if (componentPtr != NULL){
			return BaseClass2::ExtractInterface<Interface>(componentPtr, subId);
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


} // namespace icomp


