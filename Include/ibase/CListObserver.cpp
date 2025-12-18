#include <ibase/CListObserver.h>


namespace ibase
{


// public methods

// reimplemented (imod::CSingleModelObserverBase)

void CListObserver::OnUpdate(const istd::IChangeable::ChangeSet& changeSet)
{
	if (changeSet.Contains(CObservableListBase::CF_ELEMENT_ADDED)) {
		OnAfterElementAdded(changeSet.GetChangeInfo(CObservableListBase::CN_INDEX_ID).toInt());
	}
	if (changeSet.Contains(CObservableListBase::CF_ELEMENT_REMOVED)) {
		OnBeforeElementRemoved(changeSet.GetChangeInfo(CObservableListBase::CN_INDEX_ID).toInt());
	}
	if (changeSet.Contains(CObservableListBase::CF_ELEMENT_UPDATED)) {
		OnAfterElementUpdated(changeSet.GetChangeInfo(CObservableListBase::CN_INDEX_ID).toInt());
	}
	if (changeSet.Contains(istd::IChangeable::CF_ALL_DATA) || changeSet.Contains(CObservableListBase::CF_RESET)) {
		OnListReset();
	}
}


} // namespace ibase


