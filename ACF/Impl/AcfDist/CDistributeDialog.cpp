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

CDistributeDialog::CDistributeDialog(const QString& applicationFileName)
:	m_applicationFileName(applicationFileName)
{
	setupUi(this);

	QSettings settings;

	AcfDirEdit->setText(settings.value("Files/ACFDirectory", AcfDirEdit->text()).toString());
	DistributeDirEdit->setText(settings.value("Files/DistributeDirectory", DistributeDirEdit->text()).toString());
	LicenseFileEdit->setText(settings.value("Files/LicenseFile", LicenseFileEdit->text()).toString());
	ApplyLicenseCheck->setChecked(settings.value("Options/UseLicense", true).toBool());
	DistDocCheck->setChecked(settings.value("Options/DistributeDoc", false).toBool());
	DistTutorialCheck->setChecked(settings.value("Options/DistributeTutorial", false).toBool());
	DistSourcesCheck->setChecked(settings.value("Options/DistributeSource", false).toBool());
}


CDistributeDialog::~CDistributeDialog()
{
	QSettings settings;

	settings.setValue("Files/ACFDirectory", AcfDirEdit->text());
	settings.setValue("Files/DistributeDirectory", DistributeDirEdit->text());
	settings.setValue("Files/LicenseFile", LicenseFileEdit->text());
	settings.setValue("Options/UseLicense", ApplyLicenseCheck->isChecked());
	settings.setValue("Options/DistributeDoc", DistDocCheck->isChecked());
	settings.setValue("Options/DistributeTutorial", DistTutorialCheck->isChecked());
	settings.setValue("Options/DistributeSource", DistSourcesCheck->isChecked());
}


// protected slots

void CDistributeDialog::on_AcfDirBrowseButton_clicked()
{
	QString acfDirectory = QFileDialog::getExistingDirectory(this, tr("Open ACF Root Directory"), AcfDirEdit->text());

	AcfDirEdit->setText(acfDirectory);
}


void CDistributeDialog::on_DistributeDirBrowseButton_clicked()
{
	QString distributeDirectory = QFileDialog::getExistingDirectory(this, tr("Open Distribute Directory"), DistributeDirEdit->text());

	DistributeDirEdit->setText(distributeDirectory);
}


void CDistributeDialog::on_LicenseFileBrowseButton_clicked()
{
	QFileInfo fileInfo(DistributeDirEdit->text());
	QString path = fileInfo.absolutePath();
	QString licenseFile = QFileDialog::getOpenFileName(this, tr("Open ACF License File"), path);

	LicenseFileEdit->setText(licenseFile);
}


void CDistributeDialog::on_ButtonBox_clicked(QAbstractButton* button)
{
	if (ButtonBox->buttonRole(button) == QDialogButtonBox::ApplyRole){
		button->setDisabled(true);
		QApplication::processEvents();

		if (!CopyAllFiles()){
			QMessageBox::critical(this, tr("Error"), tr("Some files was not copied"));
		}

		button->setDisabled(false);
	}
}


// protected methods

bool CDistributeDialog::CopyAllFiles()
{
	bool retVal = true;

	bool areDocsEnabled = DistDocCheck->isChecked();
	bool areTutorialsEnabled = DistTutorialCheck->isChecked();
	bool areSourcesEnabled = DistSourcesCheck->isChecked();

	QDir distributeDirectory(DistributeDirEdit->text());
	QDir acfDirectory(AcfDirEdit->text());

	// copy open sources
	QStringList openSourcesFilter;
	openSourcesFilter << "*.h" << "*.js";

	QStringList allSourcesFilter(openSourcesFilter);
	allSourcesFilter << "*.cpp";

	QStringList resourcesFilter;
	resourcesFilter << "*.ui" << "*.qrc" << "*.png";

	QStringList specialsFilter;
	specialsFilter << "*.envVariable";
	specialsFilter << "GNU.txt";

	QStringList projectsFilter;
	projectsFilter << "*.vcproj" << "*.sln";

	QStringList libsFilter;
	libsFilter << "Acf*.lib" << "Acf*.pdb";

	QStringList binsFilter;
	binsFilter << "*.dll" << "*.exe";

	QStringList docsFilter;
	docsFilter << "*.html" << "*.htm";
	docsFilter << "*.bmp" << "*.png" << "*.jpg";
	docsFilter << "*.doc" << "*.pdf";

	if (areSourcesEnabled){
		retVal = CopyFileTree(acfDirectory, distributeDirectory, allSourcesFilter, CopySourceFile, 0) && retVal;

		retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Include", allSourcesFilter, CopySourceFile, 1) && retVal;
		retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Include", projectsFilter + resourcesFilter) && retVal;

		retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Impl", allSourcesFilter, CopySourceFile, 1) && retVal;
		retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Impl", projectsFilter + resourcesFilter) && retVal;
	}
	else{
		retVal = CopyFileTree(acfDirectory, distributeDirectory, openSourcesFilter, CopySourceFile, 0) && retVal;
		retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Include", openSourcesFilter, CopySourceFile, 1) && retVal;
	}

	// copy special files
	retVal = CopyFileTree(acfDirectory, distributeDirectory, specialsFilter) && retVal;

	// copy libraries
	retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Lib", libsFilter, CopyBinFile, 1) && retVal;

	// copy bin files
	retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Bin", binsFilter, CopyBinFile, areTutorialsEnabled? 2: 1) && retVal;

	if (areDocsEnabled){
		// copy documentation files
		if (areTutorialsEnabled){
			retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Docs/Tutorial", allSourcesFilter, CopySourceFile, 3) && retVal;
			retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Docs/Tutorial", projectsFilter, CopyBinFile, 3) && retVal;
		}

		retVal = CopySubFileTree(acfDirectory, distributeDirectory, "Docs", docsFilter, CopyBinFile, 4) && retVal;
	}

	return retVal;
}


