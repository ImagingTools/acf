#ifndef ibase_TSerializableContainer_included
#define ibase_TSerializableContainer_included


#include "iser/IArchive.h"
#include "iser/ISerializable.h"
#include "iser/CArchiveTag.h"

#include "ibase/TContainer.h"


namespace ibase
{


/**
* Common implementation for an abstract serializable container. 
* Derived class must only reimplement the SerializeItem(). 
*/
template <typename ItemClass, typename ContainerClass = std::vector<ItemClass> >
class TSerializableContainer: public TContainer<ItemClass, ContainerClass>, virtual public iser::ISerializable
{
public:
	typedef TContainer<ItemClass, ContainerClass> BaseClass;

	// reimplemented (iser::Serializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// abstract methods

	/**
		Serialize a single item in the container.
	*/
	virtual bool SerializeItem(ItemClass& item, iser::IArchive& archive) = 0;
};


// protected members	

// reimplemented (iser::Serializable)

template <typename ItemClass, typename ContainerClass>
bool TSerializableContainer<ItemClass, ContainerClass>::Serialize(iser::IArchive& archive)
{
	if (!archive.IsStoring()){
		this->Reset();
	}

	int itemCount = int(BaseClass::m_items.size());

	static iser::CArchiveTag itemsTag("Items", "List of items");
	static iser::CArchiveTag itemTag("Item", "Item");

	bool retVal = archive.BeginMultiTag(itemsTag, itemTag, itemCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		BaseClass::m_items.resize(itemCount);
	}

	for (int index = 0; index < itemCount; index++){
		ItemClass& item = BaseClass::m_items.at(index);

		bool retVal = archive.BeginTag(itemTag);

		retVal = retVal && SerializeItem(item, archive);

		retVal = retVal && archive.EndTag(itemTag);
	}

	return retVal;
}


} // namespace ibase


#endif // !ibase_TSerializableContainer_included
