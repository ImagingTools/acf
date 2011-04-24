#include "iqtproc/CDocumentProcessingManagerComp.h"


// ACF includes
#include "iqtgui/CGuiComponentDialog.h"


namespace iqtproc
{


// public methods

CDocumentProcessingManagerComp::CDocumentProcessingManagerComp()
	:m_rootCommands("", 100, ibase::ICommand::CF_GLOBAL_MENU),
	m_processingCommand("", 100, ibase::ICommand::CF_GLOBAL_MENU)	
{
}


// reimpemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CDocumentProcessingManagerComp::GetCommands() const
{
	return &m_processingMenu;
}


// reimpemented (icomp::IComponent)

void CDocumentProcessingManagerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

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


// private slots

void CDocumentProcessingManagerComp::OnDoProcessing()
{
	if (!m_documentManagerCompPtr.IsValid()){
		SendErrorMessage(0, "Document manager was not set", "Document processing manager");

		return;
	}

	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "Processor was not set", "Document processing manager");

		return;
	}

	istd::IPolymorphic* viewPtr = m_documentManagerCompPtr->GetActiveView();
	if (viewPtr == NULL){
		return;
	}

	istd::IChangeable* inputDocumentPtr = m_documentManagerCompPtr->GetDocumentFromView(*viewPtr);
	if (inputDocumentPtr == NULL){
		return;
	}
	
	std::string documentTypeId = m_documentManagerCompPtr->GetDocumentTypeId(*inputDocumentPtr);

	if (m_inPlaceProcessingAttrPtr.IsValid() && *m_inPlaceProcessingAttrPtr){
		DoInPlaceDocumentProcessing(*inputDocumentPtr);
	}
	else{
		DoDocumentProcessing(*inputDocumentPtr, documentTypeId);
	}
}


void CDocumentProcessingManagerComp::DoDocumentProcessing(const istd::IChangeable& inputDocument, const std::string& documentTypeId)
{
	istd::IChangeable* outputDocumentPtr = NULL;
	if (!m_documentManagerCompPtr->FileNew(documentTypeId, false, "", &outputDocumentPtr)){
		SendErrorMessage(0, "Output document could not be created", "Document processing manager");

		return;
	}

	int documentIndex = -1;

	int documentCounts = m_documentManagerCompPtr->GetDocumentsCount();
	for (int docIndex = 0; docIndex < documentCounts; docIndex++){
		istd::IChangeable& document = m_documentManagerCompPtr->GetDocumentFromIndex(docIndex);
		if (&document == outputDocumentPtr){
			documentIndex = docIndex;
			break;
		}
	}

	I_ASSERT(documentIndex >= 0);

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

	I_ASSERT(outputDocumentPtr != NULL);

	istd::CChangeNotifier changePtr(outputDocumentPtr);

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				&inputDocument,
				outputDocumentPtr,
				m_progressManagerCompPtr.GetPtr());
	
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing was failed", "Document processing manager");

		changePtr.Abort();

		m_documentManagerCompPtr->FileClose(documentIndex);
	}
	else{
		istd::IPolymorphic* viewPtr = m_documentManagerCompPtr->AddViewToDocument(*outputDocumentPtr);
		if (viewPtr == NULL){
			SendErrorMessage(0, "Output view could not be created", "Document processing manager");

			changePtr.Abort();
	
			m_documentManagerCompPtr->FileClose(documentIndex);		
		}
	}
}


void CDocumentProcessingManagerComp::DoInPlaceDocumentProcessing(istd::IChangeable& inputDocument)
{
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

	istd::CChangeNotifier changePtr(&inputDocument);

	istd::TDelPtr<istd::IChangeable> outputDocumentPtr(inputDocument.CloneMe());
	if (!outputDocumentPtr.IsValid()){
		SendErrorMessage(0, "Result object could not be created", "Document processing manager");
		
		return;
	}

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				&inputDocument,
				outputDocumentPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing was failed", "Document processing manager");
	}

	if (!inputDocument.CopyFrom(*outputDocumentPtr.GetPtr())){
		SendErrorMessage(0, "Result object is incompatible", "Document processing manager");

		return;
	}
}


} // namespace iqtproc


