#include "iipr/CCaliperParams.h"


#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


#include "istd/TChangeNotifier.h"


namespace iipr
{


CCaliperParams::CCaliperParams()
:	m_weightThreshold(0.1),
	m_polarityMode(PM_ALL),
	m_directionMode(DM_FORWARD)
{
}


// reimplemented (iipr::ICaliperParams)

double CCaliperParams::GetWeightThreshold() const
{
	return m_weightThreshold;
}


void CCaliperParams::SetWeightThreshold(double threshold)
{
	if (threshold != m_weightThreshold){
		istd::CChangeNotifier notifier(this);

		m_weightThreshold = threshold;
	}
}


int CCaliperParams::GetPolarityMode() const
{
	return m_polarityMode;
}


void CCaliperParams::SetPolarityMode(int mode)
{
	if (mode != m_polarityMode){
		istd::CChangeNotifier notifier(this);

		m_polarityMode = mode;
	}
}


int CCaliperParams::GetDirectionMode() const
{
	return m_directionMode;
}


void CCaliperParams::SetDirectionMode(int mode)
{
	if (mode != m_directionMode){
		istd::CChangeNotifier notifier(this);

		m_directionMode = mode;
	}
}


// reimplemented (iser::ISerializable)

bool CCaliperParams::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag weightThresholdTag("WeightThreshold", "Threshold describing of minimal weight acceptace");
	retVal = retVal && archive.BeginTag(weightThresholdTag);
	retVal = retVal && archive.Process(m_weightThreshold);
	retVal = retVal && archive.EndTag(weightThresholdTag);

	static iser::CArchiveTag polarityModeTag("Polarity mode", "Polarity mode code");
	retVal = retVal && archive.BeginTag(polarityModeTag);
	retVal = retVal && archive.Process(m_polarityMode);
	retVal = retVal && archive.EndTag(polarityModeTag);

	static iser::CArchiveTag directionModeTag("Direction mode", "Direction mode code");
	retVal = retVal && archive.BeginTag(directionModeTag);
	retVal = retVal && archive.Process(m_directionMode);
	retVal = retVal && archive.EndTag(directionModeTag);

	return retVal;
}


} // namespace iipr


