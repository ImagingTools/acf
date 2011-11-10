#ifndef iprm_CNameParamComp_included
#define iprm_CNameParamComp_included


#include "icomp/CComponentBase.h"

#include "ibase/TNamedWrap.h"


namespace iprm
{


/**
	Component for automatic creation of the Universally Unique Identifier (UUID)
*/
class CNameParamComp:
				public icomp::CComponentBase,
				public ibase::CNamed,
				virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CNameParamComp);
		I_REGISTER_INTERFACE(istd::INamed);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_defaultNameAttrPtr, "DefaultName", "Default name", false, "");
	I_END_COMPONENT;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(istd::CString, m_defaultNameAttrPtr);
};


} // namespace ibase


#endif // !iprm_CNameParamComp_included


