#include "iqtproc/CProgressManagerGuiComp.h"


// Qt includes
#include <QtCore/QCoreApplication>


namespace iqtproc
{


CProgressManagerGuiComp::CProgressManagerGuiComp()
:	m_isCanceled(false),
	m_isCancelable(false)
{
}


// protected methods

void CProgressManagerGuiComp::UpdateVisibleComponents()
{
	QWidget* widgetPtr = GetWidget();
	if (widgetPtr != NULL){
		if (*m_automaticHideAttrPtr){
			widgetPtr->setVisible(GetOpenSessionsCount() > 0);
		}

		m_isCancelable = m_isCancelable && !m_isCanceled;
		CancelButton->setEnabled(m_isCancelable);

		UpdateProgressBar();
	}
}


void CProgressManagerGuiComp::UpdateProgressBar()
{
	if (GetOpenSessionsCount() == 0){
		ProgressBar->setValue(0);
		ProgressBar->setEnabled(false);
	}
	else{
		ProgressBar->setValue(1000 * GetCumulatedProgress());
		ProgressBar->setEnabled(true);
	}
}


// reimplemented (iproc::CDelegatedProgressManager)

void CProgressManagerGuiComp::OnCancelable(bool cancelState)
{
	m_isCancelable = cancelState && *m_showCancelAttrPtr;

	CancelButton->setVisible(m_isCancelable);
}


// reimplemented (iproc::IProgressManager)

int CProgressManagerGuiComp::BeginProgressSession(
			const QByteArray& progressId,
			const QString& description,
			bool isCancelable)
{
	if (GetOpenSessionsCount() <= 0){
		m_isCanceled = false;
	}
	else if (m_isCanceled){
		return -1;
	}

	return BaseClass2::BeginProgressSession(progressId, description, isCancelable);
}


bool CProgressManagerGuiComp::IsCanceled(int sessionId) const
{
	if (m_isCanceled){
		return true;
	}

	if (m_isCancelable){
		QCoreApplication::processEvents();
	}

	return BaseClass2::IsCanceled(sessionId);
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
		DescriptionLabel->setText(tr((*m_descriptionAttrPtr).toLocal8Bit()));
		DescriptionLabel->setVisible(true);
	}
	else{
		DescriptionLabel->setVisible(false);
	}

	CancelButton->setVisible(false);

	UpdateVisibleComponents();
}


// protected slots

void CProgressManagerGuiComp::on_CancelButton_clicked()
{
	m_isCanceled = true;

	UpdateVisibleComponents();
}


} // namespace iqtproc


