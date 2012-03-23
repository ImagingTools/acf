#include "iqtproc/CDocumentProcessingManagerComp.h"


// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtproc
{


// protected methods

// reimplemented (iqtproc::CDocumentProcessingManagerCompBase)

void CDocumentProcessingManagerComp::DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const std::string& documentTypeId)
{
	if (m_inPlaceProcessingAttrPtr.IsValid() && *m_inPlaceProcessingAttrPtr){
		DoInPlaceProcessing(const_cast<istd::IChangeable*>(inputDocumentPtr));
	}
	else{
		DoProcessingToOutput(inputDocumentPtr, documentTypeId);
	}
}


// private methods

void CDocumentProcessingManagerComp::DoProcessingToOutput(const istd::IChangeable* inputDocumentPtr, const std::string& documentTypeId)
{
	istd::IChangeable* outputDocumentPtr = NULL;
	if (!m_documentManagerCompPtr->FileNew(documentTypeId, false, "", &outputDocumentPtr)){
		SendErrorMessage(0, "Output document could not be created", "Document processing manager");

		return;
	}

	I_ASSERT(outputDocumentPtr != NULL);

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

	istd::CChangeNotifier changePtr(outputDocumentPtr);

	QElapsedTimer timer;

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				outputDocumentPtr,
				m_progressManagerCompPtr.GetPtr());

	SendInfoMessage(0, QObject::tr("Processing time: %1 ms").arg(timer.elapsed()), "Document processing manager");
	
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


void CDocumentProcessingManagerComp::DoInPlaceProcessing(istd::IChangeable* inputDocumentPtr)
{
	if (inputDocumentPtr == NULL){
		SendErrorMessage(0, "No input document", "Document processing manager");

		return;
	}

	istd::CChangeNotifier changePtr(inputDocumentPtr);

	istd::TDelPtr<istd::IChangeable> outputDocumentPtr(inputDocumentPtr->CloneMe());
	if (!outputDocumentPtr.IsValid()){
		SendErrorMessage(0, "Result object could not be created", "Document processing manager");
		
		return;
	}

	QElapsedTimer timer;

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				outputDocumentPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());

	SendInfoMessage(0, QObject::tr("Processing time: %1 ms").arg(timer.elapsed()), "Document processing manager");
	
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing was failed", "Document processing manager");

		return;
	}

	if (!inputDocumentPtr->CopyFrom(*outputDocumentPtr.GetPtr())){
		SendErrorMessage(0, "Result object is incompatible", "Document processing manager");

		return;
	}
}


} // namespace iqtproc


