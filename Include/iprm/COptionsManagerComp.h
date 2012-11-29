#ifndef iprm_COptionsManagerComp_included
#define iprm_COptionsManagerComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iprm/COptionsManager.h"


namespace iprm
{


/**
	Implementation of selection constraints.
*/
class COptionsManagerComp:
			public icomp::CComponentBase,
			public COptionsManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(COptionsManagerComp);
		I_REGISTER_INTERFACE(iprm::ISelectionConstraints);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iprm::IOptionsManager);
		I_ASSIGN(m_slaveSelectionConstraintsCompPtr, "SlaveConstraints", "Slave constraints object", false, "SlaveConstraints");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iprm::ISelectionConstraints, m_slaveSelectionConstraintsCompPtr);
};


} // namespace iprm


#endif // !iprm_COptionsManagerComp_included


