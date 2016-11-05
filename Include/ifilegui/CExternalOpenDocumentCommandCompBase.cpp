#include <ifilegui/CExternalOpenDocumentCommandCompBase.h>


// Qt includes
#include <QtCore/QProcess>


namespace ifilegui
{



// protected methods

// reimplemented (icomp::CComponentBase)

void CExternalOpenDocumentCommandCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (		!m_documentPersistenceCompPtr.IsValid() ||
				!m_documentFileCompPtr.IsValid() ||
				!m_applicationPathCompPtr.IsValid()){
		m_startProcessCommand.setEnabled(false);
	}
}


// reimplemented (iqtgui::CProcessStartCommandComp)

bool CExternalOpenDocumentCommandCompBase::StartProcess(const QStringList& arguments)
{
	Q_ASSERT(m_documentPersistenceCompPtr.IsValid());
	Q_ASSERT(m_documentFileCompPtr.IsValid());
	Q_ASSERT(m_applicationPathCompPtr.IsValid());

	const istd::IChangeable* documentPtr = GetDocumentPtr();
	if (documentPtr != NULL){
		int saveState = m_documentPersistenceCompPtr->SaveToFile(*documentPtr, m_documentFileCompPtr->GetPath());
		if (saveState == ifile::IFilePersistence::OS_OK){
			QStringList processArguments = arguments;
			processArguments << m_documentFileCompPtr->GetPath();
		
			return BaseClass::StartProcess(processArguments);
		}
	}

	return false;
}


} // namespace ifilegui


