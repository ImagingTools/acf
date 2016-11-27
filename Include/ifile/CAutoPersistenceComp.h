#ifndef ifile_CAutoPersistenceComp_included
#define ifile_CAutoPersistenceComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QFuture>
#include <QtCore/QFileSystemWatcher>

// ACF includes
#include <istd/IChangeable.h>
#include <imod/IModel.h>
#include <imod/CMultiModelDispatcherBase.h>
#include <ilog/TLoggerCompWrap.h>
#include <iser/CMemoryWriteArchive.h>
#include <ifile/IFilePersistence.h>
#include <ifile/IFileNameParam.h>
#include <ibase/IRuntimeStatusProvider.h>

#if QT_VERSION >= 0x050000
#include <QtCore/QLockFile>
#endif

namespace ifile
{


/**
	The component for automatic object persistence. With the help of this component data objects can be backed up at specific time points and restored.
	The data object that should be restored or backed up, will be connected via component reference.
	The backup or restoring of data is done according to the component configuration, that is given by the component's attributes.
	Enable \c RestoreOnBegin attribute if you want to restore the object from the file during component initialization.
	Enable \c StoreOnEnd attribute if you want to save the object to the file during component de-initialization.
	Enable \c StoreOnChange attribute if you want to save the object to the file each time if the object data was changed.
	Also you can specify some time interval for permanently data storing.
	If this parameter is set, then the object will be stored to the file in the given time interval, but only if the object data was changed.
	\note If the time interval for the object storing is set, the \c StoreOnChange attribute will be ignored.
	\note Please note, that this component doesn't provide any public interfaces. The data object will be attached to the internal observer during the initialization phase of the component.
	To enforce the instantiation of this component, you should activate 'Automatically create instance' flag in the Compositor.

	\ingroup Persistence
*/
class CAutoPersistenceComp:
			public QObject,
			public ilog::CLoggerComponentBase,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef ilog::CLoggerComponentBase BaseClass;
	typedef imod::CMultiModelDispatcherBase BaseClass2;

	enum ModelIds
	{
		/**
			Object was changed
		*/
		MI_OBJECT,

		/**
			File path was changed
		*/
		MI_FILEPATH,

		/**
			Runtime status was changed
		*/
		MI_RUNTIME_STATUS
	};

	I_BEGIN_COMPONENT(CAutoPersistenceComp);
		I_ASSIGN(m_objectCompPtr, "Object", "Object will be restored and stored", true, "Object");
		I_ASSIGN_TO(m_objectModelCompPtr, m_objectCompPtr, false);
		I_ASSIGN_TO(m_serializeableObjectCompPtr, m_objectCompPtr, false);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader used to store and restore object", true, "FileLoader");
		I_ASSIGN(m_filePathCompPtr, "FilePath", "File path where object data will be stored if non anonymous loader is used", false, "FilePath");
		I_ASSIGN_TO(m_filePathModelCompPtr, m_filePathCompPtr, false);
		I_ASSIGN(m_restoreOnBeginAttrPtr, "RestoreOnBegin", "Flag indicating that object should be restored on begin", true, true);
		I_ASSIGN(m_storeOnBeginAttrPtr, "StoreOnBegin", "Flag indicating that object should be stored on begin if the storage file doesn't exist", true, false);
		I_ASSIGN(m_storeOnEndAttrPtr, "StoreOnEnd", "Flag indicating that object should be stored on end", true, true);
		I_ASSIGN(m_storeOnChangeAttrPtr, "StoreOnChange", "Flag indicating that object should be stored on each data change", true, false);
		I_ASSIGN(m_storeIntervalAttrPtr, "StoreInterval", "Time interval in seconds for automatic object storing", false, 10);
		I_ASSIGN(m_reloadOnFileChangeAttrPtr, "AutoReload", "Update data model if the file was changed", true, false);
		I_ASSIGN(m_runtimeStatusCompPtr, "RuntimeStatus", "Application's runtime status", false, "RuntimeStatus");
		I_ASSIGN_TO(m_runtimeStatusModelCompPtr, m_runtimeStatusCompPtr, false);
		I_ASSIGN(m_staleLockTimeAttrPtr, "StaleLockTime", "Time in seconds after which a lock file is considered stale.", true, 30.0);
		I_ASSIGN(m_tryLockTimeoutAttrPtr, "TryLockTimeout", "Lock function will wait for at most TryLockTimeout seconds for the lock file to become available."
					"\nNOTE: Lock will wait forever until the lock file can be locked when set to negative", true, 0.0);
		I_ASSIGN(m_enableLockForLoadAttrPtr, "EnableLockForRead", "When enabled lock is also set when reading from file."
					"\nNOTE: On NTFS file systems, ownership and permissions checking is disabled (in QT) by default for performance reasons."
					"\nEnable this flag when you are known what you are doing.", true, false);
	I_END_COMPONENT;

