#pragma once


// Qt includes
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QAbstractButton>

// ACF includes
#include <ibase/CCumulatedProgressManagerBase.h>


namespace iqtgui
{


class CProgressGuiBridge: public QObject, virtual public ibase::CCumulatedProgressManagerBase
{
	Q_OBJECT
public:
	CProgressGuiBridge(QProgressBar* progressWidget, QAbstractButton* cancelButton = NULL);

protected:
	// reimplemented (ibase::CCumulatedProgressManagerBase)
	virtual void OnProgressChanged(double cumulatedValue) override;

protected Q_SLOTS:
	void OnCancelButtonClicked();

Q_SIGNALS:
	void ProgressChanged(int progress);

private:
	QProgressBar* m_progresBarPtr;
	QAbstractButton* m_cancelButtonPtr;
	int m_lastProgressValue;
};


} // namespace iqtgui


