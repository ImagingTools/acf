#ifndef ibase_TContainer_included
#define ibase_TContainer_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include "istd/IContainerInfo.h"

#include "istd/TChangeNotifier.h"


namespace ibase
{


/**
	Common implementation of an abstract container. 
*/
template <typename ItemClass>
class TContainer: virtual public istd::IContainerInfo
{
public:
	enum ChangeFlags
	{
		CF_ELEMENT_ADDED = 1 << 21,
		CF_ELEMENT_REMOVED = 1 << 22,
		CF_RESET = 1 << 23
	};

	const ItemClass& GetAt(int index) const;
	ItemClass& GetAt(int index);
	ItemClass& PushBack(const ItemClass& item);
	ItemClass& PushFront(const ItemClass& item);
	ItemClass& InsertAt(const ItemClass& item, int index);
	void PopBack();
	void PopFront();
	void RemoveAt(int index);
	virtual void Reset();

	TContainer& operator=(const TContainer& container);

	// reimplemented (istd::IContainerInfo)
	virtual int GetItemsCount() const;
	virtual bool IsEmpty() const;
	virtual bool IsIndexValid(int index) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

protected:
	typedef QList<ItemClass> Items;

	Items m_items;
};


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::GetAt(int index)
{
	Q_ASSERT(TContainer<ItemClass>::IsIndexValid(index));

	return m_items[index];
}


template <typename ItemClass>
const ItemClass& TContainer<ItemClass>::GetAt(int index) const
{
	Q_ASSERT(TContainer<ItemClass>::IsIndexValid(index));

	return m_items[index];
}


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::PushBack(const ItemClass& item)
{
	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_ELEMENT_ADDED);

	m_items.append(item);

	return m_items.back();
}


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::PushFront(const ItemClass& item)
{
	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_ELEMENT_ADDED);

	m_items.prepend(item);

	return m_items.front();
}


template <typename ItemClass>
ItemClass& TContainer<ItemClass>::InsertAt(const ItemClass& item, int index)
{
	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_ELEMENT_ADDED);

	if ((index < 0) || (index >= m_items.size())){
		m_items.append(item);

		return m_items.back();
	}
	else{
		m_items.insert(index, item);

		return m_items[index];
	}
}


template <typename ItemClass>
void TContainer<ItemClass>::PopBack()
{
	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_ELEMENT_REMOVED);

	m_items.pop_back();
}


template <typename ItemClass>
void TContainer<ItemClass>::PopFront()
{
	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_ELEMENT_REMOVED);

	m_items.pop_front();
}


template <typename ItemClass>
void TContainer<ItemClass>::RemoveAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_items.size()));

	if (index < int(m_items.size())){
		istd::CChangeNotifier changePtr(this, CF_MODEL | CF_ELEMENT_REMOVED);
	
		m_items.erase(m_items.begin()  + index);
	}
}


template <typename ItemClass>
void TContainer<ItemClass>::Reset()
{
	istd::CChangeNotifier changePtr(this, CF_MODEL | CF_RESET);

	m_items.clear();
}


template <typename ItemClass>
TContainer<ItemClass>& TContainer<ItemClass>::operator=(const TContainer& container)
{
	m_items = container.m_items;

	return *this;
}


// reimplemented (istd::IContainerInfo)

template <typename ItemClass>
int TContainer<ItemClass>::GetItemsCount() const
{
	return int(m_items.size());
}


template <typename ItemClass>
bool TContainer<ItemClass>::IsEmpty() const
{
	return m_items.isEmpty();
}


template <typename ItemClass>
bool TContainer<ItemClass>::IsIndexValid(int index) const
{
	return (index >= 0 && index < int(m_items.size()));
}


// reimplemented (istd::IChangeable)

template <typename ItemClass>
bool TContainer<ItemClass>::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const TContainer* containerPtr = dynamic_cast<const TContainer*>(&object);
	if (containerPtr != NULL){
		m_items = containerPtr->m_items;

		return true;
	}

	return false;
}


} // namespace ibase


#endif // !ibase_TContainer_included


