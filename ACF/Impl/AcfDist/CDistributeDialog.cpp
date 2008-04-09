#include "CDistributeDialog.h"


#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QSettings>


#include "iqt/CFileList.h"

#include "istd/istd.h"


// public methods

CDistributeDialog::CDistributeDialog()
{
	setupUi(this);

	connect(AcfDirBrowseButton, SIGNAL(clicked()), this, SLOT(OnAcfDirectoryBrowse()));
	connect(DistributeDirBrowseButton, SIGNAL(clicked()), this, SLOT(OnDistributeDirectoryBrowse()));
	connect(LicenseFileBrowseButton, SIGNAL(clicked()), this, SLOT(OnLicenseFileBrowse()));
	connect(ButtonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(OnButtonClicked(QAbstractButton*)));

	QSettings settings;

	AcfDirEdit->setText(settings.value("Files/ACFDirectory").toString());
	DistributeDirEdit->setText(settings.value("Files/DistributeDirectory").toString());
	LicenseFileEdit->setText(settings.value("Files/LicenseFile").toString());
}


CDistributeDialog::~CDistributeDialog()
{
	QSettings settings;

	settings.setValue("Files/ACFDirectory", AcfDirEdit->text());
	settings.setValue("Files/DistributeDirectory", DistributeDirEdit->text());
	settings.setValue("Files/LicenseFile", LicenseFileEdit->text());
}


// protected slots

void CDistributeDialog::OnAcfDirectoryBrowse()
{
	QString acfDirectory = QFileDialog::getExistingDirectory(this, tr("Open ACF Root Directory"), AcfDirEdit->text());

	AcfDirEdit->setText(acfDirectory);
}


void CDistributeDialog::OnDistributeDirectoryBrowse()
{
	QString distributeDirectory = QFileDialog::getExistingDirectory(this, tr("Open Distribute Directory"), DistributeDirEdit->text());

	DistributeDirEdit->setText(distributeDirectory);
}


void CDistributeDialog::OnLicenseFileBrowse()
{
	QFileInfo fileInfo(DistributeDirEdit->text());
	QString path = fileInfo.absolutePath();
	QString licenseFile = QFileDialog::getOpenFileName(this, tr("Open ACF License File"), path);

	LicenseFileEdit->setText(licenseFile);
}


void CDistributeDialog::OnButtonClicked(QAbstractButton* button)
{
	if (ButtonBox->buttonRole(button) == QDialogButtonBox::ApplyRole){
		button->setDisabled(true);
		QApplication::processEvents();
		OnApply();
		button->setDisabled(false);
	}
}


void CDistributeDialog::OnApply()
{	
	QString distributeDirectory = DistributeDirEdit->text() + "\\ACF";
	QString  acfDirectory = AcfDirEdit->text() + "\\ACF";

	QDir inputDirectory(acfDirectory);
	if (!inputDirectory.exists()){
		QMessageBox(QMessageBox::Critical, tr("Error"), tr("No valid ACF directory"));

		return;
	}

	DistributeDefaults(AcfDirEdit->text(), DistributeDirEdit->text());

	DistributeSources(acfDirectory, distributeDirectory);

	DistributeLibs(acfDirectory, distributeDirectory);

	DistributeBins(acfDirectory, distributeDirectory);
}


// private methods

void CDistributeDialog::DistributeDefaults(const QString& acfDirectory, const QString& distributeDirectory)
{
	// create list of files:
	QStringList nameFilters;
	nameFilters << "*.js" << "*.envVariable" << "GNU.txt";

	CopyFiles(acfDirectory, distributeDirectory, nameFilters, "\\Acf", false);
	CopyFiles(acfDirectory + "\\Acf", distributeDirectory, nameFilters, "\\Acf", false);
}


void CDistributeDialog::DistributeSources(const QString& acfDirectory, const QString& distributeDirectory)
{
	// create list of files:
	QStringList nameFilters;
	nameFilters << "*.h";

	QDir inputDirectory(acfDirectory);
	I_ASSERT(inputDirectory.exists());

	iqt::CFileList inputFiles;
	inputFiles.Create(inputDirectory, true, nameFilters);

	for (int inputFileIndex = 0; inputFileIndex < inputFiles.count(); inputFileIndex++){
		QString inputFileName = inputFiles.at(inputFileIndex);

		int includeIndex = inputFileName.indexOf("\\Include");
		if (includeIndex < 0){
			continue;
		}

		QString outputFileSuffix = inputFileName.mid(includeIndex, inputFileName.length()-1);

		QFileInfo fileInfo(inputFileName);

		QFile inputFile(inputFileName);
		if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
			continue;
		}

		QString outputFileName = distributeDirectory + outputFileSuffix;

		CopyInputFile(inputFileName, outputFileName);

		QFile outputFile(outputFileName);
		if (!outputFile.open(QIODevice::WriteOnly  | QIODevice::Text)){
			continue;
		}

		QTextStream in(&inputFile);
		QTextStream out(&outputFile);
		if (ApplyLicenseCheck->isChecked()){
			QFile licenseFile(LicenseFileEdit->text());
			if (ApplyLicenseCheck->isChecked()){
				licenseFile.open(QIODevice::ReadOnly);
			}

			QTextStream licenseStream(&licenseFile);

			while (!licenseStream.atEnd()){
				QString line = licenseStream.readLine();
				out << line << endl;			
			}

			licenseFile.close();
		}

		while (!in.atEnd()) {
			QString line = in.readLine();
			out << line << endl;
		}

		out.flush();

		outputFile.close();
		inputFile.close();
	}
}


void CDistributeDialog::DistributeLibs(const QString& acfDirectory, const QString& distributeDirectory)
{
	QStringList nameFilters;
	nameFilters << "*.lib";

	CopyFiles(acfDirectory, distributeDirectory, nameFilters, "\\Lib");
}


void CDistributeDialog::DistributeBins(const QString& acfDirectory, const QString& distributeDirectory)
{
	QStringList nameFilters;
	nameFilters << "*.dll" << "*.exe";

	CopyFiles(acfDirectory, distributeDirectory, nameFilters, "\\Bin");
}


void CDistributeDialog::CopyFiles(const QString& acfDirectory, const QString& distributeDirectory, const QStringList& nameFilters, const QString& copyDirectory, bool doRecursive)
{
	QDir inputDirectory(acfDirectory);

	iqt::CFileList inputFiles;
	inputFiles.Create(inputDirectory, doRecursive, nameFilters);

	for (int inputFileIndex = 0; inputFileIndex < inputFiles.count(); inputFileIndex++){
		QString inputFileName = inputFiles.at(inputFileIndex);

		int includeIndex = inputFileName.indexOf(copyDirectory, Qt::CaseInsensitive);
		if (includeIndex < 0){
			continue;
		}

		QString outputFileSuffix = inputFileName.mid(includeIndex, inputFileName.length()-1);
		outputFileSuffix = outputFileSuffix.replace("\\Acf\\Acf", "\\Acf\\", Qt::CaseInsensitive);

		QFileInfo fileInfo(inputFileName);

		QFile inputFile(inputFileName);
		if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
			continue;
		}

		QString outputFileName = distributeDirectory + outputFileSuffix;

		CopyInputFile(inputFileName, outputFileName);
	}
}


void CDistributeDialog::CopyInputFile(const QString& inputFileName, const QString& outputFileName)
{
	QFileInfo fileInfo(outputFileName);

	QString dirPath = fileInfo.absolutePath();

	QDir outputDir(dirPath);
	if (!outputDir.exists()){
		outputDir.mkpath(dirPath);
	}

	QFile::copy(inputFileName, outputFileName);
}
