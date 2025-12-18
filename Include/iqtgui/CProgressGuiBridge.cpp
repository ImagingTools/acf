#include <iqtgui/CProgressGuiBridge.h>


namespace iqtgui
{


// public methods

CProgressGuiBridge::CProgressGuiBridge(QProgressBar* progressWidget, QAbstractButton* cancelButton)
:	m_progresBarPtr(progressWidget),
	m_cancelButtonPtr(cancelButton),
	m_lastProgressValue(0)
{
	if (m_cancelButtonPtr != NULL){
		connect(m_cancelButtonPtr, SIGNAL(clicked()), this, SLOT(OnCancelButtonClicked()));
	}

	if (m_progresBarPtr != NULL){
		m_progresBarPtr->setValue(0);

		connect(this, SIGNAL(ProgressChanged(int)), m_progresBarPtr, SLOT(setValue(int)), Qt::QueuedConnection);
	}
}


// protected methods

// reimplemented (ibase::CCumulatedProgressManagerBase)

void CProgressGuiBridge::OnProgressChanged(double cumulatedValue)
{
	int progressValue = int(cumulatedValue * 100);
	if (progressValue != m_lastProgressValue){
		m_lastProgressValue = progressValue;

		Q_EMIT ProgressChanged(progressValue);
	}
}


// protected slots

void CProgressGuiBridge::OnCancelButtonClicked()
{
	SetCanceled();
}


} // namespace iqtgui


