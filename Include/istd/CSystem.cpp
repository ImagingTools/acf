#include "istd/CSystem.h"


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>

// ACF includes
#include <istd/istd.h>


namespace istd
{


/*
	Helper implementation makeing sleep functionality public.
	\intern
*/
class CQtThread: private QThread
{
public:
	using QThread::usleep;
};


// static members

QString CSystem::GetNormalizedPath(const QString& path)
{
	QDir dir(GetEnrolledPath(path));

	return dir.absolutePath();
}


QString CSystem::GetEnrolledPath(const QString& path)
{
	QString retVal = path;

	int endIndex = 0;
	for (		int beginIndex;
				((beginIndex = retVal.indexOf("$(", endIndex)) >= 0);){
				endIndex = retVal.indexOf(")", beginIndex + 2);
		if (endIndex < 0){
			break;
		}

		Q_ASSERT(endIndex >= beginIndex + 2);

		QString varName = retVal.mid(beginIndex + 2, endIndex - beginIndex - 2);

		retVal = retVal.left(beginIndex) + FindVariableValue(varName) + retVal.mid(endIndex + 1);

		retVal = QDir::toNativeSeparators(retVal);
	}

	return retVal;
}


QString CSystem::FindVariableValue(const QString& varName)
{
	if (varName == "ConfigurationName"){
#if defined(_DEBUG) || defined(DEBUG) || defined(QT_DEBUG)
	#ifdef _MSC_VER
		#if _MSC_VER >= 1600
			return "DebugVC10";
		#elif _MSC_VER >= 1500
			return "DebugVC9";
		#elif _MSC_VER >= 1400
			return "DebugVC8";
		#elif _MSC_VER >= 1300
			return "DebugVC7";
		#else
			return "DebugVC";
		#endif
	#else // _MSC_VER
		return "DebugQMake";
	#endif // _MSC_VER
#else // _DEBUG || DEBUG
	#ifdef _MSC_VER
		#if _MSC_VER >= 1600
			return "ReleaseVC10";
		#elif _MSC_VER >= 1500
			return "ReleaseVC9";
		#elif _MSC_VER >= 1400
			return "ReleaseVC8";
		#elif _MSC_VER >= 1300
			return "ReleaseVC7";
		#else
			return "ReleaseVC";
		#endif
	#else // _MSC_VER
		return "ReleaseQMake";
	#endif // _MSC_VER
#endif // _DEBUG || DEBUG
	}
	else if (varName == "CompilerName"){
#ifdef _MSC_VER
	#if _MSC_VER >= 1600
		return "VC10";
	#elif _MSC_VER >= 1500
		return "VC9";
	#elif _MSC_VER >= 1400
		return "VC8";
	#elif _MSC_VER >= 1300
		return "VC7";
	#else
		return "VC";
	#endif
#else // _MSC_VER
		return "QMake";
#endif // _MSC_VER
	}

	EnvironmentVariables environmentVariables = GetEnvironmentVariables();

	EnvironmentVariables::ConstIterator foundVarIter = environmentVariables.constFind(varName.toUpper());
	if (foundVarIter != environmentVariables.constEnd()){
		return foundVarIter.value();
	}

	return QString();
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
	CQtThread::usleep((unsigned long)seconds * 1000000);
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
			QFile::remove(resultFile.absoluteFilePath());
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


bool CSystem::EnsurePathExists(const QString& filePath)
{
	QFileInfo inputInfo(filePath);
	QDir dirPath(inputInfo.absolutePath());

	return dirPath.mkpath(filePath);
}


} // namespace istd


