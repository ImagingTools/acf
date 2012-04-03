#include "iqt/CSystem.h"


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QThread>
#include <QtCore/QProcess>

// ACF includes
#include <istd/istd.h>


namespace iqt
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

		I_ASSERT(endIndex >= beginIndex + 2);

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

	EnvironmentVariables::const_iterator foundVarIter = environmentVariables.find(varName.toUpper());
	if (foundVarIter != environmentVariables.end()){
		return foundVarIter->second;
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


void CSystem::Sleep(double seconds)
{
	CQtThread::usleep((unsigned long)seconds * 1000000);
}


} // namespace iqt


