#ifndef iqtdoc_CHtmlHelpViewerComp_included
#define iqtdoc_CHtmlHelpViewerComp_included


// Qt includes
#include <QTextBrowser>


// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/CComponentBase.h"

#include "idoc/IHelpViewer.h"
#include "idoc/IHelpFileProvider.h"


namespace iqtdoc
{


class CHtmlHelpViewerComp:
			public icomp::CComponentBase,
			virtual public idoc::IHelpViewer
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CHtmlHelpViewerComp);
		I_REGISTER_INTERFACE(idoc::IHelpInfoProvider);
		I_REGISTER_INTERFACE(idoc::IHelpViewer);
		I_ASSIGN(m_helpFileProviderCompPtr, "HelpFileProvider", "Calculate path of html document", true, "HelpFileProvider");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpInfoProvider)
	virtual double GetHelpQuality(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

	// reimplemented (idoc::IHelpViewer)
	virtual void ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr);

private:
	I_REF(idoc::IHelpFileProvider, m_helpFileProviderCompPtr);

	istd::TDelPtr<QTextBrowser> m_helpWidgetPtr;
};


} // namespace iqtdoc


#endif // !iqtdoc_CHtmlHelpViewerComp_included


