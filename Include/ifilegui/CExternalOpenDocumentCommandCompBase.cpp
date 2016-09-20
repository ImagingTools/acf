#include <ifilegui/CExternalOpenDocumentCommandCompBase.h>


// Qt includes
#include <QtCore/QProcess>


namespace ifilegui
{


// public methods

CExternalOpenDocumentCommandCompBase::CExternalOpenDocumentCommandCompBase()
	:m_fileCommands("&File", 100),
	m_openDocumentCommand("Open In...", 100, ibase::ICommand::CF_GLOBAL_MENU, GI_EXERNAL_OPEN_COMMAND)
{
	connect(&m_openDocumentCommand, SIGNAL(triggered()), this, SLOT(OnOpenDocument()));
	m_fileCommands.InsertChild(&m_openDocumentCommand);

	m_rootCommands.InsertChild(&m_fileCommands);
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CExternalOpenDocumentCommandCompBase::GetCommands() const
{
	return &m_rootCommands;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CExternalOpenDocumentCommandCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (		!m_documentPersistenceCompPtr.IsValid() ||
				!m_documentFileCompPtr.IsValid() ||
				!m_applicationPathCompPtr.IsValid()){
		m_openDocumentCommand.setEnabled(false);
	}
	
	m_openDocumentCommand.SetName(*m_openDocumentCommandNameAttrPtr);
}


// private slots

void CExternalOpenDocumentCommandCompBase::OnOpenDocument()
{
	Q_ASSERT(m_documentPersistenceCompPtr.IsValid());
	Q_ASSERT(m_documentFileCompPtr.IsValid());
	Q_ASSERT(m_applicationPathCompPtr.IsValid());

	const istd::IChangeable* documentPtr = GetDocumentPtr();
	if (documentPtr != NULL){
		int saveState = m_documentPersistenceCompPtr->SaveToFile(*documentPtr, m_documentFileCompPtr->GetPath());
		if (saveState == ifile::IFilePersistence::OS_OK){
			QStringList arguments;
			arguments << m_documentFileCompPtr->GetPath();

			QString applicationPath = m_applicationPathCompPtr->GetPath();
			QFileInfo applicationFileInfo(applicationPath);

#ifdef Q_OS_WIN32
			if (applicationFileInfo.suffix().isEmpty()){
				applicationPath += ".exe";
			}
#endif
#ifdef Q_OS_MAC
			if (!applicationFileInfo.exists() && applicationFileInfo.suffix().isEmpty() && applicationFileInfo.isBundle()){
				applicationPath += ".app";
			}
#endif
#ifndef QT_NO_PROCESS
			QProcess::startDetached(applicationPath, arguments);
#endif // QT_NO_PROCESS
		}
	}
}


} // namespace ifilegui


