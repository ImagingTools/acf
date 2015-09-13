#include "istd/CSystem.h"


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>

// Windows includes
#ifdef Q_OS_WIN32
    #include <Windows.h>
	#undef RemoveDirectory
#endif


namespace istd
{


/*
	Helper implementation making sleep functionality public.
	\intern
*/
class CQtThread: private QThread
{
public:
	using QThread::usleep;
};


// public static methods

bool CSystem::ConvertToFileName(const QString& fileNameString, QString& fileName, const QString replacingChar)
{
	static char reservedCharacters[] = {
		'\\',
		'/',
		'*',
		'<',
		'>',
		'|',
		':',
		'?',
		'\"'
	};

	int reservedCharactersCount = sizeof(reservedCharacters) / sizeof(char);

	for (int charIndex = 0; charIndex < reservedCharactersCount; charIndex++){
		if (replacingChar.contains(reservedCharacters[charIndex])){
			qDebug("The string used for replacement of invalid characters contains an invalid character");

			return false;
		}
	}

	fileName = fileNameString;

	for (int charIndex = 0; charIndex < reservedCharactersCount; charIndex++){
		fileName.replace(reservedCharacters[charIndex], replacingChar);
	}

	return !fileName.isEmpty();
}


QString CSystem::GetNormalizedPath(const QString& path)
{
	QDir dir(GetEnrolledPath(path));

	return dir.absolutePath();
}


QString CSystem::FindVariableValue(const QString& varName, bool envVars, bool embeddedVars)
{
	if (embeddedVars){
		if (varName == "PlatformCode"){
			if (sizeof(void*) > 4){
				return "x64";
			}
			else{
				return QString();
			}
		}

		if (varName == "CompileMode"){
#ifndef QT_NO_DEBUG
			return "Debug";
#else // QT_NO_DEBUG
			return "Release";
#endif // !QT_NO_DEBUG
		}
		else if (varName == "ConfigurationName"){
			return FindVariableValue("CompileMode", false, true) + FindVariableValue("CompilerName", false, true);
		}
		else if (varName == "ConfigurationDir"){
			return FindVariableValue("ConfigurationName", false, true) + "/";
		}
		else if (varName == "CompilerName"){
#ifdef __clang__
			return "Clang";
#elif defined(__MINGW32__)
			return "MinGW";
#elif defined(__MINGW64__)
			return "MinGW_64";
#elif defined(_MSC_VER)
#if _MSC_VER >= 1900
			QString retVal = "VC14";
#elif _MSC_VER >= 1800
			QString retVal = "VC12";
#elif _MSC_VER >= 1700
			QString retVal = "VC11";
#elif _MSC_VER >= 1600
			QString retVal = "VC10";
#elif _MSC_VER >= 1500
			QString retVal = "VC9";
#elif _MSC_VER >= 1400
			QString retVal = "VC8";
#elif _MSC_VER >= 1300
			QString retVal = "VC7";
#else
			QString retVal = "VC";
#endif

			if (sizeof(void*) > 4){
				return retVal + "_64";
			}
			else{
				return retVal;
			}

#else
			return "Unknown";
#endif
		}
		else if (varName == "."){
			return QDir::currentPath();
		}
	}

	if (envVars){
		EnvironmentVariables environmentVariables = GetEnvironmentVariables();

		EnvironmentVariables::ConstIterator foundVarIter = environmentVariables.constFind(varName.toUpper());
		if (foundVarIter != environmentVariables.constEnd()){
			return foundVarIter.value();
		}
	}

	return QString();
}


QString CSystem::GetEnrolledPath(const QString& path, bool envVars, bool embeddedVars)
{
	QString retVal = path;

	int	endIndex = 0;
	for (		int beginIndex;
				((beginIndex = retVal.indexOf("$(", endIndex)) >= 0);){
		endIndex = retVal.indexOf(")", beginIndex + 2);
		if (endIndex < 0){
			break;
		}

		Q_ASSERT(endIndex >= beginIndex + 2);

		QString varName = retVal.mid(beginIndex + 2, endIndex - beginIndex - 2);

		QString left = retVal.left(beginIndex);
		QString variableValue = FindVariableValue(varName, envVars, embeddedVars);

		retVal = left + variableValue + retVal.mid(endIndex + 1);

		retVal = QDir::toNativeSeparators(retVal);

		endIndex = QString(left + variableValue).length();
	}

	return retVal;
}


CSystem::EnvironmentVariables CSystem::GetEnvironmentVariables()
{
	QStringList processEnvironment = QProcess::systemEnvironment();
	EnvironmentVariables environmentVariables;

	for (int variableIndex = 0; variableIndex < int(processEnvironment.count()); variableIndex++){
		QString variableEntry = processEnvironment[variableIndex];
		QStringList splitted = variableEntry.split('=');

		if (splitted.count() == 2){
			QString variableName = splitted[0];
			QString variableValue = splitted[1];

			environmentVariables[variableName.toUpper()] = variableValue;
		}
	}

	return environmentVariables;
}


void CSystem::SetTempDirectoryPath(const QString& tempPath)
{
	qputenv("TMP", tempPath.toUtf8());
	qputenv("TEMP", tempPath.toUtf8());
}


void CSystem::Sleep(double seconds)
{
	CQtThread::usleep((unsigned long)(seconds * 1000000));
}


bool CSystem::CopyDirectory(const QString& sourcePath, const QString& destinationPath)
{
	CSystem::RemoveDirectory(destinationPath);

	QDir parentDstDir(QFileInfo(destinationPath).path());
	if (!parentDstDir.mkdir(QFileInfo(destinationPath).fileName())){
		return false;
	}

	QDir sourceDir(sourcePath);
	foreach(const QFileInfo &info, sourceDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)){
		QString sourceItemPath = sourcePath + "/" + info.fileName();
		QString destinationItemPath = destinationPath + "/" + info.fileName();
		if (info.isDir()){
			if (!CopyDirectory(sourceItemPath, destinationItemPath)){
				return false;
			}
		}
		else if (info.isFile()){
			if (!QFile::copy(sourceItemPath, destinationItemPath)){
				return false;
			}
		}
		else{
			qDebug(qPrintable(QString("CSystem::CopyDirectory: Unhandled item (%1)").arg(info.filePath())));
		}
	}

