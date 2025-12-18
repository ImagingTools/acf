#pragma once


// ACF includes
#include <istd/TIFactory.h>
#include <istd/CClassInfo.h>
#include <icomp/IComponent.h>
#include <icomp/TComponentWrap.h>
#include <icomp/TSimComponentWrap.h>


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.

	\ingroup ComponentConcept
*/
template <class Base>
class TSimComponentsFactory: public istd::TIFactory<icomp::IComponent>, public icomp::ICompositeComponent
{
public:
	TSimComponentsFactory();

	inline TSimComponentWrap<Base>* operator ->()
	{
		return &m_base;
	}

	// reimplemented (istd::IFactoryInfo)
	virtual istd::IFactoryInfo::KeyList GetFactoryKeys() const override;

	// reimplemented (istd::TIFactory<icomp::IComponent>)
	virtual icomp::IComponent* CreateInstance(const QByteArray& keyId = "") const override;

	// reimplemented (icomp::ICompositeComponent)
	virtual IComponentSharedPtr GetSubcomponent(const QByteArray& componentId) const override;
	virtual IComponentContextSharedPtr GetSubcomponentContext(const QByteArray& componentId) const override;
	virtual IComponentUniquePtr CreateSubcomponent(const QByteArray& componentId) const override;
	virtual void OnSubcomponentDeleted(const IComponent* subcomponentPtr) override;

	// reimplemented (icomp::IComponent)
	virtual const icomp::IComponent* GetParentComponent(bool ownerOnly = false) const override;
	virtual void* GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId = "") override;
	virtual icomp::IComponentContextSharedPtr GetComponentContext() const override;
	virtual void SetComponentContext(
				const icomp::IComponentContextSharedPtr& contextPtr,
				const icomp::IComponent* parentPtr,
				bool isParentOwner) override;
private:
	KeyList m_factoryKeys;

	TSimComponentWrap<Base> m_base;
};


// public methods

template <class Base>
TSimComponentsFactory<Base>::TSimComponentsFactory()
{
	m_factoryKeys.insert(istd::CClassInfo::GetName<Base>());
}

// reimplemented (istd::IFactoryInfo)

template <class Base>
istd::IFactoryInfo::KeyList TSimComponentsFactory<Base>::GetFactoryKeys() const
{
	return m_factoryKeys;
}


// reimplemented (istd::TIFactory<icomp::IComponent>)

template <class Base>
icomp::IComponent* TSimComponentsFactory<Base>::CreateInstance(const QByteArray& keyId) const
{
	if (keyId.isEmpty() || m_factoryKeys.contains(keyId)){
		icomp::IComponent* retVal = new TComponentWrap<Base>();
		if (retVal != nullptr){
			retVal->SetComponentContext(m_base.GetComponentContext(), this, false);

			return retVal;
		}
	}

	return nullptr;
}


// reimplemented (icomp::ICompositeComponent)

template <class Base>
icomp::IComponentSharedPtr TSimComponentsFactory<Base>::GetSubcomponent(const QByteArray& componentId) const
{
	return m_base.GetSubcomponent(componentId);
}


template <class Base>
icomp::IComponentContextSharedPtr TSimComponentsFactory<Base>::GetSubcomponentContext(const QByteArray& componentId) const
{
	return m_base.GetSubcomponentContext(componentId);
}


template <class Base>
icomp::IComponentUniquePtr TSimComponentsFactory<Base>::CreateSubcomponent(const QByteArray& componentId) const
{
	IComponentUniquePtr componentPtr = m_base.CreateSubcomponent(componentId);
	if (componentPtr != NULL){
		return componentPtr;
	}

	icomp::IComponentUniquePtr retVal;
	
	retVal.reset(CreateInstance(componentId));

	return retVal;
}


template <class Base>
void TSimComponentsFactory<Base>::OnSubcomponentDeleted(const icomp::IComponent* subcomponentPtr)
{
	m_base.OnSubcomponentDeleted(subcomponentPtr);
}


// reimplemented (icomp::IComponent)

template <class Base>
const icomp::IComponent* TSimComponentsFactory<Base>::GetParentComponent(bool ownerOnly) const
{
	return m_base.GetParentComponent(ownerOnly);
}


template <class Base>
void* TSimComponentsFactory<Base>::GetInterface(const istd::CClassInfo& interfaceType, const QByteArray& subId)
{
	return m_base.GetInterface(interfaceType, subId);
}


template <class Base>
icomp::IComponentContextSharedPtr TSimComponentsFactory<Base>::GetComponentContext() const
{
	return m_base.GetComponentContext();
}


template <class Base>
void TSimComponentsFactory<Base>::SetComponentContext(
				const icomp::IComponentContextSharedPtr& contextPtr,
				const icomp::IComponent* parentPtr,
				bool isParentOwner)
{
	m_base.SetComponentContext(contextPtr, parentPtr, isParentOwner);
}


} // namespace icomp


