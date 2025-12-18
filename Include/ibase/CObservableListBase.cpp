#include <ibase/CObservableListBase.h>


namespace ibase
{


// protected static methods

istd::IChangeable::ChangeSet CObservableListBase::ElementAddChanges(qsizetype index)
{
	istd::IChangeable::ChangeSet changeSet(CF_ELEMENT_ADDED);
	changeSet.SetChangeInfo(CN_INDEX_ID, index);

	return changeSet;
}


istd::IChangeable::ChangeSet CObservableListBase::ElementRemoveChanges(qsizetype index)
{
	istd::IChangeable::ChangeSet changeSet(CF_ELEMENT_REMOVED);
	changeSet.SetChangeInfo(CN_INDEX_ID, index);

	return changeSet;
}


istd::IChangeable::ChangeSet CObservableListBase::ElementUpdatedChanges(qsizetype index)
{
	istd::IChangeable::ChangeSet changeSet(CF_ELEMENT_UPDATED);
	changeSet.SetChangeInfo(CN_INDEX_ID, index);

	return changeSet;
}


} // namespace ibase


