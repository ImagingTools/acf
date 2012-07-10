#ifndef ibase_TSerializableContainer_included
#define ibase_TSerializableContainer_included


#include "iser/IArchive.h"
#include "iser/ISerializable.h"
#include "iser/CArchiveTag.h"

#include "ibase/TContainer.h"


namespace ibase
{


/**
	Common implementation for an abstract serializable container. 
	Derived class must only reimplement the SerializeItem(). 
*/
template <typename ItemClass>
class TSerializableContainer: public TContainer<ItemClass>, virtual public iser::ISerializable
{
public:
	typedef TContainer<ItemClass> BaseClass;

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

template <typename ItemClass>
bool TSerializableContainer<ItemClass>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag itemsTag("Items", "List of items");
	static iser::CArchiveTag itemTag("Item", "Item");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	int itemCount = int(BaseClass::m_items.size());

	bool retVal = archive.BeginMultiTag(itemsTag, itemTag, itemCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		BaseClass::Reset();
		BaseClass::m_items.reserve(itemCount);

		for (int index = 0; index < itemCount; index++){
			BaseClass::PushBack(ItemClass());

			ItemClass& item = GetAt(index);

			bool retVal = archive.BeginTag(itemTag);

			retVal = retVal && SerializeItem(item, archive);

			retVal = retVal && archive.EndTag(itemTag);
		}
	}
	else{
		for (int index = 0; index < itemCount; index++){
			ItemClass& item = GetAt(index);

			bool retVal = archive.BeginTag(itemTag);

			retVal = retVal && SerializeItem(item, archive);

			retVal = retVal && archive.EndTag(itemTag);
		}
	}

	return retVal;
}


} // namespace ibase


#endif // !ibase_TSerializableContainer_included


