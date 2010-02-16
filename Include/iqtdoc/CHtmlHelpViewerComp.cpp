#include "iqtdoc/CHtmlHelpViewerComp.h"


namespace iqtdoc
{


// reimplemented (idoc::IHelpViewer)

void CHtmlHelpViewerComp::ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QTextBrowser* editorPtr = GetQtWidget();
	if (		(editorPtr != NULL) &&
				m_helpFileProviderCompPtr.IsValid() &&
				(m_helpFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr) > 0)){
		istd::CString filePath = m_helpFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		QUrl url = QUrl::fromLocalFile(iqt::GetQString(filePath));

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

void CHtmlHelpViewerComp::OnGuiShown()
{
	BaseClass::OnGuiShown();

	QTextBrowser* editorPtr = GetQtWidget();
	if ((editorPtr != NULL) && !m_urlToShow.isEmpty()){
		editorPtr->setSource(m_urlToShow);
	}
}


} // namespace iqtdoc


