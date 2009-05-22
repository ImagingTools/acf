#ifndef idoc_CSerializedStateComparator_included
#define idoc_CSerializedStateComparator_included


#include "iser/CMemoryWriteArchive.h"

#include "idoc/IDocumentStateComparator.h"


namespace idoc
{


/**
	Implementation of document state comparator based on binary serialization archive.
	Please note, that compared documents must implement interface \c iser::ISerializable.
*/
class CSerializedStateComparator: virtual public IDocumentStateComparator
{
public:
	// reimplemented (idoc::IDocumentStateComparator)
	virtual void StoreState(const istd::IChangeable& document);
	virtual bool CheckStateEquals(const istd::IChangeable& document) const;

private:
	iser::CMemoryWriteArchive m_storedArchive;
};


} // namespace idoc


#endif // !idoc_CSerializedStateComparator_included