	return true;
}


bool CSystem::RemoveDirectory(const QString& directoryPath)
{
	bool retVal = true;
	QDir dir(directoryPath);

	if (dir.exists(directoryPath)){
		QFileInfoList fileInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
		for (QFileInfoList::ConstIterator fileItemIter = fileInfoList.constBegin(); fileItemIter != fileInfoList.constEnd(); fileItemIter++){
			if ((*fileItemIter).isDir()){
				retVal = RemoveDirectory((*fileItemIter).absoluteFilePath());
			}
			else {
				retVal = QFile::remove((*fileItemIter).absoluteFilePath());
			}

			if (!retVal){
				return retVal;
			}
		}

		retVal = dir.rmdir(directoryPath);
	}

	return retVal;
}


bool CSystem::FileCopy(const QString& source, const QString& result, bool overWrite)
{
	QFileInfo sourceFile(source);
	if (!sourceFile.exists())
	{
		return false;
	}

	if (result.isEmpty())
	{
		return false;
	}

	QFileInfo resultFile(result);
	resultFile.setCaching(false);

	const QDir&  resultDir = resultFile.absoluteDir();
	if (!resultDir.exists())
	{
		if (!resultDir.mkpath(resultDir.absolutePath()))
		{
			return false;
		}
	}
	Q_ASSERT(resultDir.exists());

	if (resultFile.exists())
	{
		if (overWrite)
		{
			if (!QFile::remove(resultFile.absoluteFilePath())){
				qDebug(qPrintable(QString("File '%1' could not be overwritten").arg(resultFile.absoluteFilePath())));

				return false;
			}
		}
		else
		{
			return false;
		}
	}

	Q_ASSERT(!resultFile.exists());
	if (!QFile::copy(source, resultFile.absoluteFilePath()))
	{
		return false;
	}

	return true;
}


bool CSystem::FileMove(const QString& source, const QString& targetFolder, bool overWrite)
{
	QFileInfo sourceFile(source);
	if (!sourceFile.exists())
	{
		return false;
	}

	if (targetFolder.isEmpty())
	{
		return false;
	}

	QString targetFilePath = targetFolder + "/" + sourceFile.fileName();

	QFileInfo resultFile(targetFilePath);
	resultFile.setCaching(false);

	const QDir&  resultDir = resultFile.absoluteDir();
	if (!resultDir.exists())
	{
		if (!resultDir.mkpath(resultDir.absolutePath()))
		{
			return false;
		}
	}
	Q_ASSERT(resultDir.exists());

	if (resultFile.exists())
	{
		if (overWrite)
		{
			if (!QFile::remove(resultFile.absoluteFilePath())){
				qDebug(qPrintable(QString("File '%1' could not be overwritten").arg(resultFile.absoluteFilePath())));

				return false;
			}
		}
		else
		{
			return false;
		}
	}

	Q_ASSERT(!resultFile.exists());
	if (!QFile::rename(source, resultFile.absoluteFilePath()))
	{
		return false;
	}

	return true;
}



bool CSystem::EnsurePathExists(const QString& filePath)
{
	QFileInfo inputInfo(filePath);
	QDir dirPath(inputInfo.absolutePath());

	return dirPath.mkpath(filePath);
}


QString CSystem::GetCurrentUserName()
{
	QString userName;

#if defined(Q_OS_MAC)
	userName = QString(getenv("USER"));
#elif defined(Q_OS_WIN32)
	userName = QString(getenv("USERNAME"));
#endif

	return userName;
}


CSystem::FileDriveInfo CSystem::GetFileDriveInfo(const QString& fileDrivePath)
{
	FileDriveInfo fileDriveInfo;

#if defined(Q_OS_WIN32)
	ULARGE_INTEGER lpFreeBytesAvailable, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
	bool isOk = GetDiskFreeSpaceExA(fileDrivePath.toLatin1().data(), &lpFreeBytesAvailable, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes);
	if (isOk){
		fileDriveInfo.freeBytes = lpFreeBytesAvailable.QuadPart;
		fileDriveInfo.totalBytes = lpTotalNumberOfBytes.QuadPart;
	}
#else
	Q_UNUSED(fileDrivePath);
	// TODO: Add implementation for other platforms
#endif

	return fileDriveInfo;
}


} // namespace istd


