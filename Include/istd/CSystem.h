#ifndef istd_CSystem_included
#define istd_CSystem_included


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMap>


#if defined(Sleep)
#undef Sleep
#endif


namespace istd
{


/**
	Implementation of global system helpers.

	\ingroup SystemDefines
*/
class CSystem
{
public:
	typedef QMap<QString, QString> EnvironmentVariables;

	struct FileDriveInfo
	{
		FileDriveInfo()
			:totalBytes(0),
			freeBytes(0)
		{
		}

		bool operator ==(const FileDriveInfo& info) const
		{
			return (totalBytes == info.totalBytes) && (freeBytes == info.freeBytes);
		}

		quint64 totalBytes;
		quint64 freeBytes;
	};


	// static methods

	/**
		Make a valid file name from a string.
		All not allowed characters in the \c fileNameString will be replaced by a valid symbol defined in c\replacingChar or removed if this parameter is not set.
		\param fileNameString	Input string representing the file name
		\param fileName	Resulting file name if the conversion was successful.
		\return \c true if the operation was successful, otherwise \c false.
	*/
	static bool ConvertToFileName(const QString& fileNameString, QString& fileName, const QString replacingChar = QString());

	/**
		Get normalized path with enrolled system variables.
	*/
	static QString GetNormalizedPath(const QString& path);

	/**
		Get value of system variables.
		\param	varName	name of variable.
		\param	osEnv	if enabled, the system environment variables will be also considered.
	*/
	static QString FindVariableValue(const QString& varName, bool osEnv = true);

	/**
		Get path with enrolled system variables.
	*/
	static QString GetEnrolledPath(const QString& path, bool osEnv = true);

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
		Copy files and directories from one folder to another.
	*/
	static bool CopyDirectory(const QString &srcPath, const QString &dstPath);

	/**
		Remove a given directory with all contents.
	*/
	static bool RemoveDirectory(const QString& directoryPath);

	/**
		Copy file to directory. When result directory doesn't exist create it.
	*/
	static bool FileCopy(const QString& source, const QString& result, bool overWrite = false);

	/**
		Ensure that the given path exists.
	*/
	static bool EnsurePathExists(const QString& filePath);

	/**
		Get the name of the currently logged user.
	*/
	static QString GetCurrentUserName();

	/** 
		Get the free space information for the given file drive.
	*/
	static FileDriveInfo GetFileDriveInfo(const QString& fileDrivePath);
};


} // namespace istd


#endif // !istd_CSystem_included


