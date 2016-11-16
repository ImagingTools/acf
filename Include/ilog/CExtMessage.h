#ifndef ilog_CExtMessage_included
#define ilog_CExtMessage_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CClassInfo.h>
#include <imod/TModelWrap.h>
#include <iser/IObject.h>
#include <ilog/CMessage.h>


namespace ilog
{


/**
	Extended message supporting attached list of objects.
*/
class CExtMessage: public CMessage
{
public:
	typedef CMessage BaseClass;

	explicit CExtMessage(const iser::IObjectFactory* factoryPtr = NULL);
	CExtMessage(istd::IInformationProvider::InformationCategory category,
				int id,
				const QString& text,
				const QString& source,
				int flags = 0,
				const QDateTime* timeStampPtr = NULL,
				const iser::IObjectFactory* factoryPtr = NULL);

	const iser::IObjectFactory* GetObjectFactory() const;
	void SetObjectFactory(const iser::IObjectFactory* factoryPtr);

	/**
		Remove all attached objects from container.
	*/
	void ResetAttachedObjects();
	/**
		Get number of attached objects.
	*/
	int GetAttachedObjectsCount() const;
	/**
		Get single attached object.
	*/
	const iser::IObject* GetAttachedObject(int index) const;
	/**
		Get description of attached object.
	*/
	const QString& GetAttachedObjectDescription(int index) const;
	/**
		Add some object to this container.
		\param	objectPtr	attached object, ownership of this object belongs to container (it will be removed autoamtically).
	*/
	void InsertAttachedObject(const iser::IObject* objectPtr, const QString& description = "");

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (iser::IChangeable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_STRICT);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	static QByteArray GetTypeName();

protected:
	struct AttachedObject
	{
		istd::TDelPtr<const iser::IObject> objectPtr;
		QString description;
	};

private:
	const iser::IObjectFactory* m_factoryPtr;
	QList<AttachedObject> m_attachedObjects;
};


} // namespace ilog


#endif // ilog_CExtMessage_included


