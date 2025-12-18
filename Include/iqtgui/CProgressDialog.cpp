#include <iqtgui/CProgressDialog.h>


namespace iqtgui
{


// public methods

CProgressDialog::CProgressDialog(const QString& description, const QString& defaultText, QWidget* parentWidget)
:	BaseClass(parentWidget),
	m_lastTaskText(defaultText),
	m_lastProgressValue(0)
{
	setAutoClose(true);
	setMaximum(100);
	setMinimum(0);
	setWindowModality(Qt::WindowModal);
	setWindowTitle(description);
	setLabelText(defaultText);

	connect(this, SIGNAL(TaskTextChanged(QString)), this, SLOT(setLabelText(QString)), Qt::QueuedConnection);
	connect(this, SIGNAL(ProgressChanged(int)), this, SLOT(setValue(int)), Qt::QueuedConnection);
}


// protected methods

// reimplemented (ibase::CCumulatedProgressManagerBase)

void CProgressDialog::OnProgressChanged(double cumulatedValue)
{
	int progressValue = int(cumulatedValue * 100);
	if (progressValue != m_lastProgressValue){
		m_lastProgressValue = progressValue;

		Q_EMIT ProgressChanged(progressValue);
	}
}


void CProgressDialog::OnTasksChanged()
{
	QString taskName;
	auto tasksInfo = GetProcessedTasks(true, 1);
	if (!tasksInfo.empty()){
		taskName = tasksInfo[0].description;
	}

	if (taskName != m_lastTaskText){
		m_lastTaskText = taskName;

		Q_EMIT TaskTextChanged(taskName);
	}
}


} // namespace iqtgui


