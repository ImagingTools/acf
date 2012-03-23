#include "iqtproc/CDocumentProcessingCommandComp.h"


// Qt includes
#include <QtCore/QElapsedTimer>

// ACF includes
#include "istd/TChangeNotifier.h"

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

	QElapsedTimer timer;

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing was failed", "Document processing manager");

		return;
	}

	SendInfoMessage(0, QObject::tr("Processing time: %1 ms").arg(timer.elapsed()), "Document processing manager");

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


