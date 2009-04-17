#ifndef iqtdoc_CHtmlHelpViewerComp_included
#define iqtdoc_CHtmlHelpViewerComp_included


// Qt includes
#include <QTextBrowser>

#include "iqtgui/TGuiComponentBase.h"

#include "idoc/IHelpViewer.h"
#include "idoc/IHelpFileProvider.h"


namespace iqtdoc
{


class CHtmlHelpViewerComp:
			public iqtgui::TGuiComponentBase<QTextBrowser>,
			virtual public idoc::IHelpViewer
{
public:
	typedef iqtgui::TGuiComponentBase<QTextBrowser> BaseClass;

	I_BEGIN_COMPONENT(CHtmlHelpViewerComp);
		I_REGISTER_INTERFACE(idoc::IHelpViewer);
		I_ASSIGN(m_helpFileProviderCompPtr, "HelpFileProvider", "Generate file path for doxygen files", true, "HelpFileProvider");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpViewer)
	virtual void ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const;

private:
	I_REF(idoc::IHelpFileProvider, m_helpFileProviderCompPtr);
};


} // namespace iqtdoc


#endif // !iqtdoc_CHtmlHelpViewerComp_included


