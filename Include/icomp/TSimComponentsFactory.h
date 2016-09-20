#ifndef icomp_TSimComponentsFactory_included
#define icomp_TSimComponentsFactory_included


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
class TSimComponentsFactory: public TSimComponentWrap<Base>, public istd::TIFactory<icomp::IComponent>
{
public:
	typedef TSimComponentWrap<Base> BaseClass;

	TSimComponentsFactory();

	// reimplemented (istd::IFactoryInfo)
	virtual istd::IFactoryInfo::KeyList GetFactoryKeys() const;

	// reimplemented (istd::TIFactory<icomp::IComponent>)
	virtual icomp::IComponent* CreateInstance(const QByteArray& keyId = "") const;

	// reimplemented (icomp::ICompositeComponent)
	virtual IComponent* CreateSubcomponent(const QByteArray& componentId) const;

private:
	KeyList m_factoryKeys;
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
		if (retVal != NULL){
			retVal->SetComponentContext(this, this, false);
			return retVal;
		}
	}

	return NULL;
}


// reimplemented (icomp::ICompositeComponent)

template <class Base>
IComponent* TSimComponentsFactory<Base>::CreateSubcomponent(const QByteArray& componentId) const
{
	IComponent* componentPtr = BaseClass::CreateSubcomponent(componentId);
	if (componentPtr != NULL){
		return componentPtr;
	}

	return CreateInstance(componentId);
}


} // namespace icomp


#endif // !icomp_TSimComponentsFactory_included


