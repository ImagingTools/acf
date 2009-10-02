#include "iqt/CFileSystem.h"


// Qt includes
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QProcess>


namespace iqt
{


// reimplemented (isys::IFileSystem)

istd::CString CFileSystem::GetNormalizedPath(const istd::CString& path) const
{
	QDir dir(iqt::GetQString(path));

	QString absolutePath = dir.absolutePath();

	return iqt::GetCString(GetEnrolledPath(absolutePath));
}


istd::CString CFileSystem::GetFileName(const istd::CString& filePath) const
{
	QFileInfo fileInfo(iqt::GetQString(filePath));

	return iqt::GetCString(fileInfo.absoluteFilePath());
}


bool CFileSystem::HasExtension(const istd::CString& filePath, const istd::CString& extension) const
{
	return iqt::GetQString(filePath).endsWith(iqt::GetQString(extension));
}


bool CFileSystem::IsPresent(const istd::CString& filePath) const
{
	QFileInfo fileInfo(iqt::GetQString(filePath));

	return fileInfo.exists();
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
	}

	return retVal;
}


QString CFileSystem::FindVariableValue(const QString& varName)
{
	if (varName == "ConfigurationName"){
#ifdef _DEBUG
	#ifdef _MSC_VER
		#if _MSC_VER >= 1400
			return "DebugVC9";
		#elif _MSC_VER >= 1300
			return "DebugVC9";
		#else
			return "DebugVC";
		#endif
	#else // _MSC_VER
		return "DebugXCD";
	#endif // _MSC_VER
#else // _DEBUG
	#ifdef _MSC_VER
		#if _MSC_VER >= 1400
			return "ReleaseVC9";
		#elif _MSC_VER >= 1300
			return "ReleaseVC8";
		#else
			return "ReleaseVC";
		#endif
	#else // _MSC_VER
		return "ReleaseXCD";
	#endif // _MSC_VER
#endif // _DEBUG
	}

	QString varNameExt = varName + "=";

	const QStringList& envList = GetEnvList();
	for (		QStringList::const_iterator iter = envList.begin();
				iter != envList.end();
				++iter){
		if (iter->startsWith(varNameExt)){
			return iter->mid(varNameExt.length());
		}
	}

	return "";
}


// protected methods

const QStringList& CFileSystem::GetEnvList()
{
	static QStringList envList = QProcess::systemEnvironment();

	return envList;
}


} // namespace iqt


