#ifndef istdc_TComposedFactoryComp_included
#define istdc_TComposedFactoryComp_included


#include "istd/TComposedFactory.h"

#include "icomp/CComponentBase.h"


namespace ibase
{

// TODO: check if this class should be removed from ACF project
template <class Interface>
class TComposedFactoryComp: public icomp::CComponentBase, public istd::TComposedFactory<Interface>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef istd::TComposedFactory<Interface> BaseClass2;

	I_BEGIN_COMPONENT(TComposedFactoryComp);
		I_REGISTER_INTERFACE(FactoryInterface);
		I_ASSIGN_MULTI_0(m_slaveFactoriesCompPtr, "SlaveFactories", "Slave factories", true);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	I_MULTIREF(FactoryInterface, m_slaveFactoriesCompPtr);
};


// reimplemented (icomp::IComponent)

template <class Interface>
void TComposedFactoryComp<Interface>::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int slaveFactoriesCount = m_slaveFactoriesCompPtr.GetCount();

	for (int factoryIndex = 0; factoryIndex < slaveFactoriesCount; ++factoryIndex){
		I_ASSERT(m_slaveFactoriesCompPtr.IsValid(factoryIndex));	// isObligatory was set to true

		RegisterFactory(m_slaveFactoriesCompPtr[factoryIndex]);
	}
}


} // namespace ibase


#endif // !istdc_TComposedFactoryComp_included


