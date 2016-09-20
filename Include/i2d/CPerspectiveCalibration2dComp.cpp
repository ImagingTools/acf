#include <i2d/CPerspectiveCalibration2dComp.h>


// ACF incldues
#include <istd/CChangeGroup.h>


namespace i2d
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CPerspectiveCalibration2dComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	SetArgumentArea(m_argumentAreaCompPtr.GetPtr());
	SetResultArea(m_resultAreaCompPtr.GetPtr());
	SetArgumentUnitInfo(m_argumentUnitInfoCompPtr.GetPtr());
	SetResultUnitInfo(m_resultUnitInfoCompPtr.GetPtr());
}


void CPerspectiveCalibration2dComp::OnComponentDestroyed()
{
	BaseClass::OnComponentDestroyed();

	istd::CChangeGroup changeGroup(this);
	Q_UNUSED(changeGroup);

	SetArgumentArea(NULL);
	SetResultArea(NULL);
	SetArgumentUnitInfo(NULL);
	SetResultUnitInfo(NULL);
}


} // namespace i2d


