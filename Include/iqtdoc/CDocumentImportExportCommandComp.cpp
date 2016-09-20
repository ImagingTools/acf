#include <iqtdoc/CDocumentImportExportCommandComp.h>


// ACF includes
#include <iqtdoc/CMainWindowGuiComp.h>


namespace iqtdoc
{


// public methods

CDocumentImportExportCommandComp::CDocumentImportExportCommandComp()
	:m_fileCommands("&File", 100),
	m_importCommand("Import...", 100, ibase::ICommand::CF_GLOBAL_MENU, iqtdoc::CMainWindowGuiComp::GI_DOCUMENT + 100),
	m_exportCommand("Export...", 100, ibase::ICommand::CF_GLOBAL_MENU, iqtdoc::CMainWindowGuiComp::GI_DOCUMENT + 100)
{
}


// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CDocumentImportExportCommandComp::GetCommands() const
{
	return &m_rootCommands;
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CDocumentImportExportCommandComp::OnUpdate(const ChangeSet& /*changeSet*/)
{
	UpdateCommands();
}


// reimplemented (icomp::CComponentBase)

void CDocumentImportExportCommandComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerModelCompPtr.IsValid()){
		m_documentManagerModelCompPtr->AttachObserver(this);
	}

	if (m_importCommandNameAttrPtr.IsValid()){
		m_importCommand.SetName(*m_importCommandNameAttrPtr);

		connect(&m_importCommand, SIGNAL(triggered()), this, SLOT(OnImport()));

		m_fileCommands.InsertChild(&m_importCommand);
	}

	if (m_exportCommandNameAttrPtr.IsValid()){
		m_exportCommand.SetName(*m_exportCommandNameAttrPtr);

		connect(&m_exportCommand, SIGNAL(triggered()), this, SLOT(OnExport()));

		m_fileCommands.InsertChild(&m_exportCommand);
	}

	m_rootCommands.InsertChild(&m_fileCommands);

	UpdateCommands();
}


void CDocumentImportExportCommandComp::OnComponentDestroyed()
{
	EnsureModelDetached();

	BaseClass::OnComponentDestroyed();
}


// private slots

void CDocumentImportExportCommandComp::OnImport()
{
	if (m_documentPersistenceCompPtr.IsValid() && m_documentManagerCompPtr.IsValid()){
		istd::IChangeable* documentPtr = NULL;

		if (m_documentManagerCompPtr->InsertNewDocument(*m_documentTypeIdAttrPtr, false, "", &documentPtr)){
			Q_ASSERT(documentPtr != NULL);

			if (m_documentPersistenceCompPtr->LoadFromFile(*documentPtr) == ifile::IFilePersistence::OS_OK){
				m_documentManagerCompPtr->AddViewToDocument(*documentPtr);
			}
			else{
				int documentsCount = m_documentManagerCompPtr->GetDocumentsCount();
				for (int documentIndex = 0; documentIndex < documentsCount; ++documentIndex){
					if (&m_documentManagerCompPtr->GetDocumentFromIndex(documentIndex) == documentPtr){
						m_documentManagerCompPtr->CloseDocument(documentIndex, true);
					}
				}
			}
		}
	}
}


void CDocumentImportExportCommandComp::OnExport()
{
	if (m_documentPersistenceCompPtr.IsValid() && m_documentManagerCompPtr.IsValid()){
		istd::IPolymorphic* activeViewPtr = m_documentManagerCompPtr->GetActiveView();
		if (activeViewPtr != NULL){
			istd::IChangeable* documentPtr = m_documentManagerCompPtr->GetDocumentFromView(*activeViewPtr);
			if (documentPtr != NULL){
				m_documentPersistenceCompPtr->SaveToFile(*documentPtr);
			}
		}
	}
}


// protected methods

void CDocumentImportExportCommandComp::UpdateCommands()
{
	m_exportCommand.SetEnabled(false);

	const idoc::IDocumentManager* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		istd::IPolymorphic* activeViewPtr = objectPtr->GetActiveView();
		if (activeViewPtr != NULL){
			idoc::IDocumentManager::DocumentInfo documentInfo;
			istd::IChangeable* documentPtr = objectPtr->GetDocumentFromView(*activeViewPtr, &documentInfo);
			if (documentPtr != NULL && documentInfo.documentTypeId == *m_documentTypeIdAttrPtr){
				m_exportCommand.SetEnabled(true);
			}
		}
	}
}


}  // namespace iqtdoc


