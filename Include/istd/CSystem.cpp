#include <istd/CSystem.h>


// Qt includes
#include <QtCore/QtGlobal>
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QCoreApplication>
#if QT_VERSION >= 0x050400
	#include <QtCore/QStorageInfo>
#endif

// Windows includes
#ifdef Q_OS_WIN
	#include <windows.h>
	#include <Winnetwk.h>	// for WNetGetUniversalName

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

QString CSystem::GetOperationSystemName()
{
	QString retVal;

#if defined(Q_OS_WIN)
	retVal = "Windows";
#elif defined(Q_OS_UNIX)
	retVal = "Unix";
#elif defined(Q_OS_IOS)
	retVal = "iOS";
#elif defined(Q_OS_OSX)
	retVal = "OSX";
#endif

	return retVal;
}


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


QString CSystem::GetVariableValue(const QString& varName, bool envVars, bool embeddedVars)
{
	if (embeddedVars){
		if (varName == "PlatformCode"){
			return s_platformCode;
		}
		else if (varName == "CompileMode"){
			return s_compilerMode;
		}
		else if (varName == "CompilerName"){
			return s_compilerName;
		}
		else if (varName == "ConfigurationName"){
			QString retVal = s_compilerMode + s_compilerName;
			if (s_platformCode == "x64"){
				retVal += "_64";
			}

			return retVal;
		}
		else if (varName == "ConfigurationDir"){
			QString retVal = s_compilerMode + s_compilerName;
			if (s_platformCode == "x64"){
				retVal += "_64";
			}

			return retVal + "/";
		}
		else if (varName == "ApplicationDir"){
			return QCoreApplication::applicationDirPath();
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
		QString variableValue = GetVariableValue(varName, envVars, embeddedVars);

		retVal = left + variableValue + retVal.mid(endIndex + 1);

		retVal = QDir::toNativeSeparators(retVal);

		endIndex = QString(left + variableValue).length();
	}

	return retVal;
}


CSystem::EnvironmentVariables CSystem::GetEnvironmentVariables()
{
	EnvironmentVariables environmentVariables;

#ifndef QT_NO_PROCESS
	QStringList processEnvironment = QProcess::systemEnvironment();

	for (int variableIndex = 0; variableIndex < int(processEnvironment.count()); variableIndex++){
		QString variableEntry = processEnvironment[variableIndex];
		QStringList splitted = variableEntry.split('=');

		if (splitted.count() == 2){
			QString variableName = splitted[0];
			QString variableValue = splitted[1];

			environmentVariables[variableName.toUpper()] = variableValue;
		}
	}
#endif
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

#if QT_VERSION >= 0x050400
	QStorageInfo storageInfo(fileDrivePath);
	fileDriveInfo.totalBytes = storageInfo.bytesTotal();
	fileDriveInfo.freeBytes = storageInfo.bytesFree();
	fileDriveInfo.isValid = storageInfo.isValid();
#else
	// TODO: Remove this code after switching to Qt 5.5,
	// use QStorageInfo instead of this implementation!
	#if defined(Q_OS_WIN)
		ULARGE_INTEGER lpFreeBytesAvailable, lpTotalNumberOfBytes, lpTotalNumberOfFreeBytes;
		bool isOk = GetDiskFreeSpaceExA(fileDrivePath.toLatin1().data(), &lpFreeBytesAvailable, &lpTotalNumberOfBytes, &lpTotalNumberOfFreeBytes);
		if (isOk){
			fileDriveInfo.freeBytes = lpFreeBytesAvailable.QuadPart;
			fileDriveInfo.totalBytes = lpTotalNumberOfBytes.QuadPart;
			fileDriveInfo.isValid = isOk;
		}
	#else
		Q_UNUSED(fileDrivePath);
		// TODO: Add implementation for other platforms
	#endif
#endif // Qt < 5.4.0

	return fileDriveInfo;
}


QString CSystem::GetCompilerVariable(const QString& varName)
{
	if (varName == "PlatformCode"){
		if (sizeof(void*) > 4){
			return "x64";
		}
		else{
			return QString();
		}
	}
	else if (varName == "CompileMode"){
#ifndef QT_NO_DEBUG
		return "Debug";
#else // QT_NO_DEBUG
		return "Release";
#endif // !QT_NO_DEBUG
	}
	else if (varName == "ConfigurationName"){
		return GetCompilerVariable("CompileMode") + GetCompilerVariable("CompilerName");
	}
	else if (varName == "CompilerName"){
#ifdef __clang__
		QString retVal = "Clang";
		if (sizeof(void*) > 4){
			return retVal + "_64";
		}
		else{
			return retVal;
		}
#elif defined(__MINGW32__)
		return "MinGW";
#elif defined(__MINGW64__)
		return "MinGW_64";
#elif defined(__GNUC__)
		return "GCC";
#elif defined(__INTEL_COMPILER)
		return "ICC";
#elif defined(_MSC_VER)
#if _MSC_VER >= 1920
		QString retVal = "VC16";
#elif _MSC_VER >= 1910
		QString retVal = "VC15";
#elif _MSC_VER >= 1900
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
		return retVal;

#else
		return "Unknown";
#endif
	}

	return "";
}


void CSystem::SetUserVariables(const QString& compilerMode, const QString& compilerName, const QString& platformCode)
{
	s_compilerMode = compilerMode;
	s_compilerName = compilerName;
	s_platformCode = platformCode;
}


QString CSystem::ConvertNetworkPathToUnc(const QString& localPath)
{
	QString uncPath = localPath;

#if defined(Q_OS_WIN) && defined(Q_CC_MSVC)
	// TODO: Remove this code after complete switching to Qt 5.5,
	// use QStorageInfo instead of this implementation!

	QString nativeLocalPath = QDir::toNativeSeparators(localPath);

	DWORD pathBufferSize = 0;
	char dummyBuffer[2] = {0};

	std::wstring localPathString = std::wstring((const wchar_t *)nativeLocalPath.utf16());

	if (::WNetGetUniversalNameW(localPathString.data(), UNIVERSAL_NAME_INFO_LEVEL, dummyBuffer, &pathBufferSize) == ERROR_MORE_DATA){
		wchar_t* buf = new wchar_t[pathBufferSize];
		UNIVERSAL_NAME_INFOW* puni = (UNIVERSAL_NAME_INFOW*) buf;

		if (::WNetGetUniversalNameW(localPathString.data(), UNIVERSAL_NAME_INFO_LEVEL, buf, &pathBufferSize) == NO_ERROR){
			uncPath = QString::fromUtf16((const unsigned short*)puni->lpUniversalName);
		}

		delete [] buf;
	}
#endif // Q_OS_WIN && Q_CC_MSVC

	return uncPath;
}


// private static attributes

QString CSystem::s_compilerMode = CSystem::GetCompilerVariable("CompileMode");
QString CSystem::s_compilerName = CSystem::GetCompilerVariable("CompilerName");
QString CSystem::s_platformCode = CSystem::GetCompilerVariable("PlatformCode");


} // namespace istd


