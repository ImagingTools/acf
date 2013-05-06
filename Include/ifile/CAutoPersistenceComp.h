#ifndef ifile_CAutoPersistenceComp_included
#define ifile_CAutoPersistenceComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtCore/QFuture>

// ACF includes
#include "istd/IChangeable.h"
#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "iser/CMemoryWriteArchive.h"
#include "ifile/IFilePersistence.h"
#include "ifile/IFileNameParam.h"


namespace ifile
{


/**
	Allow automatic object persistence.
	Referenced object can be stored and restored according to the configuration mode defined by component attributes.
	Enable \c RestoreOnBegin attribute if you want to restore the object from the file during component initialization.
	Enable \c StoreOnEnd attribute if you want to save the object to the file during component de-initialization.
	Enable \c StoreOnChange attribute if you want to save the object to the file each time if the object data was changed.
	Also you can specify some time interval for permanently data storing.
	If this parameter is set, then the object will be stored to the file in the given time interval,
	but only if the object data was changed.
	\note If time interval for object storing is set, the \c StoreOnChange attribute will be ignored.
*/
class CAutoPersistenceComp:
			public QObject,
			public icomp::CComponentBase,
			protected imod::CSingleModelObserverBase
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CAutoPersistenceComp);
		I_ASSIGN(m_objectCompPtr, "Object", "Object will be restored and stored", true, "Object");
		I_ASSIGN_TO(m_objectModelCompPtr, m_objectCompPtr, false);
		I_ASSIGN_TO(m_serializeableObjectCompPtr, m_objectCompPtr, false);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader used to store and restore object", true, "FileLoader");
		I_ASSIGN(m_filePathCompPtr, "FilePath", "File path where object data will be stored if non anonymous loader is used", false, "FilePath");
		I_ASSIGN(m_restoreOnBeginAttrPtr, "RestoreOnBegin", "Flag indicating that object should be restored on begin", true, true);
		I_ASSIGN(m_storeOnEndAttrPtr, "StoreOnEnd", "Flag indicating that object should be stored on end", true, true);
		I_ASSIGN(m_storeOnChangeAttrPtr, "StoreOnChange", "Flag indicating that object should be stored on each data change", true, false);
		I_ASSIGN(m_storeIntervalAttrPtr, "StoreInterval", "Time interval in seconds for automatic object storing", false, 10)
	I_END_COMPONENT;

	CAutoPersistenceComp();

protected:
	void SaveObjectSnapshot();

	virtual void StoreObject(const istd::IChangeable& object) const;

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private Q_SLOTS:
	void OnTimeout();

private:
	I_REF(istd::IChangeable, m_objectCompPtr);
	I_REF(iser::ISerializable, m_serializeableObjectCompPtr);
	I_REF(imod::IModel, m_objectModelCompPtr);
	I_REF(ifile::IFilePersistence, m_fileLoaderCompPtr);
	I_REF(ifile::IFileNameParam, m_filePathCompPtr);
	I_ATTR(bool, m_restoreOnBeginAttrPtr);
	I_ATTR(bool, m_storeOnEndAttrPtr);
	I_ATTR(bool, m_storeOnChangeAttrPtr);
	I_ATTR(double, m_storeIntervalAttrPtr);

	iser::CMemoryWriteArchive m_lastStoredObjectState;
	bool m_isDataWasChanged;

	QTimer m_storingTimer;

	QFuture<void> m_storingFuture;

	/**
		Object copy used for multi-threading barrier.
	*/
	mutable istd::TDelPtr<istd::IChangeable> m_objectShadowPtr;
};


} // namespace ifile


#endif // !ifile_CAutoPersistenceComp_included


