#include "iipr/CProjectionData.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CProjectionData::CProjectionData(iimg::IBitmap* dataImagePtr, bool releaseFlag)
:	m_projectionImagePtr(dataImagePtr, releaseFlag),
	m_proportionRangeX(0, 1),
	m_proportionRangeY(0, 1)
{
	I_ASSERT(dataImagePtr != NULL);
}


void CProjectionData::SetProportionRangeX(const istd::CRange& range)
{
	if (range != m_proportionRangeX){
		istd::CChangeNotifier notifier(this);

		m_proportionRangeX = range;
	}
}


void CProjectionData::SetProportionRangeY(const istd::CRange& range)
{
	if (range != m_proportionRangeY){
		istd::CChangeNotifier notifier(this);

		m_proportionRangeY = range;
	}
}


// reimplemented (iser::ISerializable)

bool CProjectionData::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	double minX = m_proportionRangeX.GetMinValue();
	double maxX = m_proportionRangeX.GetMaxValue();
	static iser::CArchiveTag proportionRangeXTag("PropRangeX", "Proportion range for X axis");
	retVal = retVal && archive.BeginTag(proportionRangeXTag);
	retVal = retVal && archive.Process(minX);
	retVal = retVal && archive.Process(maxX);
	retVal = retVal && archive.EndTag(proportionRangeXTag);
	m_proportionRangeX.SetMinValue(minX);
	m_proportionRangeX.SetMaxValue(maxX);

	double minY = m_proportionRangeY.GetMinValue();
	double maxY = m_proportionRangeY.GetMaxValue();
	static iser::CArchiveTag proportionRangeYTag("PropRangeY", "Proportion range for Y axis");
	retVal = retVal && archive.BeginTag(proportionRangeYTag);
	retVal = retVal && archive.Process(minY);
	retVal = retVal && archive.Process(maxY);
	retVal = retVal && archive.EndTag(proportionRangeYTag);
	m_proportionRangeY.SetMinValue(minY);
	m_proportionRangeY.SetMaxValue(maxY);

	static iser::CArchiveTag projectionImageTag("ProjectionImage", "Projection Image Data");
	retVal = retVal && archive.BeginTag(projectionImageTag);
	retVal = retVal && m_projectionImagePtr->Serialize(archive);
	retVal = retVal && archive.EndTag(projectionImageTag);

	return retVal;
}


} // namespace iipr

