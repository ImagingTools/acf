#include <ifile/CTempFileManagerComp.h>


// Qt includes
#include <QtCore/QUuid>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QMutexLocker>

// ACF includes
#include <istd/CSystem.h>


namespace ifile
{


// public methods

// reimplemented (ifile::ITempFileManager)

QByteArray CTempFileManagerComp::BeginSession(const QString& subPath, const QByteArray& proposedSessionId)
{
	QMutexLocker locker(&m_mutex);

	for (		SessionMap::ConstIterator iter = m_sessionsMap.constBegin();
				iter != m_sessionsMap.constEnd();
				++iter){
		if (!proposedSessionId.isEmpty()){
			if (iter.key() == proposedSessionId){
				return proposedSessionId;
			}
		}
	}

	QString tempPath;

	if (m_rootFolderCompPtr.IsValid()){
		tempPath = m_rootFolderCompPtr->GetPath();
		if (tempPath.isEmpty()){
			return QByteArray();
		}
	}

	if (tempPath.isEmpty()){
		tempPath = QDir::tempPath();
	}

	QString uuid = QUuid::createUuid().toString().remove('{').remove('-').remove('}');

	Session newSession;
	newSession.id = proposedSessionId.isEmpty() ? uuid.toUtf8() : proposedSessionId;
	newSession.basePath = tempPath;

	// Construct base path for the session:
	if (subPath.isEmpty()){
		newSession.basePath += "/" + uuid;
	}
	else{
		newSession.basePath += "/" + subPath + "/" + uuid;
	}

	if (QFileInfo(newSession.basePath).exists()){
		if (!istd::CSystem::RemoveDirectory(newSession.basePath)){
			return QByteArray();
		}
	}

	// Try to create the session folder:
	if (istd::CSystem::EnsurePathExists(newSession.basePath)){
		m_sessionsMap[newSession.id] = newSession;

		return newSession.id;
	}

	return QByteArray();
}


QByteArray CTempFileManagerComp::AddFileItem(
			const QByteArray& sessionId,
			const QString& filePath,
			const QByteArray& proposedSessionId)
{
	if (filePath.isEmpty()){
		return QByteArray();
	}

	QMutexLocker locker(&m_mutex);

	if (m_sessionsMap.contains(sessionId)){
		Session& session = m_sessionsMap[sessionId];

		QString itemId = proposedSessionId.isEmpty() ? QUuid::createUuid().toString().remove('{').remove('-').remove('}') : proposedSessionId;
		QString itemSubPath = itemId + "/" + filePath;

		QString targetPath = session.basePath + "/" + itemSubPath;
		
		QFileInfo targetFileInfo(targetPath);
		QString fullTargetPath = targetFileInfo.absolutePath();
	
		// Try to create the session folder:
		if (istd::CSystem::EnsurePathExists(fullTargetPath)){
			Item newItem;
			newItem.uuid = itemId.toUtf8();
			newItem.path = itemSubPath;

			m_sessionsMap[sessionId].items.push_back(newItem);

			return newItem.uuid;
		}
	}

	return QByteArray();
}


QString CTempFileManagerComp::GetPath(const QByteArray& sessionId, const QByteArray& id) const
{
	QMutexLocker locker(&m_mutex);

	if (m_sessionsMap.contains(sessionId)){
		const Session& session = m_sessionsMap[sessionId];

		for (		Items::ConstIterator iter = session.items.constBegin();
					iter != session.items.constEnd();
					++iter){
			if (iter->uuid == id){
				return session.basePath + "/" + iter->path;
			}
		}
	}

	return QString();
}


bool CTempFileManagerComp::RemoveFileItem(const QByteArray& sessionId, const QByteArray& id)
{
	QMutexLocker locker(&m_mutex);

	if (m_sessionsMap.contains(sessionId)){
		const Session& session = m_sessionsMap[sessionId];

		for (		Items::ConstIterator iter = session.items.constBegin();
					iter != session.items.constEnd();
					++iter){
			if (iter->uuid == id){
				return QFile::remove(session.basePath + "/" + iter->path);
			}
		}
	}

	return false;
}


void CTempFileManagerComp::FinishSession(const QByteArray& sessionId)
{
	QMutexLocker locker(&m_mutex);

	if (m_sessionsMap.contains(sessionId)){
		istd::CSystem::RemoveDirectory(m_sessionsMap[sessionId].basePath);

		m_sessionsMap.remove(sessionId);
	}
}


void CTempFileManagerComp::ResetAllSessions()
{
	QMutexLocker locker(&m_mutex);

	for (		SessionMap::ConstIterator iter = m_sessionsMap.constBegin();
				iter != m_sessionsMap.constEnd();
				++iter){
		FinishSession(iter.key());
	}
}


} // namespace ifile


