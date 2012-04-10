#ifndef iqt_CSystem_included
#define iqt_CSystem_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>


namespace iqt
{


/**
	Implementation of global system helpers based on QT.
*/
class CSystem
{
public:
	typedef QMap<QString, QString> EnvironmentVariables;

	// static members
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
	static EnvironmentVariables GetEnvironmentVariables();
	/**
		Sleep current thread.
	*/
	static void Sleep(double seconds);
};


} // namespace iqt


#endif // !iqt_CSystem_included


