#ifndef imebase_CChannelAddressParamComp_included
#define imebase_CChannelAddressParamComp_included


#include "icomp/CComponentBase.h"

#include "IMeilhausAccess.h"
#include "CChannelSelectionNode.h"


namespace imebase
{


class CChannelAddressParamComp: public icomp::CComponentBase, public CChannelSelectionNode
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CChannelAddressParamComp)
		I_REGISTER_INTERFACE(iser::ISerializable)
		I_REGISTER_INTERFACE(iprm::ISelectionParam)
		I_ASSIGN(m_mailhausAccessCompPtr, "MeilhausAccess", "Allows to read set of possible options based on connected Me hardware", true, "MeilhausAccess")
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(IMeilhausAccess, m_mailhausAccessCompPtr);
};

	
} // namespace imebase


#endif // !imebase_CChannelAddressParamComp_included


