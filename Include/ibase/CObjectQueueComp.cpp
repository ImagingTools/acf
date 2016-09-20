#include <ibase/CObjectQueueComp.h>


#include <iser/ISerializable.h>

#include <icomp/CComponentBase.h>

#include <ibase/IObjectQueue.h>


namespace ibase
{


// reimplemented (ibase::IObjectQueue)

int CObjectQueueComp::GetObjectsCount(const QByteArray* typeIdPtr) const
{
	if (typeIdPtr != NULL){
		return 0;
	}
	else{
		return int(m_objectsQueue.size());
	}
}


void CObjectQueueComp::ClearQueue()
{
	while (!m_objectsQueue.isEmpty()){
		m_objectsReserve.push_front(m_objectsQueue.back());

		m_objectsQueue.pop_back();
	}

	TryReductReserve();
}


istd::IChangeable* CObjectQueueComp::CreateFrontObject(int offsetPos, const QByteArray* typeIdPtr)
{
	if (typeIdPtr == NULL){
		int objectsCount = int(m_objectsQueue.size());
		if (offsetPos <= objectsCount){
			ObjectQueue::iterator iter = m_objectsQueue.begin() + offsetPos;

			istd::IChangeable* newObjectPtr = CreateObject();

			if (newObjectPtr != NULL){
				m_objectsQueue.insert(iter, newObjectPtr);

				return newObjectPtr;
			}
		}
	}

	return NULL;
}


istd::IChangeable* CObjectQueueComp::CreateBackObject(int offsetPos, const QByteArray* typeIdPtr)
{
	if (typeIdPtr == NULL){
		int objectsCount = int(m_objectsQueue.size());
		if (offsetPos <= objectsCount){
			ObjectQueue::iterator iter = m_objectsQueue.begin() + objectsCount - offsetPos;

			istd::IChangeable* newObjectPtr = CreateObject();

			if (newObjectPtr != NULL){
				m_objectsQueue.insert(iter, newObjectPtr);

				return newObjectPtr;
			}
		}
	}

	return NULL;
}


void CObjectQueueComp::RemoveFrontObject(int offsetPos, const QByteArray* typeIdPtr)
{
	Q_ASSERT(offsetPos < int(m_objectsQueue.size()));

	if (typeIdPtr == NULL){
		ObjectQueue::iterator iter = m_objectsQueue.begin() + offsetPos;
		istd::IChangeable* objectPtr = *iter;
		Q_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter);
		m_objectsReserve.push_front(objectPtr);

		TryReductReserve();
	}
}


void CObjectQueueComp::RemoveBackObject(int offsetPos, const QByteArray* typeIdPtr)
{
	Q_ASSERT(offsetPos < int(m_objectsQueue.size()));

	if (typeIdPtr == NULL){
		int objectsCount = int(m_objectsQueue.size());

		ObjectQueue::iterator iter = m_objectsQueue.begin() + objectsCount - 1 - offsetPos;
		istd::IChangeable* objectPtr = *iter;
		Q_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter);
		m_objectsReserve.push_front(objectPtr);

		TryReductReserve();
	}
}


istd::IChangeable* CObjectQueueComp::GetFrontObject(int offsetPos, const QByteArray* typeIdPtr) const
{
	Q_ASSERT(offsetPos >= 0);
	Q_ASSERT(offsetPos < int(m_objectsQueue.size()));

	if (typeIdPtr == NULL){
		istd::IChangeable* objectPtr = m_objectsQueue.at(offsetPos);
		Q_ASSERT(objectPtr != NULL);

		return objectPtr;
	}

	return NULL;
}


istd::IChangeable* CObjectQueueComp::GetBackObject(int offsetPos, const QByteArray* typeIdPtr) const
{
	Q_ASSERT(offsetPos >= 0);
	Q_ASSERT(offsetPos < int(m_objectsQueue.size()));

	if (typeIdPtr == NULL){
		int objectsCount = int(m_objectsQueue.size());
		istd::IChangeable* objectPtr = m_objectsQueue.at(objectsCount - 1 - offsetPos);
		Q_ASSERT(objectPtr != NULL);

		return objectPtr;

		return NULL;
	}

	return NULL;
}


