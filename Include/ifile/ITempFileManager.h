#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <istd/IPolymorphic.h>
#include <ifile/ifile.h>


namespace ifile
{


/**
	Manager for working with the temporary files.
*/
class ITempFileManager: virtual public istd::IPolymorphic
{
public:
	/**
		Start a new session for managing temporary files. A new folder with the name \c subPath will be created inside of the root directory for the temp files (e.g. system temp).
		After a session was created, you can generate file paths for temporary files using returned session-ID.
		\param subPath				Path to the subfolder for the session files inside of the temp directory used for this session.
		\param proposedSessionId	The used-ID for the new session. If this value is empty, an unique ID will be generated automatically.
		\return Created session-ID or an empty value if the creation of the new session was failed.
		\note If a session with the same name or the same session-ID already exists, no new session will be created and the existing session-ID will be returned.
	*/
	virtual QByteArray BeginSession(const QString& subPath = QString(), const QByteArray& proposedSessionId = QByteArray()) = 0;

	/**
		Add a new file to the session.
		The generated file path will be inside of the session's base folder.
	*/
	virtual QByteArray AddFileItem(const QByteArray& sessionId, const QString& fileName) = 0;

	/**
		Get the generated path of the file or folder by its ID.
	*/
	virtual QString GetPath(const QByteArray& sessionId, const QByteArray& id) const = 0;

	/**
		Remove the file or folder given by its ID.
	*/
	virtual bool RemoveFileItem(const QByteArray& sessionId, const QByteArray& id) = 0;

	/**
		Finish the given session. All files contained in the session folder and the folder itself will be removed.
	*/
	virtual void FinishSession(const QByteArray& sessionId) = 0;

	/**
		Remove all running sessions.
	*/
	virtual void ResetAllSessions() = 0;
};


} // namespace ifile