	/**
		Default constructor.
	*/
	CAutoPersistenceComp();

protected:
	/**
		Store object copy in a separate thread.
	*/
	void SaveObjectSnapshot();

	/**
		Load working object data from some file.
	*/
	virtual bool LoadObject(const QString& filePath);
	/**
		Store some object data to working file.
	*/
	virtual bool StoreObject(const istd::IChangeable& object);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

private Q_SLOTS:
	/**
		Slot triggered if the persistence timer expired and trigger object saving.
	*/
	void OnTimeout();

	void OnFileContentsChanged(const QString& path);

private:
	/**
		Ensure that the persistence timer was connected to the timeout slot.
	*/
	bool TryStartIntervalStore();

	bool LockFile(const QString& filePath, bool store) const;
	void UnlockFile() const;

private:
	I_REF(ibase::IRuntimeStatusProvider, m_runtimeStatusCompPtr);
	I_REF(imod::IModel, m_runtimeStatusModelCompPtr);

	/**
		Reference to the object to be made persistent.
	*/
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(iser::ISerializable, m_serializeableObjectCompPtr);
	I_REF(imod::IModel, m_objectModelCompPtr);

	/**
		Reference to the file persistence component used for data object saving/loading.
	*/
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);

	/**
		Reference to the file path component used for data object saving/loading.
	*/
	I_REF(ifile::IFileNameParam, m_filePathCompPtr);
	I_REF(imod::IModel, m_filePathModelCompPtr);

	/**
		Attribute for enabling the reading of the data object during component initialization (eg. on application's start)
	*/
	I_ATTR(bool, m_restoreOnBeginAttrPtr);

	/**
		Attribute for enabling the storing of the data object during component initialization (eg. on application's start).
		The data object will be stored only if the storage file doesn't exist.
	*/
	I_ATTR(bool, m_storeOnBeginAttrPtr);

	/**
		Attribute for enabling the storing of the data object during component deinitialization (eg. on application's finish)
	*/
	I_ATTR(bool, m_storeOnEndAttrPtr);

	/**
		Attribute for enabling the storing of the data object on each data change.
	*/
	I_ATTR(bool, m_storeOnChangeAttrPtr);

	/**
		Attribute for defining the store interval.
		If set the data object wil be saved in the given interval, of the object's data has been changed.
	*/
	I_ATTR(double, m_storeIntervalAttrPtr);

	/**
		If set the data object wil be reloaded if the file was changed.
	*/
	I_ATTR(bool, m_reloadOnFileChangeAttrPtr);

	/**
		The value of m_staleLockTimeAttrPtr is used by Lock() in order to determine when an existing lock file is considered stale,
		i.e. left over by a crashed process. This is useful for the case where the PID got reused meanwhile, so one way to detect a stale
		lock file is by the fact that it has been around for a long time.
	*/
	I_ATTR(double, m_staleLockTimeAttrPtr);

	/**
		Lock function will wait for at most timeout seconds for the lock file to become available.
		Note: Passing a negative number as the timeout is equivalent to calling lock(),
		i.e. this function will wait forever until the lock file can be locked if timeout is negative.
	*/
	I_ATTR(double, m_tryLockTimeoutAttrPtr);

	/**
		Enable settings lock also for reading.
		On NTFS file systems, ownership and permissions checking is disabled (in QT) by default for performance reasons.
		Enable this flag when you are known what you are doing.
	*/
	I_ATTR(bool, m_enableLockForLoadAttrPtr);

	iser::CMemoryWriteArchive m_lastStoredObjectState;

	/**
		Flag indicating that object has been changed.
	*/
	bool m_isObjectChanged;

	/**
		Object state is synchronized with the file state.
	*/
	bool m_isLoadedFromFile;

	QTimer m_storingTimer;

	QFuture<void> m_storingFuture;

	/**
		Object copy used for multi-threading barrier.
	*/
	mutable istd::TDelPtr<istd::IChangeable> m_objectShadowPtr;

	QFileSystemWatcher m_fileWatcher;

	QMutex m_loadSaveMutex;
	
	mutable bool m_blockLoadingOnFileChanges;

#if QT_VERSION >= 0x050000
	mutable istd::TDelPtr<QLockFile> m_lockFilePtr;
#endif
};


} // namespace ifile


#endif // !ifile_CAutoPersistenceComp_included


