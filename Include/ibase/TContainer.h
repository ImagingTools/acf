#ifndef ibase_TContainer_included
#define ibase_TContainer_included


#include <vector>


#include "istd/IContainerInfo.h"
#include "istd/IChangeable.h"

#include "istd/TChangeNotifier.h"


namespace ibase
{


/**
	Common implementation of an abstract container. 
*/
template <typename ItemClass, typename ContainerClass = std::vector<ItemClass> >
class TContainer: virtual public istd::IContainerInfo, virtual public istd::IChangeable
{
public:
	TContainer();
	virtual ~TContainer();

	const ItemClass& GetAt(int index) const;
	ItemClass& GetAt(int index);
	void PushBack(const ItemClass& item);
	void PushFront(const ItemClass& item);
	void PopBack();
	void PopFront();
	void RemoveAt(int index);
	virtual void Reset();

	// reimplemented (istd::IContainerInfo)
	virtual int GetItemCount() const;
	virtual bool IsEmpty() const;
	virtual bool IsIndexValid(int index) const;

protected:
	typedef ContainerClass Items;
	Items m_items;
};


template <typename ItemClass, typename ContainerClass>
TContainer<ItemClass, ContainerClass>::TContainer()
{

}


template <typename ItemClass, typename ContainerClass>
TContainer<ItemClass, ContainerClass>::~TContainer()
{

}


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
	istd::TChangeNotifier<istd::IContainerInfo> changePtr(this);

	std::back_inserter<ContainerClass>(m_items) = item;
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PushFront(const ItemClass& item)
{
	istd::TChangeNotifier<istd::IContainerInfo> changePtr(this);

	std::front_inserter<ContainerClass>(m_items) = item;
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PopBack()
{
	istd::TChangeNotifier<istd::IContainerInfo> changePtr(this);

	m_items.pop_back();
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::PopFront()
{
	istd::TChangeNotifier<istd::IContainerInfo> changePtr(this);

	m_items.pop_front();
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::RemoveAt(int index)
{
	I_ASSERT(index >= 0);
	I_ASSERT(index < int(m_items.size()));

	if (index < int(m_items.size())){
		istd::TChangeNotifier<istd::IContainerInfo> changePtr(this);
	
		m_items.erase(m_items.begin()  + index);
	}
}


template <typename ItemClass, typename ContainerClass>
void TContainer<ItemClass, ContainerClass>::Reset()
{
	istd::TChangeNotifier<istd::IContainerInfo> changePtr(this);

	m_items.clear();
}


// reimplemented (istd::IContainerInfo)

template <typename ItemClass, typename ContainerClass>
int TContainer<ItemClass, ContainerClass>::GetItemCount() const
{
	return int(m_items.size());
}


template <typename ItemClass, typename ContainerClass>
bool TContainer<ItemClass, ContainerClass>::IsEmpty() const
{
	return m_items.empty();
}


template <typename ItemClass, typename ContainerClass>
bool TContainer<ItemClass, ContainerClass>::IsIndexValid(int index) const
{
	return (index >= 0 && index < int(m_items.size()));
}


} // namespace ibase


#endif // ibase_TContainer_included


