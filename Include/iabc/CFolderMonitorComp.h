#ifndef iabc_CFolderMonitorComp_included
#define iabc_CFolderMonitorComp_included


#include "iabc/iabc.h"


// Qt includes
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QTimer>
#include <QThread>


// ACF includes
#include "imod/CSingleModelObserverBase.h"

#include "icomp/CComponentBase.h"

#include "ibase/IFolderMonitor.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"

#include "iqt/CCriticalSection.h"


namespace iabc
{


/**
	Component for monitoring of the file folder.
*/
class CFolderMonitorComp:
			public QThread,
			public ibase::CLoggerComponentBase,
			public imod::CSingleModelObserverBase,
			virtual public ibase::IFolderMonitor
{
	Q_OBJECT

public:
	class FileSystemChanges: public istd::IPolymorphic
	{
	public:
		QStringList addedFiles;
		QStringList removedFiles;
		QStringList modifiedFiles;
		QStringList attributeChangedFiles;
	};

	typedef ibase::CLoggerComponentBase BaseClass;
	typedef QThread BaseClass2;

	I_BEGIN_COMPONENT(CFolderMonitorComp);
		I_REGISTER_INTERFACE(ibase::IFolderMonitor);
		I_ASSIGN(m_notificationFrequencyAttrPtr, "NotificationFrequency", "Minimal time range for the folder check after the change notification", false, 10);
		I_ASSIGN(m_poolingFrequencyAttrPtr, "PoolingFrequency", "Minimal frequency for pooling of changes in seconds", false, 60);
		I_ASSIGN_MULTI_0(m_fileFilterExpressionsAttrPtr, "FileFilters", "File filters for the folder (as regular expression)", false);
		I_ASSIGN(m_fileNameParamCompPtr, "FolderPath", "Specify folder to observe.", true, "FolderPath");
	I_END_COMPONENT;

	CFolderMonitorComp();

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (ibase::IFolderMonitor)
	virtual istd::CStringList GetChangedFileItems(int changeFlags) const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	// reimplemented (QThread)
	virtual void run();

private slots:
	void OnDirectoryChanged(const QString& directoryPath);
	void OnFolderChanged(int changeFlags);

Q_SIGNALS:
	void FolderChanged(int changeFlags);

private:
	void SetFolderPath(const QString& folderPath);
	void StartObserverThread();
	void StopObserverThread();

private:
	I_ATTR(int, m_notificationFrequencyAttrPtr);
	I_ATTR(int, m_poolingFrequencyAttrPtr);
	I_ATTR(istd::CString, m_defaultPathAttrPtr);
	I_MULTIATTR(istd::CString, m_fileFilterExpressionsAttrPtr)
	I_REF(iprm::IFileNameParam, m_fileNameParamCompPtr);

	QFileSystemWatcher m_fileSystemWatcher;

	QFileInfoList m_directoryFiles;

	mutable iqt::CCriticalSection m_lock;

	bool m_finishThread;
	bool m_directoryChangesConfirmed;

	QString m_currentFolderPath;

	// Attribute shadows:
	int m_notificationFrequency;
	int m_poolingFrequency;
	QStringList m_fileFilterExpressions;

	FileSystemChanges m_folderChanges;
};


} // namespace iabc


#endif // !iabc_CFolderMonitorComp_included


