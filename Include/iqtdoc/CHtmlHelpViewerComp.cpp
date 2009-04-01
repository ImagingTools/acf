#include "iqtdoc/CHtmlHelpViewerComp.h"


namespace iqtdoc
{


// reimplemented (idoc::IHelpViewer)

void CHtmlHelpViewerComp::ShowHelp(const istd::CString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	QTextBrowser* editorPtr = GetQtWidget();
	if (		(editorPtr != NULL) &&
				m_helpFileProviderCompPtr.IsValid() &&
				m_helpFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr)){
		istd::CString filePath = m_helpFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		QUrl url = QUrl::fromLocalFile(iqt::GetQString(filePath));
		editorPtr->setSource(url);
	}
}


} // namespace iqtdoc


