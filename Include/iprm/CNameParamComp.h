#ifndef iprm_CNameParamComp_included
#define iprm_CNameParamComp_included


// ACF includes
#include "icomp/CComponentBase.h"

#include "iprm/CNameParam.h"


namespace iprm
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID)
*/
class CNameParamComp:
				public icomp::CComponentBase,
				public iprm::CNameParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CNameParamComp);
		I_REGISTER_INTERFACE(iprm::INameParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_defaultNameAttrPtr, "DefaultName", "Default name", false, "");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(QString, m_defaultNameAttrPtr);
};


} // namespace ibase


#endif // !iprm_CNameParamComp_included


