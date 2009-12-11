#include "ibase/CObjectQueueComp.h"


#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "ibase/IObjectQueue.h"


namespace ibase
{


// reimplemented (ibase::IObjectQueue)

int CObjectQueueComp::GetObjectsCount(const std::string* typeIdPtr) const
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
	while (!m_objectsQueue.empty()){
		m_objectsReserve.push_front(m_objectsQueue.back());

		m_objectsQueue.pop_back();
	}

	TryReductReserve();
}


istd::IChangeable* CObjectQueueComp::CreateFrontObject(int offsetPos, const std::string* typeIdPtr)
{
	if (!m_objectFactoryFactPtr.IsValid()){
		return NULL;
	}

	if (typeIdPtr != NULL){
		return NULL;
	}
	else{
		ObjectQueue::iterator iter = m_objectsQueue.begin();
		while (offsetPos > 0){
			if (iter == m_objectsQueue.end()){
				return NULL;
			}

			iter++;
			offsetPos--;
		}

		istd::IChangeable* newObjectPtr = NULL;

		if (m_objectsReserve.empty()){
			newObjectPtr = m_objectFactoryFactPtr.CreateInstance();

			if (newObjectPtr == NULL){
				return NULL;
			}
		}
		else{
			newObjectPtr = m_objectsReserve.front();

			m_objectsReserve.pop_front();
		}

		I_ASSERT(newObjectPtr != NULL);

		m_objectsQueue.insert(iter, newObjectPtr);

		return newObjectPtr;
	}
}


istd::IChangeable* CObjectQueueComp::CreateBackObject(int offsetPos, const std::string* typeIdPtr)
{
	if (!m_objectFactoryFactPtr.IsValid()){
		return NULL;
	}

	if (typeIdPtr != NULL){
		return NULL;
	}
	else{
		ObjectQueue::reverse_iterator iter = m_objectsQueue.rbegin();
		while (offsetPos > 0){
			if (iter == m_objectsQueue.rend()){
				return NULL;
			}

			iter++;
			offsetPos--;
		}

		istd::IChangeable* newObjectPtr = NULL;

		if (m_objectsReserve.empty()){
			newObjectPtr = m_objectFactoryFactPtr.CreateInstance();

			if (newObjectPtr == NULL){
				return NULL;
			}
		}
		else{
			newObjectPtr = m_objectsReserve.front();

			m_objectsReserve.pop_front();
		}

		I_ASSERT(newObjectPtr != NULL);

		m_objectsQueue.insert(iter.base(), newObjectPtr);

		return newObjectPtr;
	}
}


void CObjectQueueComp::RemoveFrontObject(int offsetPos, const std::string* typeIdPtr)
{
	if (typeIdPtr == NULL){
		ObjectQueue::iterator iter = m_objectsQueue.begin();
		while (offsetPos > 0){
			iter++;
			offsetPos--;
		}

		if (iter == m_objectsQueue.end()){
			return;
		}

		istd::IChangeable* objectPtr = *iter;
		I_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter);
		m_objectsReserve.push_front(objectPtr);

		TryReductReserve();
	}
}


void CObjectQueueComp::RemoveBackObject(int offsetPos, const std::string* typeIdPtr)
{
	if (typeIdPtr == NULL){
		ObjectQueue::reverse_iterator iter = m_objectsQueue.rbegin();
		while (offsetPos > 0){
			iter++;
			offsetPos--;
		}

		if (iter == m_objectsQueue.rend()){
			return;
		}

		istd::IChangeable* objectPtr = *iter;
		I_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter.base());
		m_objectsReserve.push_front(objectPtr);

		TryReductReserve();
	}
}


istd::IChangeable* CObjectQueueComp::GetFrontObject(int offsetPos, const std::string* typeIdPtr) const
{
	if (typeIdPtr == NULL){
		ObjectQueue::const_iterator iter = m_objectsQueue.begin();
		while (offsetPos > 0){
			iter++;
			offsetPos--;
		}

		if (iter == m_objectsQueue.end()){
			return NULL;
		}

		istd::IChangeable* objectPtr = *iter;
		I_ASSERT(objectPtr != NULL);

		return objectPtr;
	}

	return NULL;
}


istd::IChangeable* CObjectQueueComp::GetBackObject(int offsetPos, const std::string* typeIdPtr) const
{
	if (typeIdPtr == NULL){
		ObjectQueue::const_reverse_iterator iter = m_objectsQueue.rbegin();
		while (offsetPos > 0){
			iter++;
			offsetPos--;
		}

		if (iter == m_objectsQueue.rend()){
			return NULL;
		}

		istd::IChangeable* objectPtr = *iter;
		I_ASSERT(objectPtr != NULL);

		return objectPtr;
	}

	return NULL;
}


void CObjectQueueComp::SelectObjects(
			ObjectList& result,
			bool doAppend,
			int offsetPos,
			const std::string* typeIdPtr) const
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


istd::IChangeable* CObjectQueueComp::PopFrontObject(int offsetPos, const std::string* typeIdPtr)
{
	if (typeIdPtr == NULL){
		ObjectQueue::iterator iter = m_objectsQueue.begin();
		while (offsetPos > 0){
			iter++;
			offsetPos--;
		}

		if (iter != m_objectsQueue.end()){
			return NULL;
		}

		istd::IChangeable* objectPtr = *iter;
		I_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter);

		return objectPtr;
	}

	return NULL;
}


istd::IChangeable* CObjectQueueComp::PopBackObject(int offsetPos, const std::string* typeIdPtr)
{
	if (typeIdPtr == NULL){
		ObjectQueue::reverse_iterator iter = m_objectsQueue.rbegin();
		while (offsetPos > 0){
			iter++;
			offsetPos--;
		}

		if (iter != m_objectsQueue.rend()){
			return NULL;
		}

		istd::IChangeable* objectPtr = *iter;
		I_ASSERT(objectPtr != NULL);

		m_objectsQueue.erase(iter.base());

		return objectPtr;
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CObjectQueueComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag queueTag("Queue", "Set of objects in some queue");
	static iser::CArchiveTag objectTag("Object", "Single queue object");

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

void CObjectQueueComp::TryReductReserve()
{
	I_ASSERT(m_maxReserveObjectsAttrPtr.IsValid());	// This attribute is not optional

	int maxReserveObjects = istd::Max(0, *m_maxReserveObjectsAttrPtr);

	while (int(m_objectsReserve.size()) > maxReserveObjects){
		delete m_objectsReserve.back();

		m_objectsReserve.pop_back();
	}
}


// reimplemented (icomp::IComponent)

void CObjectQueueComp::OnComponentDestroyed()
{
	while (!m_objectsReserve.empty()){
		delete m_objectsReserve.back();

		m_objectsReserve.pop_back();
	}

	while (!m_objectsQueue.empty()){
		delete m_objectsQueue.back();

		m_objectsQueue.pop_back();
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


