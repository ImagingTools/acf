#include "iqt/CCopyProcessorComp.h"

// Qt includes
#include <QTextStream>
#include <QDateTime>

#include "iqt/CFileSystem.h"


namespace iqt
{


// reimplemented (icomp::IComponent)

void CCopyProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QStringList filters;
	for (int filterIndex = 0; filterIndex < m_filtersAttrPtr.GetCount(); ++filterIndex){
		const istd::CString& filter = m_filtersAttrPtr[filterIndex];

		filters << iqt::GetQString(filter);
	}

	QStringList excludeFilters;
	for (int excludeIndex = 0; excludeIndex < m_excludeFiltersAttrPtr.GetCount(); ++excludeIndex){
		const istd::CString& excludeFilter = m_excludeFiltersAttrPtr[excludeIndex];

		excludeFilters << iqt::GetQString(excludeFilter);
	}

	int counter = 0;
	if (CopyFileTree(
				iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_inputPathAttrPtr)),
				iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_outputPathAttrPtr)),
				filters,
				excludeFilters,
				*m_recursionDepthAttrPtr,
				counter)){

		SendInfoMessage(MI_END_STATUS, iqt::GetCString(QObject::tr("Success: %1 files copied").arg(counter)));
	}
	else{
		SendErrorMessage(MI_END_STATUS, iqt::GetCString(QObject::tr("Failed: %1 files copied").arg(counter)));
	}
}


// protected methods

bool CCopyProcessorComp::CheckFileExistTree(
			const QDir& inputDir,
			const QStringList& filters,
			const QStringList& excludeFilters,
			int recursionDepth) const
{
	QFileInfoList fileList = inputDir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (		QFileInfoList::const_iterator fileIter = fileList.begin();
				fileIter != fileList.end();
				++fileIter){
		const QFileInfo& fileInfo = *fileIter;
		QString fileName = fileInfo.fileName();

		if (!CheckIfExcluded(fileName, excludeFilters)){
			return true;
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

			if (CheckFileExistTree(inputSubDir, filters, excludeFilters, recursionDepth - 1)){
				return true;
			}
		}
	}

	return false;
}


bool CCopyProcessorComp::CopyFileTree(
			const QDir& inputDir,
			const QDir& outputDir,
			const QStringList& filters,
			const QStringList& excludeFilters,
			int recursionDepth,
			int& counter) const
{
	if (!inputDir.exists()){
		SendWarningMessage(MI_NO_INPUT, iqt::GetCString(QObject::tr("No input directory %1").arg(inputDir.absolutePath())));

		return false;
	}

	outputDir.mkpath(outputDir.absolutePath());
	if (!outputDir.exists()){
		SendWarningMessage(MI_NO_OUTPUT, iqt::GetCString(QObject::tr("No output directory %1").arg(outputDir.absolutePath())));

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

		if (CopyFile(inputFilePath, outputFilePath)){
			counter++;
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

			if (CheckFileExistTree(inputSubDir, filters, excludeFilters, recursionDepth - 1)){
				retVal = CopyFileTree(inputSubDir, outputSubDir, filters, excludeFilters, recursionDepth - 1, counter) && retVal;
			}
		}
	}

	return retVal;
}


