#include "CCopyProcessor.h"


// STL includes
#include <iostream>

// Qt includes
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QRegExp>


// ACF includes
#include "iser/CXmlFileReadArchive.h"
#include "iqt/CFileList.h"


// public methods

bool CCopyProcessor::ProcessConfigFile(const istd::CString configFilePath)
{
	bool retVal = true;

	iser::CXmlFileReadArchive archive(configFilePath);

	static iser::CArchiveTag licenseTag("License", "Path to license file");
	static iser::CArchiveTag copyRulesTag("Copy", "Set of copy rules");
	static iser::CArchiveTag ruleTag("Rule", "Single copy rule");
	static iser::CArchiveTag sourceDirTag("SourceDir", "Source directory");
	static iser::CArchiveTag destDirTag("DestDir", "Destination directory");
	static iser::CArchiveTag filtersTag("Filters", "List of file filters");
	static iser::CArchiveTag filterTag("Filter", "Single filter like *.txt");
	static iser::CArchiveTag excludesTag("Excludes", "List of exclude file filters");
	static iser::CArchiveTag modeTag("Mode", "Copy mode 0 - binary, 1 - text with licencse");
	static iser::CArchiveTag depthTag("Depth", "Depth of recursion, 0 - only actual directory");

	retVal = retVal && archive.BeginTag(licenseTag);
	retVal = retVal && archive.Process(m_licenseFileName);
	retVal = retVal && archive.EndTag(licenseTag);

	int rulesCount;
	retVal = retVal && archive.BeginMultiTag(copyRulesTag, ruleTag, rulesCount);
	if (!retVal){
		return false;
	}

	for (int ruleIndex = 0; ruleIndex < rulesCount; ++ruleIndex){
		retVal = retVal && archive.BeginTag(ruleTag);

		istd::CString sourceDir;
		retVal = retVal && archive.BeginTag(sourceDirTag);
		retVal = retVal && archive.Process(sourceDir);
		retVal = retVal && archive.EndTag(sourceDirTag);

		istd::CString destDir;
		retVal = retVal && archive.BeginTag(destDirTag);
		retVal = retVal && archive.Process(destDir);
		retVal = retVal && archive.EndTag(destDirTag);

		QStringList filters;
		int filtersCount;
		retVal = retVal && archive.BeginMultiTag(filtersTag, filterTag, filtersCount);
		if (!retVal){
			return false;
		}
		for (int filterIndex = 0; filterIndex < filtersCount; ++filterIndex){
			istd::CString filter;
			retVal = retVal && archive.BeginTag(filterTag);
			retVal = retVal && archive.Process(filter);
			retVal = retVal && archive.EndTag(filterTag);

			filters << iqt::GetQString(filter);
		}
		retVal = retVal && archive.EndTag(filtersTag);

		QStringList excludeFilters;
		int excludesCount;
		retVal = retVal && archive.BeginMultiTag(excludesTag, filterTag, excludesCount);
		if (!retVal){
			return false;
		}
		for (int excludeIndex = 0; excludeIndex < excludesCount; ++excludeIndex){
			istd::CString filter;
			retVal = retVal && archive.BeginTag(filterTag);
			retVal = retVal && archive.Process(filter);
			retVal = retVal && archive.EndTag(filterTag);

			excludeFilters << iqt::GetQString(filter);
		}
		retVal = retVal && archive.EndTag(excludesTag);

		int mode;
		retVal = retVal && archive.BeginTag(modeTag);
		retVal = retVal && archive.Process(mode);
		retVal = retVal && archive.EndTag(modeTag);

		int depth;
		retVal = retVal && archive.BeginTag(depthTag);
		retVal = retVal && archive.Process(depth);
		retVal = retVal && archive.EndTag(depthTag);

		retVal = retVal && archive.EndTag(ruleTag);

		if (!retVal){
			return false;
		}

		std::cout << "Copying " << sourceDir.ToString() << " to " << destDir.ToString() << "..." << std::endl;

		int counter = 0;
		if (CopyFileTree(
					iqt::GetQString(sourceDir),
					iqt::GetQString(destDir),
					filters,
					excludeFilters,
					(mode == 1)? &CCopyProcessor::CopySourceFile: &CCopyProcessor::CopyBinFile,
					depth,
					counter)){
			std::cout << "Success";
		}
		else{
			retVal = false;

			std::cout << "Failed";
		}

		std::cout << " (" << counter << " files copied)"<< std::endl;
	}

	retVal = retVal && archive.EndTag(copyRulesTag);

	return retVal;
}


// protected methods

bool CCopyProcessor::CheckFileExistTree(
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


bool CCopyProcessor::CopyFileTree(
			const QDir& inputDir,
			const QDir& outputDir,
			const QStringList& filters,
			const QStringList& excludeFilters,
			CopyFileFunc copyFunc,
			int recursionDepth,
			int& counter) const
{
	if (!inputDir.exists()){
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

		if (CheckIfExcluded(fileName, excludeFilters)){
			continue;
		}

		QString inputFilePath = inputDir.absoluteFilePath(fileName);
		QString outputFilePath = outputDir.absoluteFilePath(fileName);

		if ((this->*copyFunc)(inputFilePath, outputFilePath)){
			counter++;
		}
		else{
			std::cout << "\tCannot copy " << inputFilePath.toStdString() << " to " << outputFilePath.toStdString() << std::endl;
			
			retVal = false;
		}
	}

	if (recursionDepth > 0){
		QFileInfoList dirList = inputDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (		QFileInfoList::const_iterator dirIter = dirList.begin();
					dirIter != dirList.end();
					++dirIter){
			const QFileInfo& fileInfo = *dirIter;
			QString fileName = fileInfo.fileName();

			if (CheckIfExcluded(fileName, excludeFilters)){
				continue;
			}

			QDir inputSubDir(inputDir.absoluteFilePath(fileName));
			QDir outputSubDir(outputDir.absoluteFilePath(fileName));

			if (CheckFileExistTree(inputSubDir, filters, recursionDepth - 1)){
				retVal = CopyFileTree(inputSubDir, outputSubDir, filters, excludeFilters, copyFunc, recursionDepth - 1, counter) && retVal;
			}
		}
	}

	return retVal;
}


bool CCopyProcessor::CopyBinFile(const QString& inputFileName, const QString& outputFileName) const
{
	QFile::remove(outputFileName);
	return QFile::copy(inputFileName, outputFileName);
}


bool CCopyProcessor::CopySourceFile(const QString& inputFileName, const QString& outputFileName) const
{
	QFile inputFile(inputFileName);
	QFile outputFile(outputFileName);
	QFile licenseFile(iqt::GetQString(m_licenseFileName));
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


bool CCopyProcessor::CheckIfExcluded(const QString& fileName, const QStringList& excludeFilters) const
{
	bool retVal = false;
	for (		QStringList::const_iterator excludeIter = excludeFilters.begin();
				excludeIter != excludeFilters.end();
				++excludeIter){
		QRegExp rx(*excludeIter);
		rx.setPatternSyntax(QRegExp::Wildcard);
		rx.setCaseSensitivity(Qt::CaseInsensitive);
		if (rx.exactMatch(fileName)){
			return true;
		}
	}

	return false;
}


