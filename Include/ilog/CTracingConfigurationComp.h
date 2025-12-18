#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/ITracingConfiguration.h>
#include <iser/ISerializable.h>


namespace ilog
{


class CTracingConfigurationComp:
			public icomp::CComponentBase,
			virtual public ilog::ITracingConfiguration,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTracingConfigurationComp);
		I_REGISTER_INTERFACE(ilog::ITracingConfiguration);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_defaultTracingLevel, "DefaulTracingtLevel", "Default tracing level, -1 tracing off, 0 tracing all", true, -1);
	I_END_COMPONENT;

	CTracingConfigurationComp();

	// reimplemented (ilog::ITracingConfiguration)
	virtual int GetTracingLevel() const override;
	virtual void SetTracingLevel(int tracingLevel) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	I_ATTR(int, m_defaultTracingLevel);

	int m_tracingLevel;
};


} // namespace ilog

