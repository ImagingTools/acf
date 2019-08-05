#include <iqtgui/CProgressDialog.h>


namespace iqtgui
{


// public methods

CProgressDialog::CProgressDialog(QWidget* parentWidget)
	:BaseClass(parentWidget)
{
	setAutoClose(true);
	setMaximum(100);
	setMinimum(0);
	setWindowModality(Qt::WindowModal);
}


// reimplemented (ibase::IProgressManager)

int CProgressDialog::BeginProgressSession(
			const QByteArray& /*progressId*/,
			const QString& description,
			bool /*isCancelable*/)
{
	setWindowTitle(description);

	return 0;
}


void CProgressDialog::EndProgressSession(int /*sessionId*/)
{
	close();
}


void CProgressDialog::OnProgress(int /*sessionId*/, double currentProgress)
{
	setValue(currentProgress * 100);
}


bool CProgressDialog::IsCanceled(int /*sessionId*/) const
{
	return false;
}


} // namespace iqtgui


