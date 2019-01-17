#ifndef ibase_TSerializableContainer_included
#define ibase_TSerializableContainer_included


// ACF includes
#include <iser/IArchive.h>
#include <iser/ISerializable.h>
#include <iser/CArchiveTag.h>
#include <ibase/TContainer.h>


namespace ibase
{


/**
	Common implementation for an abstract serializable container.
	Derived class must only reimplement the SerializeItem().
*/
template <typename ItemClass, class ContainerType = typename TContainer<ItemClass>::Container>
class TSerializableContainer: public TContainer<ItemClass, ContainerType>, virtual public iser::ISerializable
{
public:
	typedef TContainer<ItemClass, ContainerType> BaseClass;

	// reimplemented (iser::Serializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// abstract methods

	/**
		Serialize a single item in the container.
	*/
	virtual bool SerializeItem(ItemClass& item, iser::IArchive& archive, iser::CArchiveTag* parentTagPtr = NULL) = 0;
};


// public methods

// reimplemented (iser::Serializable)

template <typename ItemClass, class ContainerType>
bool TSerializableContainer<ItemClass, ContainerType>::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag itemsTag("Items", "List of items", iser::CArchiveTag::TT_MULTIPLE);
	static iser::CArchiveTag itemTag("Item", "Item", iser::CArchiveTag::TT_GROUP, &itemsTag);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);
	Q_UNUSED(notifier);

	int itemCount = int(BaseClass::m_items.size());

	bool retVal = archive.BeginMultiTag(itemsTag, itemTag, itemCount);
	if (!retVal){
		return false;
	}

	if (!archive.IsStoring()){
		BaseClass::Reset();

#if QT_VERSION >= 0x040700
		BaseClass::m_items.reserve(itemCount);
#endif
		for (int index = 0; index < itemCount; index++){
			BaseClass::PushBack(ItemClass());

			ItemClass& item = BaseClass::GetAt(index);

			retVal = retVal && archive.BeginTag(itemTag);

			retVal = retVal && SerializeItem(item, archive);

			retVal = retVal && archive.EndTag(itemTag);
		}
	}
	else{
		for (int index = 0; index < itemCount; index++){
			ItemClass& item = BaseClass::GetAt(index);

			retVal = retVal && archive.BeginTag(itemTag);

			retVal = retVal && SerializeItem(item, archive);

			retVal = retVal && archive.EndTag(itemTag);
		}
	}

	return retVal;
}


} // namespace ibase


#endif // !ibase_TSerializableContainer_included


