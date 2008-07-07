#include "iipr/CRectangularFilterParams.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CRectangularFilterParams::CRectangularFilterParams()
:	m_iterationsCount(1),
	m_filterSize(3, 3)
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


int CRectangularFilterParams::GetIterationsCount() const
{
	return m_iterationsCount;
}


void CRectangularFilterParams::SetIterationsCount(int iterationsCount)
{
	I_ASSERT(m_iterationsCount > 0);

	m_iterationsCount = iterationsCount;
}


// reimplemented (iser::ISerializable)

bool CRectangularFilterParams::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag sizeTag("Size", "Size");
	bool retVal = archive.BeginTag(sizeTag);
	retVal = retVal && m_filterSize.Serialize(archive);
	retVal = retVal && archive.EndTag(sizeTag);

	static iser::CArchiveTag iterationsCountTag("Iterations", "Number of filter iterations");
	retVal = retVal && archive.BeginTag(iterationsCountTag);
	retVal = retVal && archive.Process(m_iterationsCount);
	retVal = retVal && archive.EndTag(iterationsCountTag);

	return retVal;
}


} // namespace iipr