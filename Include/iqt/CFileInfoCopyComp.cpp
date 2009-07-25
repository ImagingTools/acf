#include "iqt/CFileInfoCopyComp.h"

// Qt includes
#include <QTextStream>
#include <QDateTime>
#include <QFile>

#include "iqt/CFileSystem.h"


namespace iqt
{


// reimplemented (ibase::IFileConvertCopy)

bool CFileInfoCopyComp::CopyFile(const istd::CString& inputFilePath, const istd::CString& outputFilePath) const
{
	QString inputFileName = iqt::GetQString(inputFilePath);
	QString outputFileName = iqt::GetQString(outputFilePath);

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


// protected methods

bool CFileInfoCopyComp::ProcessSubstitutionTag(const QString& tag, QString& result) const
{
	static const QString acfProductNameTag("AcfProductName");
	static const QString acfVersionTag("AcfVersion");
	static const QString acfRawVersionTag("AcfRawVersion");
	static const QString acfRcVersionTag("AcfRcVersion");
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
		else if (paramTag == acfRcVersionTag){
			bool isNumOk;
			int versionId = parameter.toInt(&isNumOk);
			I_DWORD versionNumber;
			if (		isNumOk &&
						m_applicationInfoCompPtr.IsValid() &&
						m_applicationInfoCompPtr->GetVersionNumber(versionId, versionNumber)){
				result = iqt::GetQString(m_applicationInfoCompPtr->GetEncodedVersionName(versionId, versionNumber));
				result.replace(".", ", ");
				result.replace("'", "");

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


