#include "iipr/CCaliperParams.h"


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


} // namespace iipr


