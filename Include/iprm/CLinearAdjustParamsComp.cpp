#include "iprm/CLinearAdjustParamsComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace iprm
{


CLinearAdjustParamsComp::CLinearAdjustParamsComp()
:	m_scale(1.0),
	m_offset(0)
{
}


// reimplemented (iprm::ILinearAdjustParams)

const ILinearAdjustConstraints* CLinearAdjustParamsComp::GetAdjustConstraints() const
{
	return m_constraintsCompPtr.GetPtr();
}


double CLinearAdjustParamsComp::GetScaleFactor() const
{
	return m_scale;
}


void CLinearAdjustParamsComp::SetScaleFactor(double scale)
{
	if (scale != m_scale){
		istd::CChangeNotifier notifier(this);

		m_scale = scale;
	}
}


double CLinearAdjustParamsComp::GetOffsetFactor() const
{
	return m_offset;
}


void CLinearAdjustParamsComp::SetOffsetFactor(double offset)
{
	if (offset != m_offset){
		istd::CChangeNotifier notifier(this);

		m_offset = offset;
	}
}


// reimplemented (iser::ISerializable)

bool CLinearAdjustParamsComp::Serialize(iser::IArchive& archive)
{
	bool retVal = true;

	static iser::CArchiveTag scaleTag("ScaleFactor", "Scale factor of linear adjust");
	retVal = retVal && archive.BeginTag(scaleTag);
	retVal = retVal && archive.Process(m_scale);
	retVal = retVal && archive.EndTag(scaleTag);

	static iser::CArchiveTag offsetTag("OffsetFactor", "Offset factor of linear adjust");
	retVal = retVal && archive.BeginTag(offsetTag);
	retVal = retVal && archive.Process(m_offset);
	retVal = retVal && archive.EndTag(offsetTag);

	return retVal;
}


} // namespace iprm


