#include "iqtdoc/CExternalOpenDocumentCommandComp.h"


// Qt includes
#include <QtCore/QProcess>

// ACF includes
#include <iqtdoc/CMainWindowGuiComp.h>


namespace iqtdoc
{


// public methods

CExternalOpenDocumentCommandComp::CExternalOpenDocumentCommandComp()
	:m_fileCommands("&File", 100),
	m_openDocumentCommand("Open In...", 100, ibase::ICommand::CF_GLOBAL_MENU, iqtdoc::CMainWindowGuiComp::GI_DOCUMENT + 200)
{
	connect(&m_openDocumentCommand, SIGNAL(triggered()), this, SLOT(OnOpenDocument()));
	m_fileCommands.InsertChild(&m_openDocumentCommand);

	m_rootCommands.InsertChild(&m_fileCommands);
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CExternalOpenDocumentCommandComp::GetCommands() const
{
	return &m_rootCommands;
}


// protected methods

void CExternalOpenDocumentCommandComp::UpdateCommands()
{
	m_openDocumentCommand.SetEnabled(false);

	const idoc::IDocumentManager* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::IPolymorphic* activeViewPtr = objectPtr->GetActiveView();
		if (activeViewPtr != NULL){
			idoc::IDocumentManager::DocumentInfo documentInfo;
			istd::IChangeable* documentPtr = objectPtr->GetDocumentFromView(*activeViewPtr, &documentInfo);
			if ((documentPtr != NULL) && (documentInfo.documentTypeId == *m_documentTypeIdAttrPtr)){
				m_openDocumentCommand.SetEnabled(true);
			}
		}
	}
}


// reimplemented (imod::CSingleModelObserverBase)

void CExternalOpenDocumentCommandComp::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	UpdateCommands();
}


// reimplemented (icomp::CComponentBase)

void CExternalOpenDocumentCommandComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (		!m_documentManagerCompPtr.IsValid() ||
				!m_documentPersistenceCompPtr.IsValid() ||
				!m_documentFileCompPtr.IsValid() ||
				!m_applicationPathCompPtr.IsValid()){
		m_openDocumentCommand.setEnabled(false);
	}

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}

	m_openDocumentCommand.SetName(*m_openDocumentCommandNameAttrPtr);

	UpdateCommands();
}


void CExternalOpenDocumentCommandComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private slots

void CExternalOpenDocumentCommandComp::OnOpenDocument()
{
	Q_ASSERT(m_documentManagerCompPtr.IsValid());
	Q_ASSERT(m_documentPersistenceCompPtr.IsValid());
	Q_ASSERT(m_documentFileCompPtr.IsValid());
	Q_ASSERT(m_applicationPathCompPtr.IsValid());

	istd::IPolymorphic* activeViewPtr = m_documentManagerCompPtr->GetActiveView();
	if (activeViewPtr != NULL){
		istd::IChangeable* documentPtr = m_documentManagerCompPtr->GetDocumentFromView(*activeViewPtr);
		if (documentPtr != NULL){
			int saveState = m_documentPersistenceCompPtr->SaveToFile(*documentPtr, m_documentFileCompPtr->GetPath());
			if (saveState == ifile::IFilePersistence::OS_OK){
				QStringList arguments;
				arguments << m_documentFileCompPtr->GetPath();

				QProcess::startDetached(m_applicationPathCompPtr->GetPath(), arguments);
			}
		}
	}
}


} // namespace iqtdoc


