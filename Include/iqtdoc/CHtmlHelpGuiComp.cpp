#include "iqtdoc/CHtmlHelpGuiComp.h"


namespace iqtdoc
{


// reimplemented (idoc::IHelpInfoProvider)

double CHtmlHelpGuiComp::GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	if (m_helpFileProviderCompPtr.IsValid()){
		return m_helpFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr);
	}

	return 0;
}


// reimplemented (idoc::IHelpViewer)

void CHtmlHelpGuiComp::ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr)
{
	QTextBrowser* editorPtr = GetQtWidget();
	if (		(editorPtr != NULL) &&
				m_helpFileProviderCompPtr.IsValid() &&
				(m_helpFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr) > 0)){
		QString filePath = m_helpFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		QUrl url = QUrl::fromLocalFile(filePath);

		if (editorPtr->isVisible()){
			editorPtr->setSource(url);

			m_urlToShow.clear();
		}
		else{
			m_urlToShow = url;
		}
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CHtmlHelpGuiComp::OnGuiShown()
{
	BaseClass::OnGuiShown();

	QTextBrowser* editorPtr = GetQtWidget();
	if ((editorPtr != NULL) && !m_urlToShow.isEmpty()){
		editorPtr->setSource(m_urlToShow);
	}
}


} // namespace iqtdoc


