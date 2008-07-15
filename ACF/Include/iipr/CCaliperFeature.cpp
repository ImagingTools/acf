#include "iipr/CCaliperFeature.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iipr
{


CCaliperFeature::CCaliperFeature(double weight, double position)
:	m_weight(weight),
	m_position(1, position)
{
}


// reimplemented (iipr::IFeature)

double CCaliperFeature::GetWeight() const
{
	return m_weight;
}


imath::CVarVector CCaliperFeature::GetPosition() const
{
	return m_position;
}


istd::CString CCaliperFeature::GetDescription() const
{
	return "";
}


// reimplemented (iser::ISerializable)

bool CCaliperFeature::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag weightTag("Weight", "Weight of feature");
	retVal = retVal && archive.BeginTag(weightTag);
	retVal = retVal && archive.Process(m_weight);
	retVal = retVal && archive.EndTag(weightTag);

	static iser::CArchiveTag positionTag("Position", "Position of caliper feature");
	retVal = retVal && archive.BeginTag(positionTag);
	retVal = retVal && archive.Process(m_position[0]);
	retVal = retVal && archive.EndTag(positionTag);

	return retVal;
}


} // namespace iipr


