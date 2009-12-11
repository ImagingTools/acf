#ifndef ibase_CObjectQueueComp_included
#define ibase_CObjectQueueComp_included


// STL includes
#include <list>

#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "ibase/IObjectQueue.h"


namespace ibase
{


/**
	Implementation of ibase::IObjectQueue including some smart optimizations.
*/
class CObjectQueueComp:
			public icomp::CComponentBase,
			virtual public IObjectQueue,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CObjectQueueComp)
		I_REGISTER_INTERFACE(IObjectQueue);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_objectFactoryFactPtr, "ObjectFactory", "Object factory used to create queue instancies", true, "ObjectFactory");
		I_ASSIGN(m_maxReserveObjectsAttrPtr, "MaxReserveObjects", "Maximal number of reserve objects used to avoid cretion and removing of objects from heap", true, 10);
	I_END_COMPONENT

	// reimplemented (ibase::IObjectQueue)
	virtual int GetObjectsCount(const std::string* typeIdPtr = NULL) const;
	virtual void ClearQueue();
	virtual istd::IChangeable* CreateFrontObject(int offsetPos = 0, const std::string* typeIdPtr = NULL);
	virtual istd::IChangeable* CreateBackObject(int offsetPos = 0, const std::string* typeIdPtr = NULL);
	virtual void RemoveFrontObject(int offsetPos = 0, const std::string* typeIdPtr = NULL);
	virtual void RemoveBackObject(int offsetPos = 0, const std::string* typeIdPtr = NULL);
	virtual istd::IChangeable* GetFrontObject(int offsetPos = 0, const std::string* typeIdPtr = NULL) const;
	virtual istd::IChangeable* GetBackObject(int offsetPos = 0, const std::string* typeIdPtr = NULL) const;
	virtual void SelectObjects(
				ObjectList& result,
				bool doAppend = false,
				int offsetPos = 0,
				const std::string* typeIdPtr = NULL) const;
	virtual istd::IChangeable* PopFrontObject(int offsetPos = 0, const std::string* typeIdPtr = NULL);
	virtual istd::IChangeable* PopBackObject(int offsetPos = 0, const std::string* typeIdPtr = NULL);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	void TryReductReserve();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentDestroyed();

private:
	I_FACT(istd::IChangeable, m_objectFactoryFactPtr);
	I_ATTR(int, m_maxReserveObjectsAttrPtr);

	typedef std::list<istd::IChangeable*> ObjectQueue;

	ObjectQueue m_objectsQueue;
	ObjectQueue m_objectsReserve;
};


} // namespace ibase


#endif // !ibase_CObjectQueueComp_included


