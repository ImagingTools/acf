#ifndef iproc_TILockedProducer_included
#define iproc_TILockedProducer_included


// ACF includes
#include "istd/IPolymorphic.h"

#include "iproc/iproc.h"


namespace iproc
{


/**
	Template interface for providers of cached data.
	Cached data are accessible in transaction blocks between lock and unlock operation.
	This data are accessed using abstract key object.
	Please note that \c Key class must implement standard STL operators needed to use it as QMap key.
*/
template <class Key, class CacheObject>
class TILockedProducer: virtual public istd::IPolymorphic
{
public:
	/**
		Begin of accessing to cached element.
		\return	cached object, if it was possible to produce it for specified key.
	*/
	virtual const CacheObject* ProduceLockedObject(const Key& key) = 0;

	/**
		End of accessing to cached element.
	*/
	virtual void UnlockObject(const CacheObject* objectPtr) = 0;
};


} // namespace iproc


#endif // !iproc_TILockedProducer_included


