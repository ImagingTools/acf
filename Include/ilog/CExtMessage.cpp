#include "ilog/CExtMessage.h"


// ACF includes
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace ilog
{


// static attributes
static const iser::CArchiveTag s_attachedObjectsTag("AttachedObjects", "List of attached objects", iser::CArchiveTag::TT_MULTIPLE, NULL, true);
static const iser::CArchiveTag s_objectTag("Object", "Single attached object", iser::CArchiveTag::TT_GROUP, &s_attachedObjectsTag, true);
static const iser::CArchiveTag s_objectIdTag("ObjectTypeId", "ID of object", iser::CArchiveTag::TT_LEAF, &s_objectTag);
static const iser::CArchiveTag s_objectDescriptionTag("ObjectDescription", "human readable description of this object", iser::CArchiveTag::TT_LEAF, &s_objectTag);


CExtMessage::CExtMessage(const iser::IObjectFactory* factoryPtr)
:	BaseClass(),
	m_factoryPtr(factoryPtr)
{
}


CExtMessage::CExtMessage(
			istd::IInformationProvider::InformationCategory category,
			int id,
			const QString& text,
			const QString& source,
			int flags,
			const QDateTime* timeStampPtr,
			const iser::IObjectFactory* factoryPtr)
:	CMessage(category, id, text, source, flags, timeStampPtr),
	m_factoryPtr(factoryPtr)
{
}


const iser::IObjectFactory* CExtMessage::GetObjectFactory() const
{
	return m_factoryPtr;
}


void CExtMessage::SetObjectFactory(const iser::IObjectFactory* factoryPtr)
{
	m_factoryPtr = factoryPtr;
}


void CExtMessage::ResetAttachedObjects()
{
	m_attachedObjects.clear();
}


int CExtMessage::GetAttachedObjectsCount() const
{
	return m_attachedObjects.size();
}


const iser::IObject* CExtMessage::GetAttachedObject(int index) const
{
	return m_attachedObjects[index].objectPtr.GetPtr();
}


void CExtMessage::InsertAttachedObject(const iser::IObject* objectPtr, const QString& description)
{
	Q_ASSERT(objectPtr != NULL);

	m_attachedObjects.push_back(AttachedObject());
	AttachedObject& lastElement = m_attachedObjects.last();

	lastElement.objectPtr.SetPtr(objectPtr);
	lastElement.description = description;
}


// reimplemented (iser::IObject)

QByteArray CExtMessage::GetFactoryId() const
{
	return GetTypeName();
}


// reimplemented (iser::ISerializable)

bool CExtMessage::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);
 
	int objectsCount = m_attachedObjects.size();
	retVal = retVal && archive.BeginMultiTag(s_attachedObjectsTag, s_objectTag, objectsCount);
	if (!retVal){
		return false;
	}

	if (archive.IsStoring()){
		for (int i = 0; i < objectsCount; ++i){
			retVal = retVal && archive.BeginTag(s_objectTag);

			AttachedObject& attachedobject = m_attachedObjects[i];

			iser::IObject* objectPtr = const_cast<iser::IObject*>(attachedobject.objectPtr.GetPtr());
			Q_ASSERT(objectPtr != NULL);
			QByteArray objectTypeId = objectPtr->GetFactoryId();

			retVal = retVal && archive.BeginTag(s_objectIdTag);
			retVal = retVal && archive.Process(objectTypeId);
			retVal = retVal && archive.EndTag(s_objectIdTag);

			retVal = retVal && archive.BeginTag(s_objectDescriptionTag);
			retVal = retVal && archive.Process(attachedobject.description);
			retVal = retVal && archive.EndTag(s_objectDescriptionTag);

			retVal = retVal && objectPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(s_objectTag);
		}
	}
	else if (m_factoryPtr != NULL){
		m_attachedObjects.clear();

		for (int i = 0; i < objectsCount; ++i){
			retVal = retVal && archive.BeginTag(s_objectTag);

			QByteArray objectTypeId;
			retVal = retVal && archive.BeginTag(s_objectIdTag);
			retVal = retVal && archive.Process(objectTypeId);
			retVal = retVal && archive.EndTag(s_objectIdTag);

			QString description;
			retVal = retVal && archive.BeginTag(s_objectDescriptionTag);
			retVal = retVal && archive.Process(description);
			retVal = retVal && archive.EndTag(s_objectDescriptionTag);

			istd::TDelPtr<iser::IObject> objectPtr(m_factoryPtr->CreateInstance(objectTypeId));
			if (objectPtr.IsValid()){
				retVal = retVal && objectPtr->Serialize(archive);
				if (!retVal){
					return false;
				}

				m_attachedObjects.push_back(AttachedObject());
				AttachedObject& lastElement = m_attachedObjects.last();
				lastElement.objectPtr.SetPtr(objectPtr.PopPtr());
				lastElement.description = description;
			}
			else if (!archive.IsTagSkippingSupported()){
				return false;
			}

			retVal = retVal && archive.EndTag(s_objectTag);
		}
	}

	retVal = retVal && archive.EndTag(s_attachedObjectsTag);

	return retVal;
}


// reimplemented (iser::IChangeable)

bool CExtMessage::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	if (BaseClass::CopyFrom(object, mode)){
		m_attachedObjects.clear();

		const CExtMessage* messageObjectPtr = dynamic_cast<const CExtMessage*>(&object);
		if (messageObjectPtr != NULL){
			int objectsCount = messageObjectPtr->m_attachedObjects.size();
			for (int i = 0; i < objectsCount; ++i){
				const AttachedObject& sourceElement = messageObjectPtr->m_attachedObjects[i];
				Q_ASSERT(sourceElement.objectPtr.IsValid());

				istd::TDelPtr<iser::IObject> clonedPtr;
				clonedPtr.SetCastedOrRemove(sourceElement.objectPtr->CloneMe(mode));

				if (clonedPtr.IsValid()){
					m_attachedObjects.push_back(AttachedObject());
					AttachedObject& lastElement = m_attachedObjects.last();
					lastElement.objectPtr.SetPtr(clonedPtr.PopPtr());
					lastElement.description = sourceElement.description;
				}
			}
		}

		return true;
	}

	return true;
}


istd::IChangeable* CExtMessage::CloneMe(CompatibilityMode mode) const
{
	istd::TDelPtr<CExtMessage> clonedPtr(new CExtMessage);
	if (clonedPtr->CopyFrom(*this, mode)){
		return clonedPtr.PopPtr();
	}

	return NULL;
}


// static methods

QByteArray CExtMessage::GetTypeName()
{
	return istd::CClassInfo::GetName<CExtMessage>();
}


} // namespace ilog


