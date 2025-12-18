#pragma once


// ACF includes
#include <ibase/CObservableListBase.h>
#include <imod/CSingleModelObserverBase.h>


namespace ibase
{


/**
	Base implementation for observing a list.
	Routes ChangeSet change infos to specific virtual method
*/
class CListObserver: public imod::CSingleModelObserverBase
{
public:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

protected:
	virtual void OnListReset() = 0;
	virtual void OnBeforeElementRemoved(qsizetype index) = 0;
	virtual void OnAfterElementAdded(qsizetype index) = 0;
	virtual void OnAfterElementUpdated(qsizetype index) = 0;
};


} // namespace ibase


