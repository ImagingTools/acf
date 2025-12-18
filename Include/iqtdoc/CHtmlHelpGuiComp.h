#pragma once


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QTextBrowser>
#include <QtPrintSupport/QPrinter>
#else
#include <QtGui/QTextBrowser>
#include <QtGui/QPrinter>
#endif

// ACF includes
#include <iqtgui/TGuiComponentBase.h>
#include <idoc/IHelpViewer.h>
#include <idoc/IHelpFileProvider.h>


namespace iqtdoc
{


class CHtmlHelpGuiComp:
			public iqtgui::TGuiComponentBase<QTextBrowser>,
			virtual public idoc::IHelpViewer
{
public:
	typedef iqtgui::TGuiComponentBase<QTextBrowser> BaseClass;

	I_BEGIN_COMPONENT(CHtmlHelpGuiComp);
		I_REGISTER_INTERFACE(idoc::IHelpInfoProvider);
		I_REGISTER_INTERFACE(idoc::IHelpViewer);
		I_ASSIGN(m_helpFileProviderCompPtr, "HelpFileProvider", "Calculate path of html document", true, "HelpFileProvider");
	I_END_COMPONENT;

	// reimplemented (idoc::IHelpInfoProvider)
	virtual double GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const override;

	// reimplemented (idoc::IHelpViewer)
	virtual void ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) override;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated() override;
	virtual void OnGuiShown() override;

private:
	I_REF(idoc::IHelpFileProvider, m_helpFileProviderCompPtr);

	mutable QUrl m_urlToShow;
};


} // namespace iqtdoc


