#include "iipr/CSearchParams.h"


#include "istd/TChangeNotifier.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CSearchParams::CSearchParams()
	:m_angleRange(-180.0, 180.0),
	m_scaleRange(0.5, 2.0),
	m_nominalModelsCount(1),
	m_minScore(0.6)
{
}


// reimplemented (iipr::ISearchParams)

i2d::CRectangle CSearchParams::GetSearchRegion() const
{
	return m_searchRegion;
}


double CSearchParams::GetMinScore() const
{
	return m_minScore;
}


void CSearchParams::SetMinScore(double minScore)
{
	if (m_minScore != minScore){
		istd::CChangeNotifier changeNotifier(this);

		m_minScore = minScore;
	}
}


const istd::CRange& CSearchParams::GetRotationRange() const
{
	return m_angleRange;
}


void CSearchParams::SetRotationRange(const istd::CRange& angleRange)
{
	if (m_angleRange != angleRange){
		istd::CChangeNotifier changeNotifier(this);

		m_angleRange = angleRange;
	}
}


const istd::CRange& CSearchParams::GetScaleRange() const
{
	return m_scaleRange;
}


void CSearchParams::SetScaleRange(const istd::CRange& scaleRange)
{
	if (m_scaleRange != scaleRange){
		istd::CChangeNotifier changeNotifier(this);

		m_scaleRange = scaleRange;
	}
}


int CSearchParams::GetNominalModelsCount() const
{
	return m_nominalModelsCount;
}


void CSearchParams::SetNominalModelsCount(int nominalModelsCount)
{
	if (m_nominalModelsCount != nominalModelsCount){
		istd::CChangeNotifier changeNotifier(this);

		m_nominalModelsCount = nominalModelsCount;
	}
}


bool CSearchParams::IsRotationEnabled() const 
{
	return m_isRotationEnabled;
}


void CSearchParams::SetRotationEnabled(bool isRotationEnabled)
{
	if (m_isRotationEnabled != isRotationEnabled){
		istd::CChangeNotifier notifierPtr(this);

		m_isRotationEnabled = isRotationEnabled;
	}
}


bool CSearchParams::IsScaleEnabled() const
{
	return m_isScaleEnabled;
}


void CSearchParams::SetScaleEnabled(bool isScaleEnabled)
{
	if (m_isScaleEnabled != isScaleEnabled){
		istd::CChangeNotifier notifierPtr(this);

		m_isScaleEnabled = isScaleEnabled;
	}
}


// reimplemented (iser::ISerializable)

bool CSearchParams::Serialize(iser::IArchive & archive)
{
	double startAngle = m_angleRange.GetMinValue();
	double endAngle = m_angleRange.GetMaxValue();
	double minScale = m_scaleRange.GetMinValue();
	double maxScale = m_scaleRange.GetMaxValue();

	static iser::CArchiveTag startAngleTag("StartAngle", "Minimum angle");
	bool retVal = archive.BeginTag(startAngleTag);
	retVal = retVal && archive.Process(startAngle);
	retVal = retVal && archive.EndTag(startAngleTag);

	static iser::CArchiveTag endAngleTag("EndAngle", "Maximum angle");
	retVal = retVal && archive.BeginTag(endAngleTag);
	retVal = retVal && archive.Process(endAngle);
	retVal = retVal && archive.EndTag(endAngleTag);

	static iser::CArchiveTag minScoreTag("MinScore", "Minimum match score");
	retVal = retVal && archive.BeginTag(minScoreTag);
	retVal = retVal && archive.Process(m_minScore);
	retVal = retVal && archive.EndTag(minScoreTag);

	static iser::CArchiveTag minScaleTag("MinScale", "Minimum scale");
	retVal = retVal && archive.BeginTag(minScaleTag);
	retVal = retVal && archive.Process(minScale);
	retVal = retVal && archive.EndTag(minScaleTag);

	static iser::CArchiveTag maxScaleTag("MaxScale", "Maximum scale");
	retVal = retVal && archive.BeginTag(maxScaleTag);
	retVal = retVal && archive.Process(maxScale);
	retVal = retVal && archive.EndTag(maxScaleTag);

	static iser::CArchiveTag isRotationEnabledTag("RotationEnabled", "Is angle range enabled");
	retVal = retVal && archive.BeginTag(isRotationEnabledTag);
	retVal = retVal && archive.Process(m_isRotationEnabled);
	retVal = retVal && archive.EndTag(isRotationEnabledTag);

	static iser::CArchiveTag isScaleEnabledTag("ScaleEnabledTag", "Is scale range enabled");
	retVal = retVal && archive.BeginTag(isScaleEnabledTag);
	retVal = retVal && archive.Process(m_isScaleEnabled);
	retVal = retVal && archive.EndTag(isScaleEnabledTag);

	static iser::CArchiveTag nominalModelsCountTag("NominalModelsCount", "Minimum model matches count");
	retVal = retVal && archive.BeginTag(nominalModelsCountTag);
	retVal = retVal && archive.Process(m_nominalModelsCount);
	retVal = retVal && archive.EndTag(nominalModelsCountTag);

	static iser::CArchiveTag searchRegionTag("SearchRegion", "Region for the model search");
	retVal = retVal && archive.BeginTag(searchRegionTag);
	retVal = retVal && m_searchRegion.Serialize(archive);
	retVal = retVal && archive.EndTag(searchRegionTag);

	if (!archive.IsStoring()){
		m_angleRange = istd::CRange(startAngle, endAngle);
		m_scaleRange = istd::CRange(minScale, maxScale);
	}

	return retVal;
}


} // namespace iipr

