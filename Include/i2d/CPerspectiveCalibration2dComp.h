#ifndef i2d_CPerspectiveCalibration2dComp_included
#define i2d_CPerspectiveCalibration2dComp_included


// ACF includes
#include <icomp/CComponentBase.h>

#include <i2d/CPerspectiveCalibration2d.h>


namespace i2d
{


/**
	Attributable component for perspective 2D calibration.
*/
class CPerspectiveCalibration2dComp: public icomp::CComponentBase, public CPerspectiveCalibration2d
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CPerspectiveCalibration2d BaseClass2;

	I_BEGIN_COMPONENT(CPerspectiveCalibration2dComp);
		I_REGISTER_INTERFACE(ICalibration2d);
		I_REGISTER_INTERFACE(ITransformation2d);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(CPerspectiveCalibration2d);
		I_ASSIGN(m_argumentAreaCompPtr, "ArgumentArea", "Area where argument positions are guilty", false, "ArgumentArea");
		I_ASSIGN(m_resultAreaCompPtr, "ResultArea", "Area where result positions are guilty", false, "ResultArea");
		I_ASSIGN(m_argumentUnitInfoCompPtr, "ArgumentUnitInfo", "Unit information for calibration argument", false, "ArgumentUnitInfo");
		I_ASSIGN(m_resultUnitInfoCompPtr, "ResultUnitInfo", "Unit information for calibration argument", false, "ResultUnitInfo");
	I_END_COMPONENT;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(i2d::CRectangle, m_argumentAreaCompPtr);
	I_REF(i2d::CRectangle, m_resultAreaCompPtr);
	I_REF(imath::IUnitInfo, m_argumentUnitInfoCompPtr);
	I_REF(imath::IUnitInfo, m_resultUnitInfoCompPtr);
};


} // namespace i2d


#endif // !i2d_CPerspectiveCalibration2dComp_included