void CObjectQueueComp::SelectObjects(
			ObjectList& result,
			bool doAppend,
			int offsetPos,
			const QByteArray* typeIdPtr) const
{
	if (!doAppend){
		result.clear();
	}

	if (typeIdPtr == NULL){
		for (		ObjectQueue::const_iterator iter = m_objectsQueue.begin();
					(iter != m_objectsQueue.end());
					++iter, --offsetPos){
			if (offsetPos > 0){
				continue;
			}

			result.push_back(*iter);
		}
	}
}


istd::IChangeable* CObjectQueueComp::PopFrontObject(int offsetPos, const QByteArray* typeIdPtr)
{
	Q_ASSERT(offsetPos >= 0);
	Q_ASSERT(offsetPos < int(m_objectsQueue.size()));

	if (typeIdPtr == NULL){
		ObjectQueue::iterator iter = m_objectsQueue.begin() + offsetPos;

		istd::IChangeable* objectPtr = *iter;
		Q_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter);

		return objectPtr;
	}

	return NULL;
}


istd::IChangeable* CObjectQueueComp::PopBackObject(int offsetPos, const QByteArray* typeIdPtr)
{
	Q_ASSERT(offsetPos >= 0);
	Q_ASSERT(offsetPos < int(m_objectsQueue.size()));

	if (typeIdPtr == NULL){
		int objectsCount = int(m_objectsQueue.size());
		ObjectQueue::iterator iter = m_objectsQueue.begin() + objectsCount - 1 - offsetPos;

		istd::IChangeable* objectPtr = *iter;
		Q_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter);

		return objectPtr;
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CObjectQueueComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag queueTag("Queue", "Set of objects in some queue", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag objectTag("Object", "Single queue object", iser::CArchiveTag::TT_GROUP, &queueTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	if (archive.IsStoring()){
		int objectsCount = int(m_objectsQueue.size());

		retVal = retVal && archive.BeginMultiTag(queueTag, objectTag, objectsCount);
		for (		ObjectQueue::const_iterator iter = m_objectsQueue.begin();
					iter != m_objectsQueue.end();
					++iter){
			iser::ISerializable* serializableObjectPtr = dynamic_cast<iser::ISerializable*>(*iter);
			if (serializableObjectPtr == NULL){
				return false;
			}

			retVal = retVal && archive.BeginTag(objectTag);
			retVal = retVal && serializableObjectPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(objectTag);
		}

		retVal = retVal && archive.EndTag(queueTag);
	}
	else{
		ClearQueue();

		int objectsCount = 0;

		retVal = retVal && archive.BeginMultiTag(queueTag, objectTag, objectsCount);

		if (!retVal){
			return false;
		}

		for (int i = 0; i < objectsCount; ++i){
			iser::ISerializable* serializableObjectPtr = dynamic_cast<iser::ISerializable*>(CreateFrontObject());
			if (serializableObjectPtr == NULL){
				return false;
			}

			retVal = retVal && archive.BeginTag(objectTag);
			retVal = retVal && serializableObjectPtr->Serialize(archive);
			retVal = retVal && archive.EndTag(objectTag);
		}

		retVal = retVal && archive.EndTag(queueTag);
	}

	return retVal;
}


// protected methods

istd::IChangeable* CObjectQueueComp::CreateObject()
{
	if (!m_objectFactoryFactPtr.IsValid()){
		return NULL;
	}

	if (m_objectsReserve.isEmpty()){
		return m_objectFactoryFactPtr.CreateInstance();
	}
	else{
		istd::IChangeable* retVal = m_objectsReserve.front();

		m_objectsReserve.pop_front();

		return retVal;
	}
}


void CObjectQueueComp::TryReductReserve()
{
	Q_ASSERT(m_maxReserveObjectsAttrPtr.IsValid());	// This attribute is not optional

	int maxReserveObjects = qMax(0, *m_maxReserveObjectsAttrPtr);

	while (int(m_objectsReserve.size()) > maxReserveObjects){
		delete m_objectsReserve.back();

		m_objectsReserve.pop_back();
	}
}


// reimplemented (icomp::CComponentBase)

void CObjectQueueComp::OnComponentDestroyed()
{
	while (!m_objectsReserve.isEmpty()){
		delete m_objectsReserve.back();

		m_objectsReserve.pop_back();
	}

	while (!m_objectsQueue.isEmpty()){
		delete m_objectsQueue.back();

		m_objectsQueue.pop_back();
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


