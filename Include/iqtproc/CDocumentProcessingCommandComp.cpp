#include "iqtproc/CDocumentProcessingCommandComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CTimer.h"

#include "iqtgui/CGuiComponentDialog.h"


namespace iqtproc
{


// protected methods

// reimplemented (iqtproc::CDocumentProcessingManagerCompBase)

void CDocumentProcessingCommandComp::DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const std::string& /*documentTypeId*/)
{
	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Processing result data model not set");

		return;
	}

	istd::CChangeNotifier changePtr(m_outputDataCompPtr.GetPtr());

	iqt::CTimer timer;

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing was failed", "Document processing manager");

		return;
	}

	double processingTime = timer.GetElapsed();

	SendInfoMessage(0, QString("Processing time: ") + QString("%1").arg(processingTime * 1000) + " ms", "Document processing manager");

	changePtr.Reset();

	// show results in the dialog:
	istd::TDelPtr<iqtgui::CGuiComponentDialog> dialogPtr;

	if (m_outputDataGuiCompPtr.IsValid()){
		dialogPtr.SetPtr(
					new iqtgui::CGuiComponentDialog(
								m_outputDataGuiCompPtr.GetPtr(),
								QDialogButtonBox::Ok,
								true));

		dialogPtr->exec();
	}
}


} // namespace iqtproc


