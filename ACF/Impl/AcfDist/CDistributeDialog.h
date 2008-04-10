#ifndef CDistributeDialog_included
#define CDistributeDialog_included


#include <QString>
#include <QDialog>
#include <QDir>

#include "Generated/ui_CDistributeDialog.h"


class CDistributeDialog: public QDialog, public Ui::CDistributeDialog
{
	Q_OBJECT

public:
	CDistributeDialog(const QString& applicationFileName);
	~CDistributeDialog();

protected slots:
	virtual void on_AcfDirBrowseButton_clicked();
	virtual void on_DistributeDirBrowseButton_clicked();
	virtual void on_LicenseFileBrowseButton_clicked();
	virtual void on_ButtonBox_clicked(QAbstractButton* button);

protected:
	enum{
		MAX_RECURSION_DEPTH = 100
	};

	/**
		Copy single file from \c inputFilePath to \c outputFilePath.
	*/
	typedef bool (CDistributeDialog::*CopyFileFunc)(const QString& inputFilePath, const QString& outputFilePath) const;

	/**
		Copy all files to distributed dirs.
	*/
	bool CopyAllFiles();

	/**
		Copy complete files tree for specified filter.
		\param	inputDir		input directory.
		\param	outputDir		output directory.
		\param	subDir			subdirectory will be applied to input and output directory.
		\param	filters			list of file filter will be copied.
		\param	copyFunc		function used to copy single file.
		\param	recursionDepth	maximale recursion depth.
	*/
	bool CopySubFileTree(
				const QDir& inputDir,
				const QDir& outputDir,
				const QString& subDir,
				const QStringList& filters,
				CopyFileFunc copyFunc = CopyBinFile,
				int recursionDepth = MAX_RECURSION_DEPTH) const;

	/**
		Check if in tree specified files exists.
		\param	inputDir		input directory.
		\param	filters			list of file filter will be copied.
		\param	recursionDepth	maximale recursion depth.
	*/
	bool CheckFileExistTree(
				const QDir& inputDir,
				const QStringList& filters,
				int recursionDepth = MAX_RECURSION_DEPTH) const;
	/**
		Copy complete files tree for specified filter.
		\param	inputDir		input directory.
		\param	outputDir		output directory.
		\param	filters			list of file filter will be copied.
		\param	copyFunc		function used to copy single file.
		\param	recursionDepth	maximale recursion depth.
	*/
	bool CopyFileTree(
				const QDir& inputDir,
				const QDir& outputDir,
				const QStringList& filters,
				CopyFileFunc copyFunc = CopyBinFile,
				int recursionDepth = MAX_RECURSION_DEPTH) const;
	bool CopyBinFile(const QString& inputFileName, const QString& outputFileName) const;
	bool CopySourceFile(const QString& inputFileName, const QString& outputFileName) const;

private:
	QString m_applicationFileName;
};


#endif // CDistributeDialog_included