bool CCopyProcessorComp::CopyFile(const QString& inputFileName, const QString& outputFileName) const
{
	SendInfoMessage(MI_FILE_INFO, iqt::GetCString(QObject::tr("Processing file %1 to %2").arg(inputFileName).arg(outputFileName)));

	if (!*m_useSubstitutionAttrPtr && !m_licensePathAttrPtr.IsValid()){
		QFile::remove(outputFileName);
		return QFile::copy(inputFileName, outputFileName);
	}

	QFile inputFile(inputFileName);
	QFile outputFile(outputFileName);
	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		SendWarningMessage(MI_INPUT_OPEN, iqt::GetCString(QObject::tr("Opening input file failed (%1)").arg(inputFileName)));

		return false;
	}

	if (!outputFile.open(QIODevice::WriteOnly  | QIODevice::Text)){
		SendWarningMessage(MI_OUTPUT_OPEN, iqt::GetCString(QObject::tr("Opening output file failed (%1)").arg(outputFileName)));

		return false;
	}

	QTextStream inputStream(&inputFile);
	QTextStream outputStream(&outputFile);

	if (m_licensePathAttrPtr.IsValid()){
		QString licenseFileName = iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_licensePathAttrPtr));

		QFile licenseFile(licenseFileName);
		if (!licenseFile.open(QIODevice::ReadOnly)){
			SendWarningMessage(MI_LICENSE_OPEN, iqt::GetCString(QObject::tr("Opening license file failed (%1)").arg(licenseFileName)));

			return false;
		}

		QTextStream licenseStream(&licenseFile);

		while (!licenseStream.atEnd()){
			QString line = licenseStream.readLine();
			outputStream << line << endl;			
		}
	}

	for (int lineCounter = 1; !inputStream.atEnd(); ++lineCounter) {
		QString line = inputStream.readLine();

		int endIndex = -1;
		if (*m_useSubstitutionAttrPtr){
			for (int beginIndex; (beginIndex = line.indexOf("$", endIndex + 1)) >= 0;){
				endIndex = line.indexOf("$", beginIndex + 1);
				if (endIndex < 0){
					SendWarningMessage(MI_BAD_TAG, iqt::GetCString(QObject::tr("%1(%2) : Substitution tag is uncomplete").arg(inputFileName).arg(lineCounter)));

					break;
				}
				QString substitutionTag = line.mid(beginIndex + 1, endIndex - beginIndex - 1);
				QString substituted;

				if (ProcessSubstitutionTag(substitutionTag, substituted)){
					line.replace(beginIndex, endIndex - beginIndex + 1, substituted);

					endIndex += substituted.length() - (endIndex - beginIndex + 1);
				}
				else{
					SendWarningMessage(MI_BAD_TAG, iqt::GetCString(QObject::tr("%1(%2) : Cannot process tag '%3'").arg(inputFileName).arg(lineCounter).arg(substitutionTag)));
				}
			}
		}

		outputStream << line << endl;
	}

	inputFile.close();
	outputFile.close();

	return true;
}


bool CCopyProcessorComp::CheckIfExcluded(const QString& fileName, const QStringList& excludeFilters) const
{
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


bool CCopyProcessorComp::ProcessSubstitutionTag(const QString& tag, QString& result) const
{
	static const QString acfProductNameTag("AcfProductName");
	static const QString acfVersionTag("AcfVersion");
	static const QString acfRawVersionTag("AcfRawVersion");
	static const QString acfTimestampTag("AcfTimestamp");

	int separatorIndex = tag.indexOf(":");

	if (separatorIndex >= 0){
		QString paramTag = tag.left(separatorIndex);
		QString parameter = tag.right(tag.length() - separatorIndex - 1);

		if (paramTag == acfVersionTag){
			bool isNumOk;
			int versionId = parameter.toInt(&isNumOk);
			I_DWORD versionNumber;
			if (		isNumOk &&
						m_applicationInfoCompPtr.IsValid() &&
						m_applicationInfoCompPtr->GetVersionNumber(versionId, versionNumber)){
				result = iqt::GetQString(m_applicationInfoCompPtr->GetEncodedVersionName(versionId, versionNumber));

				return true;
			}
		}
		else if (paramTag == acfRawVersionTag){
			bool isNumOk;
			int versionId = parameter.toInt(&isNumOk);
			I_DWORD versionNumber;
			if (		isNumOk &&
						m_applicationInfoCompPtr.IsValid() &&
						m_applicationInfoCompPtr->GetVersionNumber(versionId, versionNumber)){
				result = QString::number(versionNumber);

				return true;
			}
		}
	}
	else{
		if (tag == acfProductNameTag){
			if (m_applicationInfoCompPtr.IsValid()){
				result = iqt::GetQString(m_applicationInfoCompPtr->GetApplicationName());

				return true;
			}
		}
		else if (tag == acfTimestampTag){
			QDateTime currentTime = QDateTime::currentDateTime();
			result = currentTime.toString();

			return true;
		}

		int userTagsCount = istd::Min(m_userSubstitutionTagsAttrPtr.GetCount(), m_userSubstitutionValuesAttrPtr.GetCount());
		for (int userTagIndex = 0; userTagIndex < userTagsCount; ++userTagIndex){
			if (tag == iqt::GetQString(m_userSubstitutionTagsAttrPtr[userTagIndex])){
				result = iqt::GetQString(m_userSubstitutionValuesAttrPtr[userTagIndex]);

				return true;
			}
		}
	}

	return false;
}


} // namespace iqt


