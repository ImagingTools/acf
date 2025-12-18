#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <idoc/IHelpFileProvider.h>
#include <iqtdoc/iqtdoc.h>


namespace iqtdoc
{


class CDoxygenHelpFileProviderComp:
			public icomp::CComponentBase,
			virtual public idoc::IHelpFileProvider
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CDoxygenHelpFileProviderComp);
		I_REGISTER_INTERFACE(idoc::IHelpFileProvider);
		I_ASSIGN(m_doxygenDirectoryAttrPtr, "DoxygenDirectory", "Specify the directory where doxygen directory can be find", true, "./Doxygen");
		I_ASSIGN(m_slaveProvider, "SlaveProvider", "Slave help file path provider", false, "SlaveProvider");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpFileProvider)
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const override;
	virtual QString GetHelpFilePath(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const override;

protected:
	virtual istd::CClassInfo CalcClassInfo(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const;
	QString CalcFilePath(const istd::CClassInfo& classInfo) const;

private:
	I_ATTR(QString, m_doxygenDirectoryAttrPtr);
	I_REF(idoc::IHelpFileProvider, m_slaveProvider);
};


} // namespace iqtdoc


