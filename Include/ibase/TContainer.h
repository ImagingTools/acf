#pragma once


// Qt includes
#include <QtCore/QList>

// ACF includes
#include <istd/IContainerInfo.h>
#include <istd/CChangeNotifier.h>
#include <ibase/CObservableListBase.h>

namespace ibase
{


/**
	Common implementation of an abstract container.
	
	\note	CF_ELEMENT_UPDATED notification is not implemented, 
			because this container provides direct mutable access to its members.
			Inherit with protected visibility or aggregate this class and emit it there.
*/
template <typename ItemClass, class ContainerType = QList<ItemClass> >
class TContainer: virtual public istd::IContainerInfo, public CObservableListBase
{
public:
	typedef ContainerType Container;

	virtual void Reserve(int count);
	virtual void Resize(int count);
	virtual void RemoveAt(int index);
	virtual void Reset();

	const ItemClass& GetAt(int index) const;	
	ItemClass& GetAt(int index);
	ItemClass& PushBack(const ItemClass& item);
	ItemClass& PushFront(const ItemClass& item);
	ItemClass& InsertAt(const ItemClass& item, int index);
	void PopBack();
	void PopFront();

	// methods with names compatible to standard containers

	inline void push_back(const ItemClass& item)
	{
		PushBack(item);
	}


	inline void push_front(const ItemClass& item)
	{
		PushFront(item);
	}


	inline ItemClass& operator[](qsizetype index)
	{
		return m_items[index];
	}


	inline const ItemClass& operator[](qsizetype index) const
	{
		return m_items[index];
	}


	typename Container::iterator insert(qsizetype index, const ItemClass& item)
	{
		auto changes = ElementAddChanges(index);
		istd::CChangeNotifier notifier(this, &changes);

		return m_items.insert(m_items.begin() + index, item);
	}


	typename Container::iterator insert(qsizetype index, ItemClass&& item)
	{
		auto changes = ElementAddChanges(index);
		istd::CChangeNotifier notifier(this, &changes);

		return m_items.insert(m_items.begin() + index, std::move(item));
	}

	// remove of multiple elements is not implemented yet
	void remove(qsizetype i)
	{
		Q_UNUSED(i);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		auto changes = ElementRemoveChanges(i);
		istd::CChangeNotifier notifier(this, &changes);

		m_items.remove(i);
#endif
	}


	void resize(qsizetype size)
	{
		Q_UNUSED(size);

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
		istd::CChangeNotifier notifier(this, &s_resetChange);

		m_items.resize(size);
#endif
	}


	inline qsizetype size() const
	{
		return m_items.size();
	}


	TContainer& operator=(const TContainer& container);

	// reimplemented (istd::IContainerInfo)
	virtual int GetItemsCount() const override;
	virtual bool IsEmpty() const override;
	virtual bool IsIndexValid(int index) const override;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;

	// Iterators
	typename Container::iterator begin()
	{
		return m_items.begin();
	}


	typename Container::iterator end()
	{
		return m_items.end();
	}


	typename Container::const_iterator begin() const
	{
		return m_items.begin();
	}


	typename Container::const_iterator end() const
	{
		return m_items.end();
	}


	typename Container::const_iterator cbegin() const
	{
		return m_items.begin();
	}


	typename Container::const_iterator cend() const
	{
		return m_items.end();
	}


	// Equality
	bool operator==(const TContainer& other) const
	{
		return m_items == other.m_items;
	}

	bool operator!=(const TContainer& other) const
	{
		return !operator==(other);
	}

protected:
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
	auto changes = ElementAddChanges(m_items.size());
	istd::CChangeNotifier notifier(this, &changes);
	Q_UNUSED(notifier);

	m_items.push_back(item);

	return m_items.back();
}


template <typename ItemClass, typename ContainerType>
ItemClass& TContainer<ItemClass, ContainerType>::PushFront(const ItemClass& item)
{
	auto changes = ElementAddChanges(0);
	istd::CChangeNotifier notifier(this, &changes);
	Q_UNUSED(notifier);

	m_items.push_front(item);

	return m_items.front();
}


template <typename ItemClass, typename ContainerType>
ItemClass& TContainer<ItemClass, ContainerType>::InsertAt(const ItemClass& item, int index)
{
	if ((index < 0) || (index >= m_items.size())){
		return PushBack(item);
	}
	else{
		auto changes = ElementAddChanges(index);
		istd::CChangeNotifier notifier(this, &changes);
		m_items.insert(index, item);

		return m_items[index];
	}
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::PopBack()
{
	auto changes = ElementRemoveChanges(m_items.size() - 1);
	istd::CChangeNotifier notifier(this, &changes);
	Q_UNUSED(notifier);

	m_items.pop_back();
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::PopFront()
{
	auto changes = ElementRemoveChanges(0);
	istd::CChangeNotifier notifier(this, &changes);
	Q_UNUSED(notifier);

	m_items.pop_front();
}


template <typename ItemClass, typename ContainerType>
void TContainer<ItemClass, ContainerType>::RemoveAt(int index)
{
	Q_ASSERT(index >= 0);
	Q_ASSERT(index < int(m_items.size()));

	if (index < int(m_items.size())){
		auto changes = ElementRemoveChanges(index);
		istd::CChangeNotifier notifier(this, &changes);
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
	istd::CChangeNotifier notifier(this, &s_resetChange);
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
const istd::IChangeable::ChangeSet TContainer<ItemClass, ContainerType>::s_resetChange(CF_RESET);


} // namespace ibase


