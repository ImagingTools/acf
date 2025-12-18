#pragma once


// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <icomp/CComponentBase.h>
#include <iprm/IEnableableParam.h>


namespace iprm
{


class CDelegatedEnableableParamComp:
			public icomp::CComponentBase,
			public iprm::IEnableableParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDelegatedEnableableParamComp);
		I_REGISTER_INTERFACE(iprm::IEnableableParam);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_ASSIGN(m_baseEnableableParamCompPtr, "BaseEnabler", "Base enableable parameter", false, "BaseEnabler");
		I_ASSIGN_TO(m_baseEnableableParamModelCompPtr, m_baseEnableableParamCompPtr, false);
	I_END_COMPONENT;

	CDelegatedEnableableParamComp();

protected:
	// reimplemented (iprm::IEnableableParam)
	virtual bool IsEnabled() const override;
	virtual bool IsEnablingAllowed() const override;
	virtual bool SetEnabled(bool isEnabled = true) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(iprm::IEnableableParam, m_baseEnableableParamCompPtr);
	I_REF(imod::IModel, m_baseEnableableParamModelCompPtr);

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace iprm


