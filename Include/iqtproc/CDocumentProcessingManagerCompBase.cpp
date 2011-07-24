#include "iqtproc/CDocumentProcessingManagerCompBase.h"


// ACF includes
#include "iqtgui/CGuiComponentDialog.h"


namespace iqtproc
{


// public methods

CDocumentProcessingManagerCompBase::CDocumentProcessingManagerCompBase()
	:m_rootCommands("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_processingCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_documentManagerObserver(*this)
{
}


void CDocumentProcessingManagerCompBase::OnModelChanged(int modelId, int /*changeFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	idoc::IDocumentManager* objectPtr = m_documentManagerObserver.GetObjectPtr<idoc::IDocumentManager>(modelId);
	I_ASSERT(objectPtr != NULL);
	if (objectPtr != NULL){
		SetProcessingCommandEnabled(objectPtr->GetActiveView() != NULL || !IsInputDocumentRequired());
	}
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

	imod::IModel* documentManagerModelPtr = dynamic_cast<imod::IModel*>(m_documentManagerCompPtr.GetPtr());
	if (documentManagerModelPtr != NULL){
		m_documentManagerObserver.RegisterModel(documentManagerModelPtr);
	}
	
	QString menuName = tr("Processing");

	if (m_menuNameAttrPtr.IsValid()){
		menuName = iqt::GetQString(*m_menuNameAttrPtr);
	}

	if (m_commandNameAttrPtr.IsValid() && !(*m_commandNameAttrPtr).IsEmpty()){
		QString commandName = iqt::GetQString(*m_commandNameAttrPtr);

		m_processingCommand.SetVisuals(commandName, commandName, commandName);
		m_rootCommands.SetVisuals(menuName, menuName, tr("Document processing actions"));

		m_rootCommands.InsertChild(&m_processingCommand);
		m_processingMenu.InsertChild(&m_rootCommands);

		connect(&m_processingCommand, SIGNAL(triggered()), this, SLOT(OnDoProcessing()));
	}
}


void CDocumentProcessingManagerCompBase::OnComponentDestroyed()
{
	m_documentManagerObserver.UnregisterAllModels();

	BaseClass::OnComponentDestroyed();
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
	
	std::string documentTypeId;
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

	DoDocumentProcessing(inputDocumentPtr, documentTypeId);
}


} // namespace iqtproc


