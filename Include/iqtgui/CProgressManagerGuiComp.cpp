#include <iqtgui/CProgressManagerGuiComp.h>


// Qt includes
#include <QtCore/QCoreApplication>


namespace iqtgui
{


// protected methods

// reimplemented (ibase::CCumulatedProgressManagerBase)

void CProgressManagerGuiComp::OnProgressChanged(double cumulatedValue)
{
	int pogressValue = 1000 * cumulatedValue;

	if (IsGuiCreated()){
		ProgressBar->setValue(pogressValue);

		qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	}
}


void CProgressManagerGuiComp::OnTasksChanged()
{
	QWidget* widgetPtr = GetWidget();
	if (widgetPtr != NULL){
		if (*m_automaticHideAttrPtr){
			auto openTasks = GetProcessedTasks();
			widgetPtr->setVisible(!openTasks.empty());
		}

		CancelButton->setEnabled(IsCancelable());

		qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
	}
}


// reimplemented (iqtgui::CGuiComponentBase)

void CProgressManagerGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	int pogressValue = 1000 * GetCumulatedProgress();

	ProgressBar->setValue(pogressValue);

	if (m_descriptionAttrPtr.IsValid()){
		DescriptionLabel->setText(tr((*m_descriptionAttrPtr).toLocal8Bit()));
		DescriptionLabel->setVisible(true);
	}
	else{
		DescriptionLabel->setVisible(false);
	}

	CancelButton->setVisible(*m_showCancelAttrPtr);
}


// protected slots

void CProgressManagerGuiComp::on_CancelButton_clicked()
{
	SetCanceled();
}


} // namespace iqtgui


