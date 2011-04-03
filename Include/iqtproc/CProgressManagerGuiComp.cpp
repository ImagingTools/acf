#include "iqtproc/CProgressManagerGuiComp.h"


// Qt includes
#include <QCoreApplication>


namespace iqtproc
{


CProgressManagerGuiComp::CProgressManagerGuiComp()
:	m_nextSessionId(0),
	m_progressSum(0),
	m_cancelableSessionsCount(0)
{
}


// reimplemented (iproc::IProgressManager)

bool CProgressManagerGuiComp::IsCanceled(int /*sessionId*/) const
{
	return m_isCanceled;
}


// protected methods

void CProgressManagerGuiComp::UpdateVisibleComponents()
{
	QWidget* widgetPtr = GetWidget();
	if (widgetPtr != NULL){
		if (*m_automaticHideAttrPtr){
			widgetPtr->setVisible(!m_progressMap.empty());
		}

		CancelButton->setVisible(m_cancelableSessionsCount > 0);
		CancelButton->setEnabled(!m_isCanceled);

		UpdateProgressBar();
	}
}


void CProgressManagerGuiComp::UpdateProgressBar()
{
	if (m_progressMap.empty()){
		ProgressBar->setValue(0);
		ProgressBar->setEnabled(false);
	}
	else{
		ProgressBar->setValue(1000 * m_progressSum / m_progressMap.size());
		ProgressBar->setEnabled(true);
	}
}


// reimplemented (istd::IChangeable)

void CProgressManagerGuiComp::OnEndChanges(int changeFlags, istd::IPolymorphic* /*changeParamsPtr*/)
{
	if ((changeFlags & CF_SESSIONS_NUMBER) != 0){
		UpdateVisibleComponents();
	}

	if ((changeFlags & CF_PROGRESS_CHANGED) != 0){
		UpdateProgressBar();
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProgressManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	if (m_descriptionAttrPtr.IsValid()){
		DescriptionLabel->setText(tr((*m_descriptionAttrPtr).ToString().c_str()));
		DescriptionLabel->setVisible(true);
	}
	else{
		DescriptionLabel->setVisible(false);
	}

	UpdateVisibleComponents();
}


// protected slots

void CProgressManagerGuiComp::on_CancelButton_clicked()
{
	UpdateVisibleComponents();

	m_isCanceled = true;
}


} // namespace iqtproc


