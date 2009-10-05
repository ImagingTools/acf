#ifndef iprm_CEnableableParamComp_included
#define iprm_CEnableableParamComp_included


#include "istd/IEnableable.h"

#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"


namespace iprm
{


class CEnableableParamComp:
			public icomp::CComponentBase,
			virtual public istd::IEnableable,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CEnableableParamComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(istd::IEnableable);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_defaultIsEnabledAttrPtr, "DefaultEnabled", "Default value", true, false);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (istd::IEnableable)
	virtual bool IsEnabled() const;
	virtual bool IsEnablingAllowed() const;
	virtual void SetEnabled(bool isEnabled = true);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	bool m_isEnabled;

	I_ATTR(bool, m_defaultIsEnabledAttrPtr);
	I_ATTR(int, m_pathTypeAttrPtr);
};


} // namespace iprm


#endif // !iprm_CEnableableParamComp_included


