#ifndef istd_CSystem_included
#define istd_CSystem_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>


namespace istd
{


/**
	Implementation of global system helpers based on QT.
*/
class CSystem
{
public:
	typedef QMap<QString, QString> EnvironmentVariables;

	// static methods

	/**
		Get normalized path with enrolled system variables.
	*/
	static QString GetNormalizedPath(const QString& path);

	/**
		Get value of system variables.
	*/
	static QString FindVariableValue(const QString& varName);

	/**
		Get path with enrolled system variables.
	*/
	static QString GetEnrolledPath(const QString& path);

	/**
		Get list of environment valiables related to the current process.
	*/
	static EnvironmentVariables GetEnvironmentVariables();

	/**
		Set path for for temporary data to the current process.
	*/
	static void SetTempDirectoryPath(const QString& tempPath);

	/**
		Sleep current thread.
	*/
	static void Sleep(double seconds);

	/**
		Remove a given directory with all contents.
	*/
	static bool RemoveDirectory(const QString& directoryPath);
};


} // namespace istd


#endif // !istd_CSystem_included


