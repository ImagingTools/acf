#ifndef ibase_CObjectSynchronizerComp_included
#define ibase_CObjectSynchronizerComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <imod/CSingleModelObserverBase.h>
#include <istd/IChangeable.h>


namespace ibase
{


/**
	Component for synchronization between some master data object and its "slaves".
	On component initialization the slave objects are initialized with data of "master" object.
	On each change of the "master" data, the slave objects will be automatically synchronized.

	\ingroup DataModel
*/
class CObjectSynchronizerComp:
			public icomp::CComponentBase,
			protected imod::CSingleModelObserverBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CObjectSynchronizerComp);
		I_ASSIGN(m_referenceObjectCompPtr, "ReferenceObject", "Reference data object", true, "ReferenceObject");
		I_ASSIGN_TO(m_referenceObjectModelCompPtr, m_referenceObjectCompPtr, true);
		I_ASSIGN_MULTI_0(m_slaveObjectsCompPtr, "SlaveObjects", "The list of object which will be hold synchronized with reference object", true);
	I_END_COMPONENT;
	
protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(istd::IChangeable, m_referenceObjectCompPtr);
	I_REF(imod::IModel, m_referenceObjectModelCompPtr);
	I_MULTIREF(istd::IChangeable, m_slaveObjectsCompPtr);
};


} // namespace ibase


#endif // !ibase_CObjectSynchronizerComp_included


