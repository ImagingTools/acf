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

void CDocumentProcessingCommandComp::DoDocumentProcessing(const istd::IChangeable* inputDocumentPtr, const QByteArray& /*documentTypeId*/)
{
	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Processing result data model not set");

		return;
	}

	istd::CChangeNotifier changePtr(m_outputDataCompPtr.GetPtr());

	QElapsedTimer timer;
	timer.start();

	int retVal = m_processorCompPtr->DoProcessing(
				m_paramsSetCompPtr.GetPtr(),
				inputDocumentPtr,
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	
	if (retVal != iproc::IProcessor::TS_OK){
		SendErrorMessage(0, "Processing was failed", "Document processing manager");

		return;
	}

#if QT_VERSION > 0x040800
	double processingTime = timer.nsecsElapsed() / 1000000.0;
#else
	double processingTime = timer.elapsed();
#endif

	SendInfoMessage(0, QObject::tr("Processing time: %1 ms").arg(processingTime, 2, 'f', 2), "Document processing manager");

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


