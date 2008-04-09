#ifndef CDistributeDialog_included
#define CDistributeDialog_included


#include <QDialog>

#include "Generated/ui_CDistributeDialog.h"


class CDistributeDialog: public QDialog, public Ui::CDistributeDialog
{
	Q_OBJECT

public:
	CDistributeDialog();
	~CDistributeDialog();

protected slots:
	virtual void OnAcfDirectoryBrowse();
	virtual void OnDistributeDirectoryBrowse();
	virtual void OnLicenseFileBrowse();
	virtual void OnButtonClicked(QAbstractButton* button);
	virtual void OnApply();

private:
	void DistributeDefaults(const QString& acfDirectory, const QString& distributeDirectory);
	void DistributeSources(const QString& acfDirectory, const QString& distributeDirectory);
	void DistributeLibs(const QString& acfDirectory, const QString& distributeDirectory);
	void DistributeBins(const QString& acfDirectory, const QString& distributeDirectory);

	void CopyFiles(const QString& acfDirectory, const QString& distributeDirectory, const QStringList& nameFilters, const QString& inputDirectory, bool doRecursive = true);
	void CopyInputFile(const QString& inputFileName, const QString& outputFileName);

};


#endif // CDistributeDialog_included

