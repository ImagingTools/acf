#ifndef iavt_CAvtTriggerParamsComp_included
#define iavt_CAvtTriggerParamsComp_included


#include "icomp/CComponentBase.h"

#include "iavt/IAvtTriggerParams.h" 


namespace iavt
{


class CAvtTriggerParamsComp: public icomp::CComponentBase, virtual public iavt::IAvtTriggerParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	CAvtTriggerParamsComp();

	I_BEGIN_COMPONENT(CAvtTriggerParamsComp);
		I_REGISTER_INTERFACE(iavt::IAvtTriggerParams);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_isTriggerEnabledAttrPtr, "ExternalTriggerEnabled", "Enables external trigger", false, false);
		I_ASSIGN(m_triggerModeAttrPtr, "TriggerMode", "Set trigger mode: 0 - Edge mode, 1 - Level mode, 3 - Bulk mode", false, 0);
		I_ASSIGN(m_triggerPolarityAttrPtr, "TriggerPolarity", "Set trigger polarity: 0 - falling, 1 - rising", false, 1);
	I_END_COMPONENT;

	// reimplmented (iavt::IAvtTriggerParams)
	virtual bool IsTriggerEnabled() const;
	virtual void SetTriggerEnabled(bool isEnabled);

	virtual int GetTriggerMode() const;
	virtual void SetTriggerMode(int triggerMode);

	virtual int GetTriggerPolarity() const;
	virtual void SetTriggerPolarity(int triggerMode);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);
	
	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

private:
	bool m_isTriggerEnabled;
	int m_triggerMode;
	int m_triggerPolarity;

	I_ATTR(bool, m_isTriggerEnabledAttrPtr);
	I_ATTR(int, m_triggerModeAttrPtr);
	I_ATTR(int, m_triggerPolarityAttrPtr);
};


} // namespace iavt


#endif // !iavt_CAvtTriggerParamsComp_included


