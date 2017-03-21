#include <iser/TCopySerializedWrap.h>


// ACF includes
#include <iser/ISerializable.h>
#include <iser/CMemoryReadArchive.h>
#include <iser/CMemoryWriteArchive.h>


namespace iser
{


bool CopyByArchive(const istd::IChangeable& object, istd::IChangeable& result)
{
	const ISerializable* sourcePtr = dynamic_cast<const ISerializable*>(&object);
	if (sourcePtr == NULL){
		return false;
	}

	ISerializable* resultPtr = dynamic_cast<ISerializable*>(&result);
	if (resultPtr == NULL){
		return false;
	}

	return CMemoryReadArchive::CloneObjectByArchive(*sourcePtr, *resultPtr);
}


bool CompareByArchive(const istd::IChangeable& object1, const istd::IChangeable& object2)
{
	const ISerializable* object1Ptr = dynamic_cast<const ISerializable*>(&object1);
	if (object1Ptr == NULL){
		return false;
	}

	const ISerializable* object2Ptr = dynamic_cast<const ISerializable*>(&object2);
	if (object2Ptr == NULL){
		return false;
	}

	bool retVal = true;
	CMemoryWriteArchive archive1;
	retVal = retVal && const_cast<ISerializable*>(object1Ptr)->Serialize(archive1);

	CMemoryWriteArchive archive2;
	retVal = retVal && const_cast<ISerializable*>(object2Ptr)->Serialize(archive2);

	return retVal && (archive1 == archive2);
}



} // namespace iser


