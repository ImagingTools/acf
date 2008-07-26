#include "iipr/CRectangularFilterParams.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CRectangularFilterParams::CRectangularFilterParams()
:	m_filterSize(3, 3)
{
}


const ibase::CSize& CRectangularFilterParams::GetSize() const
{
	return m_filterSize;
}


void CRectangularFilterParams::SetSize(const ibase::CSize& filterSize)
{
	int filterWidth = istd::GetNextOdd(filterSize.GetX());
	int filterHeight = istd::GetNextOdd(filterSize.GetY());

	m_filterSize = ibase::CSize(filterWidth, filterHeight);
}


// reimplemented (iser::ISerializable)

bool CRectangularFilterParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag sizeTag("Size", "Size");

	bool retVal = archive.BeginTag(sizeTag);
	retVal = retVal && m_filterSize.Serialize(archive);
	retVal = retVal && archive.EndTag(sizeTag);

	return retVal;
}


} // namespace iipr

