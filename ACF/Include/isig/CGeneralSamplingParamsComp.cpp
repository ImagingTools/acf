#include "isig/CGeneralSamplingParamsComp.h"


#include "istd/TChangeNotifier.h"


namespace isig
{


CGeneralSamplingParamsComp::CGeneralSamplingParamsComp()
:	m_interval(0), m_samplingMode(SM_SINGLE)
{
}


// reimplemented (icomp::IComponent)

void CGeneralSamplingParamsComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	for (int i = 0; i <= SM_LAST; ++i){
		if (IsSamplingModeSupported(i)){
			m_samplingMode = i;
			break;
		}
	}

	m_interval = sqrt(*m_minIntervalAttrPtr * *m_maxIntervalAttrPtr);
}


// reimplemented (isig::ISamplingParams)

istd::CRange CGeneralSamplingParamsComp::GetIntervalRange() const
{
	return istd::CRange(*m_minIntervalAttrPtr, *m_maxIntervalAttrPtr);
}


double CGeneralSamplingParamsComp::GetInterval() const
{
	return m_interval;
}


void CGeneralSamplingParamsComp::SetInterval(double value)
{
	istd::CRange range = GetIntervalRange();

	double interval = range.GetNearestInRange(value);

	if (interval != m_interval){
		istd::CChangeNotifier notifier(this);

		m_interval = interval;
	}
}


bool CGeneralSamplingParamsComp::IsSamplingModeSupported(int mode) const
{
	switch (mode){
	case SM_SINGLE:
		return *m_isSingleModeSupportedAttrPtr;

	case SM_PERIODIC:
		return *m_isPeriodicModeSupportedAttrPtr;

	case SM_SYNCHRONIZED:
		return *m_isSynchronizedModeSupportedAttrPtr;

	default:
		return false;
	}
}


int CGeneralSamplingParamsComp::GetSamplingMode() const
{
	return m_samplingMode;
}


bool CGeneralSamplingParamsComp::SetSamplingMode(int mode)
{
	if (IsSamplingModeSupported(mode)){
		if (mode != m_samplingMode){
			istd::CChangeNotifier notifier(this);

			m_samplingMode = mode;
		}

		return true;
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CGeneralSamplingParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	double interval = m_interval;
	static iser::CArchiveTag intervalTag("Interval", "Interval [s] of simple sample");
	retVal = retVal && archive.BeginTag(intervalTag);
	retVal = retVal && archive.Process(interval);
	retVal = retVal && archive.EndTag(intervalTag);

	int samplingMode = m_samplingMode;
	static iser::CArchiveTag samplingModeTag("SamplingMode", "Sampling mode (isig::ISamplingParams::SamplingMode)");
	retVal = retVal && archive.BeginTag(samplingModeTag);
	retVal = retVal && archive.Process(samplingMode);
	retVal = retVal && archive.EndTag(samplingModeTag);

	if (!archive.IsStoring()){
		istd::CChangeNotifier notifier(this);

		SetInterval(interval);
		SetSamplingMode(samplingMode);
	}

	return retVal;
}


} // namespace isig


