#include "iser/TCopySerializedWrap.h"


// ACF includes
#include "iser/ISerializable.h"
#include "iser/CMemoryReadArchive.h"


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




} // namespace iser


