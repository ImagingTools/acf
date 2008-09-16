#ifndef icomp_TSimComponentWrap_included
#define icomp_TSimComponentWrap_included


#include "icomp/TComponentWrap.h"
#include "icomp/CSimComponentContextBase.h"


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.
*/
template <class Base>
class TSimComponentWrap: public TComponentWrap<Base>, public CSimComponentContextBase
{
public:
	typedef TComponentWrap<Base> BaseClass;
	typedef CSimComponentContextBase BaseClass2;

	TSimComponentWrap();

	/**
		Initialilze component after setting all its attributes and references.
	*/
	void InitComponent();

	// pseudo-reimplemented (icomp::IComponent)
	virtual const IComponent* GetParentComponent(bool ownerOnly = false) const;
	virtual IComponent* GetSubcomponent(const std::string& componentId) const;
	virtual IComponent* CreateSubcomponent(const std::string& componentId) const;
};


// public methods

template <class Base>
TSimComponentWrap<Base>::TSimComponentWrap()
:	BaseClass(), BaseClass2(&InitStaticInfo(NULL))
{
}


template <class Base>
void TSimComponentWrap<Base>::InitComponent()
{
	SetComponentContext(this, NULL, false);
}


template <class Base>
const IComponent* TSimComponentWrap<Base>::GetParentComponent(bool ownerOnly) const
{
	if (ownerOnly){
		return NULL;
	}
	else{
		return const_cast<TSimComponentWrap<Base>* >(this);
	}
}


template <class Base>
IComponent* TSimComponentWrap<Base>::GetSubcomponent(const std::string& componentId) const
{
	BaseClass2::ComponentsMap::const_iterator iter = BaseClass2::m_componentsMap.find(componentId);

	if (iter != BaseClass2::m_componentsMap.end()){
		return iter->second;
	}

	return NULL;
}


template <class Base>
IComponent* TSimComponentWrap<Base>::CreateSubcomponent(const std::string& componentId) const
{
	FactoriesMap::const_iterator iter = m_factoriesMap.find(componentId);
	if (iter != m_factoriesMap.end()){
		I_ASSERT(iter->second != NULL);

		return iter->second->CreateInstance();
	}

	return NULL;
}


}//namespace icomp


#endif // !icomp_TSimComponentWrap_included


