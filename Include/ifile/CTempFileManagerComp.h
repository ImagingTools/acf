#pragma once


// ACF includes
#include <icomp/CComponentBase.h>
#include <ifile/IFileNameParam.h>
#include <ifile/ITempFileManager.h>


namespace ifile
{


/**
	Component implementation of ITempFileManager interface.
*/
class CTempFileManagerComp: public icomp::CComponentBase, virtual public ITempFileManager
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTempFileManagerComp);
		I_REGISTER_INTERFACE(ITempFileManager);
		I_ASSIGN(m_rootFolderCompPtr, "RootFolder", "Root folder used as a storage for temporary files", false, "RootFolder");
	I_END_COMPONENT;

	// reimplemented (ifile::ITempFileManager)
	virtual QByteArray BeginSession(const QString& subPath = QString(), const QByteArray& proposedSessionId = QByteArray()) override;
	virtual QByteArray AddFileItem(const QByteArray& sessionId, const QString& filePath) override;
	virtual QString GetPath(const QByteArray& sessionId, const QByteArray& id) const override;
	virtual bool RemoveFileItem(const QByteArray& sessionId, const QByteArray& id) override;
	virtual void FinishSession(const QByteArray& sessionId) override;
	virtual void ResetAllSessions() override;

private:
	I_REF(ifile::IFileNameParam, m_rootFolderCompPtr);

	struct Item
	{
		QByteArray uuid;
		QString path;
	};
	typedef QVector<Item> Items;

	struct Session
	{
		QByteArray id;
		QString basePath;

		Items items;
	};

	typedef QMap<QByteArray, Session> SessionMap;
	SessionMap m_sessionsMap;
};


} // namespace ifile


