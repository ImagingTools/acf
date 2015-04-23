#include "istd/IChangeable.h"


namespace istd
{


IChangeable::ChangeSet::ChangeSet()
{
}


IChangeable::ChangeSet::ChangeSet(const QString& description)
:	m_description(description)
{
}


IChangeable::ChangeSet::ChangeSet(int id1, const QString& description)
:	m_description(description)
{
	m_ids << id1;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, int id5, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
	m_ids << id5;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
	m_ids << id5;
	m_ids << id6;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
	m_ids << id5;
	m_ids << id6;
	m_ids << id7;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, int id8, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
	m_ids << id5;
	m_ids << id6;
	m_ids << id7;
	m_ids << id8;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, int id8, int id9, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
	m_ids << id5;
	m_ids << id6;
	m_ids << id7;
	m_ids << id8;
	m_ids << id9;
}


IChangeable::ChangeSet::ChangeSet(int id1, int id2, int id3, int id4, int id5, int id6, int id7, int id8, int id9, int id10, const QString& description)
:	m_description(description)
{
	m_ids << id1;
	m_ids << id2;
	m_ids << id3;
	m_ids << id4;
	m_ids << id5;
	m_ids << id6;
	m_ids << id7;
	m_ids << id8;
	m_ids << id9;
	m_ids << id10;
}


void IChangeable::ChangeSet::Reset()
{
	m_ids.clear();
}


bool IChangeable::ChangeSet::IsEmpty() const
{
	return m_ids.empty();
}


bool IChangeable::ChangeSet::Contains(int changeId) const
{
	if (m_ids.contains(CF_ALL_DATA)){
		return true;
	}

	return m_ids.contains(changeId);
}


bool IChangeable::ChangeSet::ContainsExplicit(int changeId) const
{
	return m_ids.contains(changeId);
}


bool IChangeable::ChangeSet::ContainsAny(const ChangeSet& changeSet) const
{
	if (m_ids.contains(CF_ALL_DATA) && !changeSet.m_ids.isEmpty()){
		return true;
	}

	if (changeSet.m_ids.contains(CF_ALL_DATA) && !m_ids.isEmpty()){
		return true;
	}

	return !(m_ids & changeSet.m_ids).empty();
}


void IChangeable::ChangeSet::MaskOut(const ChangeSet& changeSet)
{
	m_ids -= m_ids & changeSet.m_ids;
}


IChangeable::ChangeSet IChangeable::ChangeSet::operator+(const ChangeSet& changeSet) const
{
	ChangeSet retVal;

	retVal.m_ids += m_ids;
	retVal.m_ids += changeSet.m_ids;

	if (m_description.isEmpty()){
		retVal.m_description = changeSet.m_description;
	}
	else{
		retVal.m_description = m_description;
	}

	return retVal;
}


IChangeable::ChangeSet& IChangeable::ChangeSet::operator+=(int changeId)
{
	m_ids << changeId;

	return *this;
}


IChangeable::ChangeSet& IChangeable::ChangeSet::operator+=(const ChangeSet& changeSet)
{
	m_ids += changeSet.m_ids;

	if (m_description.isEmpty()){
		m_description = changeSet.m_description;
	}

	return *this;
}


// static attributes

IChangeable::ChangeSet IChangeable::s_emptyChanges;
IChangeable::ChangeSet IChangeable::s_anyChanges(CF_ANY);
IChangeable::ChangeSet IChangeable::s_allChanges(CF_ALL_DATA);
IChangeable::ChangeSet IChangeable::s_delegatedChanges(CF_DELEGATED);


} // namespace istd


