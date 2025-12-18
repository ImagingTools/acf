#pragma once


// ACF includes
#include <istd/IChangeable.h>


namespace ibase
{


/**
	Base implementation for creating simple observable collection.
	Contains helper methods for constructing ChangeSets.

	\sa CListObserver
 */
class CObservableListBase
{
public:
	enum ChangeFlags
	{
		CF_ELEMENT_ADDED = 0x382b230,
		CF_ELEMENT_REMOVED,
		CF_ELEMENT_UPDATED,
		CF_RESET
	};

	inline static const QByteArray CN_INDEX_ID = QByteArrayLiteral("INDEX");

protected:
	static istd::IChangeable::ChangeSet ElementAddChanges(qsizetype index);
	static istd::IChangeable::ChangeSet ElementRemoveChanges(qsizetype index);
	static istd::IChangeable::ChangeSet ElementUpdatedChanges(qsizetype index);
};


} // namespace ibase


