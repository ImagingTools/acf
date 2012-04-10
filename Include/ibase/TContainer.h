#ifndef ibase_TContainer_included
#define ibase_TContainer_included


// Qt includes
#include <QtCore/QVector>

// ACF includes
#include "istd/IContainerInfo.h"
#include "istd/IChangeable.h"

#include "istd/TChangeNotifier.h"


namespace ibase
{


/**
	Common implementation of an abstract container. 
*/
template <typename ItemClass, typename ContainerClass = QVector<ItemClass> >
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
	void PushBack(const ItemClass& item);
	void PushFront(const ItemClass& item);
	void InsertAt(const ItemClass& item, int index);
	void PopBack();
	void PopFront();
	void RemoveAt(int index);
	virtual void Reset();

	// reimplemented (istd::IContainerInfo)
	virtual int GetItemsCount() const;
	virtual bool IsEmpty() const;
	virtual bool IsIndexValid(int index) const;

protected:
	typedef ContainerClass Items;
	Items m_items;
};


template <typename ItemClass, typename ContainerClass>
ItemClass& TContainer<ItemClass, ContainerClass>::GetAt(int index)
{
	I_ASSERT(IsIndexValid(index));

	return *(m_items.begin() + index);
}


template <typename ItemClass, typename ContainerClass>
const ItemClass& TContainer<ItemClass, ContainerClass>::GetAt(int index) const
{
	I_ASSERT(IsIndexValid(index));

	return *(m_items.begin() + index);
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PushBack(const ItemClass& item)
{
	istd::CChangeNotifier changePtr(this, CF_ELEMENT_ADDED);

	std::back_inserter<ContainerClass>(m_items) = item;
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PushFront(const ItemClass& item)
{
	istd::CChangeNotifier changePtr(this, CF_ELEMENT_ADDED);

	std::front_inserter<ContainerClass>(m_items) = item;
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::InsertAt(const ItemClass& item, int index)
{
	if (index < 0 || index >= int(m_items.size())){
		PushBack(item);
	}
	else{
		istd::CChangeNotifier changePtr(this, CF_ELEMENT_ADDED);

		std::inserter<ContainerClass>(m_items, m_items.begin() + index) = item;
	}
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PopBack()
{
	istd::CChangeNotifier changePtr(this, CF_ELEMENT_REMOVED);

	m_items.pop_back();
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PopFront()
{
	istd::CChangeNotifier changePtr(this, CF_ELEMENT_REMOVED);

	m_items.pop_front();
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::RemoveAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_items.size()));

	if (index < int(m_items.size())){
		istd::CChangeNotifier changePtr(this, CF_ELEMENT_REMOVED);
	
		m_items.erase(m_items.begin()  + index);
	}
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::Reset()
{
	istd::CChangeNotifier changePtr(this, CF_RESET);

	m_items.clear();
}


// reimplemented (istd::IContainerInfo)

template <typename ItemClass, typename ContainerClass>
int TContainer<ItemClass, ContainerClass>::GetItemsCount() const
{
	return int(m_items.size());
}


template <typename ItemClass, typename ContainerClass>
bool TContainer<ItemClass, ContainerClass>::IsEmpty() const
{
	return m_items.isEmpty();
}


template <typename ItemClass, typename ContainerClass>
bool TContainer<ItemClass, ContainerClass>::IsIndexValid(int index) const
{
	return (index >= 0 && index < int(m_items.size()));
}


} // namespace ibase


#endif // !ibase_TContainer_included


