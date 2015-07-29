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
		I_REGISTER_INTERFACE(iprm::IOptionsList);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(iprm::IOptionsManager);
		I_ASSIGN(m_fixedOptionsListCompPtr, "FixedOptionsList", "List of options could not be changed using this manager", false, "FixedOptionsList");
		I_ASSIGN(m_serializeSelectionAttrPtr, "SerializeSelection", "If enabled, the current parameter set selection will be serialized", true, true);
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(iprm::IOptionsList, m_fixedOptionsListCompPtr);
	I_ATTR(bool, m_serializeSelectionAttrPtr);
};


} // namespace iprm


#endif // !iprm_COptionsManagerComp_included


