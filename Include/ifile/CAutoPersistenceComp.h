#ifndef ifile_CAutoPersistenceComp_included
#define ifile_CAutoPersistenceComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QFuture>
#include <QtCore/QFileSystemWatcher>

// ACF includes
#include "istd/IChangeable.h"
#include "imod/IModel.h"
#include "imod/CMultiModelDispatcherBase.h"
#include "icomp/CComponentBase.h"
#include "iser/CMemoryWriteArchive.h"
#include "ifile/IFilePersistence.h"
#include "ifile/IFileNameParam.h"


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
	To enforce the instatiation of this component, you should activate 'Automatically create instance' flag in the Compositor.

	\ingroup Persistence
*/
class CAutoPersistenceComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;
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
		MI_FILEPATH
	};

	I_BEGIN_COMPONENT(CAutoPersistenceComp);
		I_ASSIGN(m_objectCompPtr, "Object", "Object will be restored and stored", true, "Object");
		I_ASSIGN_TO(m_objectModelCompPtr, m_objectCompPtr, false);
		I_ASSIGN_TO(m_serializeableObjectCompPtr, m_objectCompPtr, false);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader used to store and restore object", true, "FileLoader");
		I_ASSIGN(m_filePathCompPtr, "FilePath", "File path where object data will be stored if non anonymous loader is used", false, "FilePath");
		I_ASSIGN(m_restoreOnBeginAttrPtr, "RestoreOnBegin", "Flag indicating that object should be restored on begin", true, true);
		I_ASSIGN(m_storeOnEndAttrPtr, "StoreOnEnd", "Flag indicating that object should be stored on end", true, true);
		I_ASSIGN(m_storeOnChangeAttrPtr, "StoreOnChange", "Flag indicating that object should be stored on each data change", true, false);
		I_ASSIGN(m_storeIntervalAttrPtr, "StoreInterval", "Time interval in seconds for automatic object storing", false, 10);
		I_ASSIGN(m_reloadOnFileChangeAttrPtr, "AutoReload", "Update data model if the file was changed", true, false);
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
		Store data object using pre-configured persistence component and file path.
	*/
	virtual void StoreObject(const istd::IChangeable& object) const;

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
		Ensure that the peristence timer was connected to the timeout slot.
	*/
	void EnsureTimerConnected();

private:
	/**
		Reference to the object to be made persisitent.
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

	/**
		Attribute for enabling the reading of the data object during component initialization (eg. on application's start)
	*/
	I_ATTR(bool, m_restoreOnBeginAttrPtr);

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

	iser::CMemoryWriteArchive m_lastStoredObjectState;

	/**
		Data has beem changed.
	*/
	bool m_isDataWasChanged;

	/**
		Loading of the object was successfull.
	*/
	bool m_wasLoadingSuceeded;

	QTimer m_storingTimer;

	QFuture<void> m_storingFuture;

	/**
		Object copy used for multi-threading barrier.
	*/
	mutable istd::TDelPtr<istd::IChangeable> m_objectShadowPtr;

	QFileSystemWatcher m_fileWatcher;

	bool m_isReloading;
};


} // namespace ifile


#endif // !ifile_CAutoPersistenceComp_included


