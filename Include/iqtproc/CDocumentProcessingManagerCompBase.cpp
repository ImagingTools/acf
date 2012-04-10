#include "iqtproc/CDocumentProcessingManagerCompBase.h"


// ACF includes
#include "iqtgui/CGuiComponentDialog.h"


namespace iqtproc
{


// public methods

CDocumentProcessingManagerCompBase::CDocumentProcessingManagerCompBase()
:	m_rootCommands("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_processingCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU)
{
}


bool CDocumentProcessingManagerCompBase::IsInputDocumentRequired() const
{
	bool retVal = true;

	if (m_inputDocumentRequiredAttrPtr.IsValid()){
		retVal = *m_inputDocumentRequiredAttrPtr;
	}

	return retVal;
}


void CDocumentProcessingManagerCompBase::SetProcessingCommandEnabled(bool isProcessingCommandEnabled)
{
	m_processingCommand.setEnabled(isProcessingCommandEnabled);
}


// reimpemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CDocumentProcessingManagerCompBase::GetCommands() const
{
	return &m_processingMenu;
}


// reimpemented (icomp::IComponent)

void CDocumentProcessingManagerCompBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_documentManagerModelCompPtr.IsValid()){
		RegisterModel(m_documentManagerModelCompPtr.GetPtr());
	}
	
	QString menuName = tr("Processing");

	if (m_menuNameAttrPtr.IsValid()){
		menuName = *m_menuNameAttrPtr;
	}

	if (m_commandNameAttrPtr.IsValid() && !(*m_commandNameAttrPtr).isEmpty()){
		QString commandName = *m_commandNameAttrPtr;

		m_processingCommand.SetVisuals(commandName, commandName, commandName);
		m_rootCommands.SetVisuals(menuName, menuName, tr("Document processing actions"));

		m_rootCommands.InsertChild(&m_processingCommand);
		m_processingMenu.InsertChild(&m_rootCommands);

		connect(&m_processingCommand, SIGNAL(triggered()), this, SLOT(OnDoProcessing()));
	}
}


void CDocumentProcessingManagerCompBase::OnComponentDestroyed()
{
	UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
}


// protected methods

// reimplemented (imod::CMultiModelDispatcherBase)

void CDocumentProcessingManagerCompBase::OnModelChanged(int modelId, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	idoc::IDocumentManager* objectPtr = GetObjectAt<idoc::IDocumentManager>(modelId);
	I_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		SetProcessingCommandEnabled((objectPtr->GetActiveView() != NULL) || !IsInputDocumentRequired());
	}
}


// private slots

void CDocumentProcessingManagerCompBase::OnDoProcessing()
{
	if (!m_documentManagerCompPtr.IsValid()){
		SendErrorMessage(0, "Document manager was not set", "Document processing manager");

		return;
	}

	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "Processor was not set", "Document processing manager");

		return;
	}

	bool inputDocumentRequired = IsInputDocumentRequired();

	istd::IPolymorphic* viewPtr = m_documentManagerCompPtr->GetActiveView();
	if ((viewPtr == NULL) && inputDocumentRequired){
		return;
	}

	istd::IChangeable* inputDocumentPtr = m_documentManagerCompPtr->GetDocumentFromView(*viewPtr);
	if ((inputDocumentPtr == NULL) && inputDocumentRequired){
		return;
	}
	
	QByteArray documentTypeId;
	if (inputDocumentPtr != NULL){
		documentTypeId = m_documentManagerCompPtr->GetDocumentTypeId(*inputDocumentPtr);
	}

	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr;

	if (m_paramsGuiCompPtr.IsValid()){
		dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_paramsGuiCompPtr.GetPtr(),
								QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
								true));

		int retVal = dialogPtr->exec();
		if (retVal != QDialog::Accepted){
			return;
		}
	}

	// Force model update:
	dialogPtr.Reset();

	// Process document:
	DoDocumentProcessing(inputDocumentPtr, documentTypeId);
}


} // namespace iqtproc


