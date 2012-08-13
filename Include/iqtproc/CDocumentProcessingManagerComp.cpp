#include "iqtproc/CDocumentProcessingManagerComp.h"


// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include "istd/TChangeNotifier.h"


namespace iqtproc
{


// protected methods

// reimplemented (iqtproc::CDocumentProcessingManagerCompBase)

void CDocumentProcessingManagerComp::DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const QByteArray& documentTypeId)
{
	if (m_inPlaceProcessingAttrPtr.IsValid() && *m_inPlaceProcessingAttrPtr){
		DoInPlaceProcessing(const_cast<istd::IChangeable*>(inputDocumentPtr));
	}
	else{
		DoProcessingToOutput(inputDocumentPtr, documentTypeId);
	}
}


// private methods

void CDocumentProcessingManagerComp::DoProcessingToOutput(const istd::IChangeable* inputDocumentPtr, const QByteArray& documentTypeId)
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
	timer.start();

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				outputDocumentPtr,
				m_progressManagerCompPtr.GetPtr());

#if QT_VERSION > 0x040800
	double processingTime = timer.nsecsElapsed() / 1000000.0;
#else
	double processingTime = timer.elapsed();
#endif

	SendInfoMessage(0, QObject::tr("Processing time: %1 ms").arg(processingTime, 2, 'f', 2), "Document processing manager");

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
	timer.start();

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				outputDocumentPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());

#if QT_VERSION > 0x040800
	double processingTime = timer.nsecsElapsed() / 1000000.0;
#else
	double processingTime = timer.elapsed();
#endif

	SendInfoMessage(0, QObject::tr("Processing time: %1 ms").arg(processingTime, 2, 'f', 2), "Document processing manager");

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


