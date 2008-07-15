#ifndef icomp_TSimComponentsFactory_included
#define icomp_TSimComponentsFactory_included


#include "istd/TSingleFactory.h"

#include "icomp/IComponent.h"
#include "icomp/TComponentWrap.h"
#include "icomp/CSimComponentContextBase.h"


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.
*/
template <class Base>
class TSimComponentsFactory: public CSimComponentContextBase, public istd::TIFactory<icomp::IComponent>
{
public:
	typedef CSimComponentContextBase BaseClass;

	TSimComponentsFactory();

	// reimplemented (istd::TIFactory<icomp::IComponent>)
	virtual KeyList GetFactoryKeys() const;
	virtual icomp::IComponent* CreateInstance(const std::string& keyId = "") const;

private:
	KeyList m_factoryKeys;
};


// public methods

template <class Base>
TSimComponentsFactory<Base>::TSimComponentsFactory()
:	BaseClass(&Base::InitStaticInfo(NULL))
{
	m_factoryKeys.push_back(typeid(Base).name());
}


// reimplemented (istd::TIFactory<icomp::IComponent>)

template <class Base>
istd::TIFactory<icomp::IComponent>::KeyList TSimComponentsFactory<Base>::GetFactoryKeys() const
{
	return m_factoryKeys;
}


template <class Base>
icomp::IComponent* TSimComponentsFactory<Base>::CreateInstance(const std::string& keyId) const
{
	if (keyId.empty() || (keyId == m_factoryKeys.back())){
		return new TComponentWrap<Base>(this);
	}
	else{
		return NULL;
	}
}


}//namespace icomp


#endif // !icomp_TSimComponentsFactory_included


