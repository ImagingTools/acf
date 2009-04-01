#ifndef iqtdoc_CDoxygenHelpFileProviderComp_included
#define iqtdoc_CDoxygenHelpFileProviderComp_included


#include "icomp/CComponentBase.h"

#include "idoc/IHelpFileProvider.h"

#include "iqtdoc/iqtdoc.h"


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
	virtual double GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;
	virtual istd::CString GetHelpFilePath(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

protected:
	virtual istd::CClassInfo CalcClassInfo(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;
	istd::CString CalcFilePath(const istd::CClassInfo& classInfo) const;

private:
	I_ATTR(istd::CString, m_doxygenDirectoryAttrPtr);
	I_REF(idoc::IHelpFileProvider, m_slaveProvider);
};


} // namespace iqtdoc


#endif // !iqtdoc_CDoxygenHelpFileProviderComp_included