bool CDistributeDialog::CopySubFileTree(
			const QDir& inputDir,
			const QDir& outputDir,
			const QString& subDir,
			const QStringList& filters,
			CopyFileFunc copyFunc,
			int recursionDepth) const
{
	QDir realInputDir(inputDir);
	QDir realOutputDir(outputDir);

	if (!subDir.isEmpty()){
		realInputDir = QDir(inputDir.absoluteFilePath(subDir));
		realOutputDir = QDir(outputDir.absoluteFilePath(subDir));
	}

	return CopyFileTree(realInputDir, realOutputDir, filters, copyFunc, recursionDepth);
}


bool CDistributeDialog::CheckFileExistTree(
			const QDir& inputDir,
			const QStringList& filters,
			int recursionDepth) const
{
	QFileInfoList fileList = inputDir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	if (!fileList.isEmpty()){
		return true;
	}

	if (recursionDepth > 0){
		QFileInfoList dirList = inputDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (		QFileInfoList::const_iterator dirIter = dirList.begin();
					dirIter != dirList.end();
					++dirIter){
			const QFileInfo& fileInfo = *dirIter;

			QDir inputSubDir(inputDir.absoluteFilePath(fileInfo.fileName()));

			if (CheckFileExistTree(inputSubDir, filters, recursionDepth - 1)){
				return true;
			}
		}
	}

	return false;
}


bool CDistributeDialog::CopyFileTree(
			const QDir& inputDir,
			const QDir& outputDir,
			const QStringList& filters,
			CopyFileFunc copyFunc,
			int recursionDepth) const
{
	if (!inputDir.exists()){
		QMessageBox::critical(const_cast<CDistributeDialog*>(this), tr("Error"), tr("No valid ACF directory:\n%1").arg(inputDir.absolutePath()));

		return false;
	}

	outputDir.mkpath(outputDir.absolutePath());
	if (!outputDir.exists()){
		return false;
	}

	bool retVal = true;

	QFileInfoList fileList = inputDir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (		QFileInfoList::const_iterator fileIter = fileList.begin();
				fileIter != fileList.end();
				++fileIter){
		const QFileInfo& fileInfo = *fileIter;

		QString fileName = fileInfo.fileName();
		if (fileName.compare(m_applicationFileName, Qt::CaseInsensitive) != 0){
			QString inputFilePath = inputDir.absoluteFilePath(fileName);
			QString outputFilePath = outputDir.absoluteFilePath(fileName);

			retVal = (this->*copyFunc)(inputFilePath, outputFilePath) && retVal;
		}
	}

	if (recursionDepth > 0){
		QFileInfoList dirList = inputDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (		QFileInfoList::const_iterator dirIter = dirList.begin();
					dirIter != dirList.end();
					++dirIter){
			const QFileInfo& fileInfo = *dirIter;

			QDir inputSubDir(inputDir.absoluteFilePath(fileInfo.fileName()));
			QDir outputSubDir(outputDir.absoluteFilePath(fileInfo.fileName()));

			if (CheckFileExistTree(inputSubDir, filters, recursionDepth - 1)){
				retVal = CopyFileTree(inputSubDir, outputSubDir, filters, copyFunc, recursionDepth - 1) && retVal;
			}
		}
	}

	return retVal;
}


bool CDistributeDialog::CopyBinFile(const QString& inputFileName, const QString& outputFileName) const
{
	QFile::remove(outputFileName);
	return QFile::copy(inputFileName, outputFileName);
}


bool CDistributeDialog::CopySourceFile(const QString& inputFileName, const QString& outputFileName) const
{
	if (!ApplyLicenseCheck->isChecked()){
		return CopyBinFile(inputFileName, outputFileName);
	}

	QFile inputFile(inputFileName);
	QFile outputFile(outputFileName);
	QFile licenseFile(LicenseFileEdit->text());
	if (		!inputFile.open(QIODevice::ReadOnly | QIODevice::Text) ||
				!outputFile.open(QIODevice::WriteOnly  | QIODevice::Text) ||
				!licenseFile.open(QIODevice::ReadOnly)){
		return false;
	}

	QTextStream inputStream(&inputFile);
	QTextStream outputStream(&outputFile);
	QTextStream licenseStream(&licenseFile);

	while (!licenseStream.atEnd()){
		QString line = licenseStream.readLine();
		outputStream << line << endl;			
	}

	while (!inputStream.atEnd()) {
		QString line = inputStream.readLine();
		outputStream << line << endl;
	}

	licenseFile.close();
	inputFile.close();
	outputFile.close();

	return true;
}


