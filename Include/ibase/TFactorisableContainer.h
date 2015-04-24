#ifndef ibase_TFactorisableContainer_included
#define ibase_TFactorisableContainer_included


// Qt includes
#include <QtCore/QPair>

// ACF includes
#include "istd/TSmartPtr.h"
#include "istd/TIFactory.h"
#include "ibase/TSerializableContainer.h"


namespace ibase
{


/**
	Specific container implementation for factorisable items. 

	This container implementation supports following features:
	\li Automatic memory management for created items
	\li Serialization mechanism with automatic item allocation.
*/
template <class InterfaceClass>
class TFactorisableContainer: 
			public ibase::TSerializableContainer<
						QPair<istd::TSmartPtr<InterfaceClass>, QByteArray> >
{
public:
	typedef QPair<istd::TSmartPtr<InterfaceClass>, QByteArray> ItemClass;
	typedef ibase::TSerializableContainer<ItemClass> BaseClass;

	TFactorisableContainer();
	virtual ~TFactorisableContainer();

	/**
		Insert an element into the container at given index. 
	*/
	InterfaceClass* InsertElement(int index, const QByteArray& elementFactoryKey);

	/**
		Add an element to the container.
	*/
	InterfaceClass* AddElement(const QByteArray& elementFactoryKey);

	/**
		Gets an element with given index \c elementIndex from the container.
		You will need this function, because the GetAt() returns an ItemClass 
		instance, which is not usable.
	*/
	InterfaceClass* GetElement(int elementIndex) const;

	/**
		Gets the index of given element from the container. 
	*/
	int GetElementIndex(const InterfaceClass& elementRef) const;

	/**
		Gets the element key associated with the element with given \c elementIndex from the container.
	*/
	QByteArray GetElementKey(int elementIndex) const;

	void RegisterItemFactory(istd::TIFactory<InterfaceClass>* itemFactoryPtr);

	// reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);

protected:
	virtual InterfaceClass* CreateElement(const QByteArray& itemKey);
	virtual void OnElementCreated(InterfaceClass* elementPtr);

	// reimplemented (ibase::TContainer)
	virtual bool SerializeItem(ItemClass& item, iser::IArchive& archive);

protected:
	istd::TIFactory<InterfaceClass>* m_itemFactoryPtr;
};


template <class InterfaceClass>
TFactorisableContainer<InterfaceClass>::TFactorisableContainer()
{
	m_itemFactoryPtr = NULL;
}


template <class InterfaceClass>
TFactorisableContainer<InterfaceClass>::~TFactorisableContainer()
{
	BaseClass::Reset();
}


template <class InterfaceClass>
InterfaceClass* TFactorisableContainer<InterfaceClass>::AddElement(const QByteArray& elementFactoryKey)
{
	istd::TSmartPtr<InterfaceClass> elementPtr(CreateElement(elementFactoryKey));
	if (elementPtr.IsValid()){
		BaseClass::PushBack(ItemClass(elementPtr, elementFactoryKey));
	}

	return elementPtr.GetPtr();
}


template <class InterfaceClass>
InterfaceClass* TFactorisableContainer<InterfaceClass>::InsertElement(int index, const QByteArray& elementFactoryKey)
{
	istd::TSmartPtr<InterfaceClass> elementPtr(CreateElement(elementFactoryKey));
	if (elementPtr.IsValid()){
		BaseClass::InsertAt(ItemClass(elementPtr, elementFactoryKey), index);
	}

	return elementPtr.GetPtr();
}


template <class InterfaceClass>
InterfaceClass* TFactorisableContainer<InterfaceClass>::GetElement(int elementIndex) const
{
	if (elementIndex < BaseClass::GetItemsCount() && elementIndex >= 0){
		return const_cast<InterfaceClass*>(BaseClass::GetAt(elementIndex).first.GetPtr());
	}

	return NULL;
}


template <class InterfaceClass>
int TFactorisableContainer<InterfaceClass>::GetElementIndex(const InterfaceClass& elementRef) const
{
	for (int itemIndex = 0; itemIndex < BaseClass::GetItemsCount(); itemIndex++){
		InterfaceClass* elementPtr = GetElement(itemIndex);
		if (elementPtr == &elementRef){
			return itemIndex;
		}
	}

	return -1;
}


template <class InterfaceClass>
QByteArray TFactorisableContainer<InterfaceClass>::GetElementKey(int elementIndex) const
{
	if (elementIndex < BaseClass::GetItemsCount() &&  elementIndex >= 0){
		return BaseClass::GetAt(elementIndex).second;
	}

	return QByteArray();
}


template <class InterfaceClass>
void TFactorisableContainer<InterfaceClass>::RegisterItemFactory(istd::TIFactory<InterfaceClass>* itemFactoryPtr)
{
	m_itemFactoryPtr = itemFactoryPtr;
}


// reimplemented (iser::ISerializable)

template <class InterfaceClass>
bool TFactorisableContainer<InterfaceClass>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag itemsTag("Items", "List of items", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag itemTag("Item", "Item", iser::CArchiveTag::TT_GROUP, &itemsTag);
	static iser::CArchiveTag keyTag("ItemKey", "Factory key of the item", iser::CArchiveTag::TT_LEAF, &itemTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	if (!archive.IsStoring()){
		this->Reset();
	}

	int itemCount = BaseClass::GetItemsCount();

	bool retVal = archive.BeginMultiTag(itemsTag, itemTag, itemCount);
	if (!retVal){
		return false;
	}

	for (int index = 0; index < itemCount; index++){
		retVal = retVal && archive.BeginTag(itemTag);

		ItemClass item;
		QByteArray itemKey;

		if (archive.IsStoring()){
			itemKey = BaseClass::GetAt(index).second;
		}

		retVal = retVal && archive.BeginTag(keyTag);
		retVal = retVal && archive.Process(itemKey);
		retVal = retVal && archive.EndTag(keyTag);

		if (!archive.IsStoring()){	
			item.second = itemKey;
			InterfaceClass* interfacePtr = CreateElement(itemKey);
			if (interfacePtr != NULL){
				item.first.SetPtr(interfacePtr);

				BaseClass::PushBack(item);
			}
			else{
				return false;
			}
		}

		ItemClass& containerItem = BaseClass::GetAt(index);

		retVal = retVal && SerializeItem(containerItem, archive);

		retVal = retVal && archive.EndTag(itemTag);
	}

	retVal = retVal && archive.EndTag(itemsTag);

	return retVal;
}


// protected methods

template <class InterfaceClass>
InterfaceClass* TFactorisableContainer<InterfaceClass>::CreateElement(const QByteArray& itemKey)
{
	if (m_itemFactoryPtr != NULL){
		istd::IPolymorphic* polymorphicPtr = m_itemFactoryPtr->CreateInstance(itemKey);
		if (polymorphicPtr != NULL){
			InterfaceClass* interfacePtr = dynamic_cast<InterfaceClass*>(polymorphicPtr);
			if (interfacePtr != NULL){
				OnElementCreated(interfacePtr);

				return interfacePtr;
			}
			else{
				delete polymorphicPtr;
				
				return NULL;
			}
		}
	}
	
	return NULL;
}


template <class InterfaceClass>
void TFactorisableContainer<InterfaceClass>::OnElementCreated(InterfaceClass* /*elementPtr*/)
{
}


// reimplemented (ibase::TContainer)

template <class InterfaceClass>
bool TFactorisableContainer<InterfaceClass>::SerializeItem(ItemClass& item, iser::IArchive& archive)
{
	iser::ISerializable* serializablePtr = dynamic_cast<iser::ISerializable*>(item.first.GetPtr());
	if (serializablePtr != NULL){
		return serializablePtr->Serialize(archive);
	}

	return false;
}


} // namespace ibase


#endif // !ibase_TFactorisableContainer_included

