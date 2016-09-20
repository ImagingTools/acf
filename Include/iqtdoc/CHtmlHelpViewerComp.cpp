#include <iqtdoc/CHtmlHelpViewerComp.h>


// ACF includes
#include <iqt/iqt.h>


namespace iqtdoc
{


// reimplemented (idoc::IHelpInfoProvider)

double CHtmlHelpViewerComp::GetHelpQuality(const QString& contextText, const istd::IPolymorphic* contextObjectPtr) const
{
	if (m_helpFileProviderCompPtr.IsValid()){
		return m_helpFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr);
	}

	return 0;
}


// reimplemented (idoc::IHelpViewer)

void CHtmlHelpViewerComp::ShowHelp(const QString& contextText, const istd::IPolymorphic* contextObjectPtr)
{
	if (		m_helpFileProviderCompPtr.IsValid() &&
				(m_helpFileProviderCompPtr->GetHelpQuality(contextText, contextObjectPtr) > 0)){
		QString filePath = m_helpFileProviderCompPtr->GetHelpFilePath(contextText, contextObjectPtr);

		QUrl url = QUrl::fromLocalFile(filePath);

		if (!m_helpWidgetPtr.IsValid()){
			m_helpWidgetPtr.SetPtr(new QTextBrowser(NULL));
			m_helpWidgetPtr->showMaximized();
		}
		Q_ASSERT(m_helpWidgetPtr.IsValid());

		m_helpWidgetPtr->setSource(url);
		m_helpWidgetPtr->setVisible(true);
	}
}


} // namespace iqtdoc


