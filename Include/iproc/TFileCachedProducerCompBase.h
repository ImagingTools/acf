#ifndef iproc_TFileCachedProducerCompBase_included
#define iproc_TFileCachedProducerCompBase_included

// Old C includes
#include <stdio.h>

// STL includes
#include <list>

// ACF includes
#include "iser/IFileLoader.h"
#include "icomp/CComponentBase.h"

#include "iproc/TILockedProducer.h"


namespace iproc
{


/**
	Template base implementation iproc::TILockedProducer caching (buffering) objects using file system.
	Please note, that no memory caching is done - if you ask second time of existing (locked) object,
	second instance will be created and loaded.
*/
template <class Key, class CacheObject>
class TFileCachedProducerCompBase:
			public icomp::CComponentBase,
			virtual public TILockedProducer<Key, CacheObject>
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef TILockedProducer<Key, CacheObject> LockedProducerType;

	I_BEGIN_BASE_COMPONENT(TFileCachedProducerCompBase);
		I_REGISTER_INTERFACE(LockedProducerType);
		I_ASSIGN(m_cacheLoaderCompPtr, "CacheLoader", "Loads and saves cached object to temporary file", true, "CacheLoader");
		I_TASSIGN(m_slaveCacheEngineCompPtr, "SlaveCacheEngine", "Slave cache engine providing access to cached object", true, "SlaveCacheEngine");
	I_END_COMPONENT;

	// reimplemented (iproc::TILockedProducer)
	virtual const CacheObject* ProduceLockedObject(const Key& key);
	virtual void UnlockObject(const CacheObject* objectPtr);

protected:
	bool PushKeyBack(const Key& key);

	/**
		Removes cache files if there is too many of them.
	*/
	void CleanFileList();

	// methods for optional ovelloading
	virtual void OnCacheFileSaved(const Key& key, const QString& cacheFilePath);
	virtual void OnCacheFileRemoved(const Key& key, const QString& cacheFilePath);

	// abstract methods
	/**
		Calculate unique file path of temporary file cache object.
	*/
	virtual QString CalcCacheFilePath(const Key& key) const = 0;
	/**
		Get number of maximal cached files.
	*/
	virtual int GetMaxCachedFilesCount() const = 0;

private:
	I_REF(iser::IFileLoader, m_cacheLoaderCompPtr);
	I_TREF(LockedProducerType, m_slaveCacheEngineCompPtr);

	typedef QMap<Key, QString> KeyToFileNameMap;
	typedef std::list<Key> RecentlyUsedKeys;
	typedef std::set<const CacheObject*> OwnedObjects;

	mutable KeyToFileNameMap m_keyToFileNameMap;
	mutable RecentlyUsedKeys m_recentlyUsedKeys;
	mutable OwnedObjects m_ownedObjects;
};


// public methods

// reimplemented (iproc::TILockedProducer)

template <class Key, class CacheObject>
const CacheObject* TFileCachedProducerCompBase<Key, CacheObject>::ProduceLockedObject(const Key& key)
{
	if (m_cacheLoaderCompPtr.IsValid()){
		QString cacheFilePath;
		typename KeyToFileNameMap::iterator foundIter = m_keyToFileNameMap.find(key);
		if (foundIter != m_keyToFileNameMap.end()){
			cacheFilePath = foundIter.value();
			I_ASSERT(!cacheFilePath.isEmpty());
		}
		else{
			cacheFilePath = CalcCacheFilePath(key);
		}

		if (!cacheFilePath.isEmpty()){
			if (m_cacheLoaderCompPtr->IsOperationSupported(
						NULL,
						&cacheFilePath,
						iser::IFileLoader::QF_LOAD | iser::IFileLoader::QF_FILE,
						false)){
				istd::TDelPtr<CacheObject> objectPtr(new CacheObject);

				if (objectPtr.IsValid()){
					if (m_cacheLoaderCompPtr->LoadFromFile(*objectPtr, cacheFilePath) == iser::IFileLoader::StateOk){
						m_ownedObjects.insert(objectPtr.GetPtr());

						return objectPtr.PopPtr();
					}
					else if (foundIter != m_keyToFileNameMap.end()){
						m_keyToFileNameMap.erase(foundIter);
					}
				}
			}
		}
	}

	if (m_slaveCacheEngineCompPtr.IsValid()){
		const CacheObject* objectPtr = m_slaveCacheEngineCompPtr->ProduceLockedObject(key);
		if (objectPtr != NULL){
			QString cacheFilePath = CalcCacheFilePath(key);
			if (!cacheFilePath.isEmpty() && m_cacheLoaderCompPtr.IsValid()){
				if (m_cacheLoaderCompPtr->SaveToFile(*const_cast<CacheObject*>(objectPtr), cacheFilePath) == iser::IFileLoader::StateOk){
					OnCacheFileSaved(key, cacheFilePath);

					m_keyToFileNameMap[key] = cacheFilePath;
					m_recentlyUsedKeys.push_back(key);

					const_cast<TFileCachedProducerCompBase<Key, CacheObject>&>(*this).CleanFileList();
				}
			}

			return objectPtr;
		}
	}

	return NULL;
}


template <class Key, class CacheObject>
void TFileCachedProducerCompBase<Key, CacheObject>::UnlockObject(const CacheObject* objectPtr)
{
	I_ASSERT(objectPtr != NULL);

	typename OwnedObjects::iterator foundIter = m_ownedObjects.find(objectPtr);
	if (foundIter != m_ownedObjects.end()){
		m_ownedObjects.erase(objectPtr);

		delete objectPtr;

		return;
	}

	if (m_slaveCacheEngineCompPtr.IsValid()){
		m_slaveCacheEngineCompPtr->UnlockObject(objectPtr);
	}
}


// protected methods

template <class Key, class CacheObject>
bool TFileCachedProducerCompBase<Key, CacheObject>::PushKeyBack(const Key& key)
{
	if (m_keyToFileNameMap.find(key) == m_keyToFileNameMap.end()){
		QString cacheFilePath = CalcCacheFilePath(key);
		if (!cacheFilePath.isEmpty()){
			m_keyToFileNameMap[key] = cacheFilePath;
			m_recentlyUsedKeys.push_back(key);

			return true;
		}
	}

	return false;
}


template <class Key, class CacheObject>
void TFileCachedProducerCompBase<Key, CacheObject>::CleanFileList()
{
	int maxCachedFiles = GetMaxCachedFilesCount();
	I_ASSERT(maxCachedFiles >= 0);

	while (int(m_recentlyUsedKeys.size()) > maxCachedFiles){
		const QString& key = m_recentlyUsedKeys.front();

		typename KeyToFileNameMap::iterator foundIter = m_keyToFileNameMap.find(key);
		if (foundIter != m_keyToFileNameMap.end()){
			const QString& cacheFilePath = foundIter.value();

			if (remove(cacheFilePath.toStdString().c_str()) != 0){
				OnCacheFileRemoved(key, cacheFilePath);
			}

			m_keyToFileNameMap.erase(foundIter);
		}

		m_recentlyUsedKeys.pop_front();
	}
}


template <class Key, class CacheObject>
void TFileCachedProducerCompBase<Key, CacheObject>::OnCacheFileSaved(const Key& /*key*/, const QString& /*cacheFilePath*/)
{
}


template <class Key, class CacheObject>
void TFileCachedProducerCompBase<Key, CacheObject>::OnCacheFileRemoved(const Key& /*key*/, const QString& /*cacheFilePath*/)
{
}


} // namespace iproc


#endif // !iproc_TFileCachedProducerCompBase_included


