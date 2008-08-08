#ifndef icomp_TSimComponentsFactory_included
#define icomp_TSimComponentsFactory_included


#include "istd/TSingleFactory.h"

#include "icomp/IComponent.h"
#include "icomp/TComponentWrap.h"
#include "icomp/TSimComponentWrap.h"


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.
*/
template <class Base>
class TSimComponentsFactory: public TSimComponentWrap<Base>, public istd::TIFactory<icomp::IComponent>
{
public:
	typedef TSimComponentWrap<Base> BaseClass;

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
		icomp::IComponent* retVal = new TComponentWrap<Base>();
		if (retVal != NULL){
			retVal->SetComponentContext(this, this, false);
			return retVal;
		}
	}

	return NULL;
}


}//namespace icomp


#endif // !icomp_TSimComponentsFactory_included


