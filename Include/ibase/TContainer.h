#ifndef ibase_TContainer_included
#define ibase_TContainer_included


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/IContainerInfo.h>
#include <istd/CChangeNotifier.h>


namespace ibase
{


/**
	Common implementation of an abstract container. 
*/
template <typename ItemClass, class ContainerType = QList<ItemClass> >
class TContainer: virtual public istd::IContainerInfo
{
public:
	typedef ContainerType Container;

	enum ChangeFlags
	{
		CF_ELEMENT_ADDED = 0x382b230,
		CF_ELEMENT_REMOVED,
		CF_RESET
	};

	virtual void Reserve(int count);
	virtual void Resize(int count);

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
	static const ChangeSet s_elementAddChange;
	static const ChangeSet s_elementRemoveChange;
	static const ChangeSet s_resetChange;

	typedef ContainerType Items;
	
	Items m_items;
};


// public methods

template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::Reserve(int /*count*/)
{
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::Resize(int /*count*/)
{
}


template <typename ItemClass, typename ContainerType>
ItemClass& TContainer<ItemClass, ContainerType>::GetAt(int index)
{
	Q_ASSERT(TContainer::IsIndexValid(index));

	return m_items[index];
}


template <typename ItemClass, typename ContainerType>
const ItemClass& TContainer<ItemClass, ContainerType>::GetAt(int index) const
{
	Q_ASSERT(TContainer::IsIndexValid(index));

	return m_items[index];
}


template <typename ItemClass, typename ContainerType>
ItemClass& TContainer<ItemClass, ContainerType>::PushBack(const ItemClass& item)
{
	istd::CChangeNotifier notifier(this, &s_elementAddChange);
	Q_UNUSED(notifier);

	m_items.push_back(item);

	return m_items.back();
}


template <typename ItemClass, typename ContainerType>
ItemClass& TContainer<ItemClass, ContainerType>::PushFront(const ItemClass& item)
{
	istd::CChangeNotifier notifier(this, &s_elementAddChange);
	Q_UNUSED(notifier);

	m_items.push_front(item);

	return m_items.front();
}


template <typename ItemClass, typename ContainerType>
ItemClass& TContainer<ItemClass, ContainerType>::InsertAt(const ItemClass& item, int index)
{
	istd::CChangeNotifier notifier(this, &s_elementAddChange);
	Q_UNUSED(notifier);

	if ((index < 0) || (index >= m_items.size())){
		m_items.push_back(item);

		return m_items.back();
	}
	else{
		m_items.insert(index, item);

		return m_items[index];
	}
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::PopBack()
{
	istd::CChangeNotifier notifier(this, &s_elementRemoveChange);
	Q_UNUSED(notifier);

	m_items.pop_back();
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::PopFront()
{
	istd::CChangeNotifier notifier(this, &s_elementRemoveChange);
	Q_UNUSED(notifier);

	m_items.pop_front();
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::RemoveAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_items.size()));

	if (index < int(m_items.size())){
		istd::CChangeNotifier notifier(this, &s_elementRemoveChange);
		Q_UNUSED(notifier);
	
		m_items.erase(m_items.begin()  + index);
	}
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::Reset()
{
	istd::CChangeNotifier notifier(this, &s_resetChange);
	Q_UNUSED(notifier);

	m_items.clear();
}


template <typename ItemClass, typename ContainerType>
TContainer<ItemClass, ContainerType>& TContainer<ItemClass, ContainerType>::operator=(const TContainer& container)
{
	m_items = container.m_items;

	return *this;
}


// reimplemented (istd::IContainerInfo)

template <typename ItemClass, typename ContainerType>
int TContainer<ItemClass, ContainerType>::GetItemsCount() const
{
	return int(m_items.size());
}


template <typename ItemClass, typename ContainerType>
bool TContainer<ItemClass, ContainerType>::IsEmpty() const
{
	return (m_items.size() == 0);
}


template <typename ItemClass, typename ContainerType>
bool TContainer<ItemClass, ContainerType>::IsIndexValid(int index) const
{
	return (index >= 0 && index < int(m_items.size()));
}


// reimplemented (istd::IChangeable)

template <typename ItemClass, typename ContainerType>
bool TContainer<ItemClass, ContainerType>::CopyFrom(const IChangeable& object, CompatibilityMode /*mode*/)
{
	const TContainer* containerPtr = dynamic_cast<const TContainer*>(&object);
	if (containerPtr != NULL){
		m_items = containerPtr->m_items;

		return true;
	}

	return false;
}


// protected static members

template <typename ItemClass, typename ContainerType>
const istd::IChangeable::ChangeSet TContainer<ItemClass, ContainerType>::s_elementAddChange(CF_ELEMENT_ADDED);

template <typename ItemClass, typename ContainerType>
const istd::IChangeable::ChangeSet TContainer<ItemClass, ContainerType>::s_elementRemoveChange(CF_ELEMENT_REMOVED);

template <typename ItemClass, typename ContainerType>
const istd::IChangeable::ChangeSet TContainer<ItemClass, ContainerType>::s_resetChange(CF_RESET);


} // namespace ibase


#endif // !ibase_TContainer_included


