#ifndef ibase_TFactorisableContainer_included
#define ibase_TFactorisableContainer_included


#include <memory>


#include "istd/TSmartPtr.h"
#include "istd/TIFactory.h"

#include "ibase/TSerializableContainer.h"


namespace ibase
{

	
/**
	Specific container implementation for factorisable items. 

	This container implementation supports folowing features:
	1. Automatic memory management for created items
	2. Serialization mechanism with automatic item allocation.
*/
template <class InterfaceClass>
class TFactorisableContainer: public ibase::TSerializableContainer<std::pair<istd::TSmartPtr<InterfaceClass>, std::string> >
{
public:
	typedef std::pair<istd::TSmartPtr<InterfaceClass>, std::string> ItemClass;
	typedef ibase::TSerializableContainer<ItemClass> BaseClass;

	TFactorisableContainer();
	virtual ~TFactorisableContainer();

	/**
		Adds the element to container. 
	*/
	InterfaceClass* AddElement(const std::string& elementFactoryKey);

	/**
		Gets the element with the index \c elementIndex from container. 
		You will need this function, because the GetAt() returns the ItemClass instance, what is not usable.
	*/
	InterfaceClass* GetElement(int elementIndex) const;

	/**
		Gets the element index from container. 
	*/
	int GetElementIndex(const InterfaceClass& elementRef) const;

	/**
		Gets the element key associated with the element the index \c elementIndex from container. 
	*/
	std::string GetElementKey(int elementIndex) const;

	void RegisterItemFactory(istd::TIFactory<InterfaceClass>* itemFactoryPtr);

	// reimplemented (iser::ISerializable)
	bool Serialize(iser::IArchive& archive);

protected:
	virtual InterfaceClass* CreateElement(const std::string& itemKey);
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
InterfaceClass* TFactorisableContainer<InterfaceClass>::AddElement(const std::string& elementFactoryKey)
{
	InterfaceClass* elementPtr = CreateElement(elementFactoryKey);
	if (elementPtr != NULL){
		BaseClass::PushBack(std::make_pair(elementPtr, elementFactoryKey));
	}

	return elementPtr;
}



template <class InterfaceClass>
InterfaceClass* TFactorisableContainer<InterfaceClass>::GetElement(int elementIndex) const
{
	if (elementIndex < BaseClass::GetItemCount() && elementIndex >= 0){
		return const_cast<InterfaceClass*>(BaseClass::GetAt(elementIndex).first.GetPtr());
	}

	return NULL;
}


template <class InterfaceClass>
int TFactorisableContainer<InterfaceClass>::GetElementIndex(const InterfaceClass& elementRef) const
{
	for (int itemIndex = 0; itemIndex < GetItemCount(); itemIndex++){
		InterfaceClass* elementPtr = GetElement(itemIndex);
		if (elementPtr == &elementRef){
			return itemIndex;
		}
	}

	return -1;
}


template <class InterfaceClass>
std::string TFactorisableContainer<InterfaceClass>::GetElementKey(int elementIndex) const
{
	if (elementIndex < BaseClass::GetItemCount() &&  elementIndex >= 0){
		return BaseClass::GetAt(elementIndex).second;
	}

	return std::string();
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
	if (!archive.IsStoring()){
		this->Reset();
	}

	int itemCount = BaseClass::GetItemCount();

	static iser::CArchiveTag itemsTag("Items", "List of items");
	static iser::CArchiveTag itemTag("Item", "Item");

	bool retVal = archive.BeginMultiTag(itemsTag, itemTag, itemCount);
	if (!retVal){
		return false;
	}

	for (int index = 0; index < itemCount; index++){
		ItemClass item;
		std::string itemKey;

		if (archive.IsStoring()){
			itemKey = GetAt(index).second;
		}

		static iser::CArchiveTag keyTag("ItemKey", "Factory key of the item");

		retVal = retVal && archive.BeginTag(keyTag);
		retVal = retVal && archive.Process(itemKey);
		retVal = retVal && archive.EndTag(keyTag);

		if (!archive.IsStoring()){	
			item.second = itemKey;
			InterfaceClass* interfacePtr = CreateElement(itemKey);
			if (interfacePtr != NULL){
				item.first = istd::TSmartPtr<InterfaceClass>(interfacePtr);

				BaseClass::PushBack(item);
			}
			else{
				return false;
			}
		}
				
		ItemClass& containerItem = GetAt(index);

		retVal = retVal && archive.BeginTag(itemTag);
		retVal = retVal && SerializeItem(containerItem, archive);
		retVal = retVal && archive.EndTag(itemTag);
	}

	retVal = retVal && archive.EndTag(itemsTag);

	return retVal;
}


// protected methods

template <class InterfaceClass>
InterfaceClass* TFactorisableContainer<InterfaceClass>::CreateElement(const std::string& itemKey)
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
void TFactorisableContainer<InterfaceClass>::OnElementCreated(InterfaceClass* elementPtr)
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

