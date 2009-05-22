#include "idoc/CSerializedStateComparator.h"


#include "iser/ISerializable.h"


namespace idoc
{


void CSerializedStateComparator::StoreState(const istd::IChangeable& document)
{
	m_storedArchive.Reset();

	const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&document);
	if (serializablePtr != NULL){
		const_cast<iser::ISerializable*>(serializablePtr)->Serialize(m_storedArchive);
	}
}


bool CSerializedStateComparator::CheckStateEquals(const istd::IChangeable& document) const
{
	iser::CMemoryWriteArchive compareArchive;

	const iser::ISerializable* serializablePtr = dynamic_cast<const iser::ISerializable*>(&document);
	if (serializablePtr != NULL){
		const_cast<iser::ISerializable*>(serializablePtr)->Serialize(compareArchive);
	}

	return compareArchive == m_storedArchive;
}


} // namespace idoc


