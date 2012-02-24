#include "iqt/CFileSystem.h"


// Qt includes
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QProcess>


// ACF includes
#include "iqt/CProcessEnvironment.h"


namespace iqt
{


// reimplemented (isys::IFileSystem)

QString CFileSystem::GetNormalizedPath(const QString& path) const
{
	QDir dir(GetEnrolledPath(path));

	return dir.absolutePath();
}


QString CFileSystem::GetFileName(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	return fileInfo.fileName();
}


QString CFileSystem::GetBaseFileName(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	return fileInfo.baseName();
}


QString CFileSystem::GetDirPath(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	return fileInfo.absolutePath();
}


bool CFileSystem::HasExtension(const QString& filePath, const QString& extension) const
{
	return filePath.endsWith(extension);
}


bool CFileSystem::IsPresent(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	return fileInfo.exists();
}


bool CFileSystem::CreateFileCopy(const QString& inputFile, const QString& outputFile, bool overwriteExisting) const
{
	QString inputFilePath = inputFile;
	QFileInfo inputFileInfo(inputFilePath);
	if (!inputFileInfo.exists()){
		return false;
	}

	QString outputFilePath = outputFile;

	if (overwriteExisting){
		QFileInfo fileInfo(outputFilePath);
		if (fileInfo.exists()){
			if (!QFile::remove(outputFilePath)){
				return false;
			}
		}
	}

	return QFile::copy(inputFilePath, outputFilePath);
}


bool CFileSystem::RemoveFile(const QString& filePath) const
{
	QFile file(filePath);

	return file.remove();
}


bool CFileSystem::RemoveFolder(const QString& directoryPath, bool ignoreNonEmpty) const
{
	QString directory = directoryPath;

	QDir dir(directory);

	if (ignoreNonEmpty){
		bool retVal = true;
		QFileInfoList directoryItems = dir.entryInfoList(QDir::Files | QDir::Dirs);
		for (int itemIndex = 0; itemIndex < directoryItems.count(); itemIndex++){
			QFileInfo& fileInfo  = directoryItems[itemIndex];

			if (fileInfo.exists()){
				if (fileInfo.isDir()){
					retVal = retVal && RemoveFolder(fileInfo.absoluteFilePath(), true);
				}
				else{				
					retVal = retVal && RemoveFile(fileInfo.absoluteFilePath());
				}
			}
		}
	}

	return dir.rmdir(directory);
}


bool CFileSystem::CreateFolder(const QString& directoryPath) const
{
	QString directory = directoryPath;

	QDir dir(directory);

	return dir.mkdir(directory);
}


// static members

QString CFileSystem::GetEnrolledPath(const QString& path)
{
	QString retVal = path;

	int endIndex = 0;
	for (		int beginIndex;
				((beginIndex = retVal.indexOf("$(", endIndex)) >= 0);){
				endIndex = retVal.indexOf(")", beginIndex + 2);
		if (endIndex < 0){
			break;
		}

		I_ASSERT(endIndex >= beginIndex + 2);

		QString varName = retVal.mid(beginIndex + 2, endIndex - beginIndex - 2);

		retVal = retVal.left(beginIndex) + FindVariableValue(varName) + retVal.mid(endIndex + 1);

		retVal = QDir::toNativeSeparators(retVal);
	}

	return retVal;
}


QString CFileSystem::FindVariableValue(const QString& varName)
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

	iqt::CProcessEnvironment processEnvironment;
	iqt::CProcessEnvironment::EnvironmentVariables environmentVariables = processEnvironment.GetEnvironmentVariables();

	iqt::CProcessEnvironment::EnvironmentVariables::const_iterator foundVarIter = environmentVariables.find(varName.toUpper());
	if (foundVarIter != environmentVariables.end()){
		return foundVarIter->second;
	}

	return QString();
}


} // namespace iqt


