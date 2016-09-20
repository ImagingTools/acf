#include <i2d/CAffineCalibration2d.h>


// ACF includes
#include <imod/TModelWrap.h>
#include <istd/CChangeNotifier.h>


namespace i2d
{


// static constants
static const istd::IChangeable::ChangeSet s_calibrationChangedChangeSet(ICalibration2d::CF_CALIBRATION_CHANGED, "Calibration changed");
static const istd::IChangeable::ChangeSet s_areaChangedChangeSet(ICalibration2d::CF_AREA_CHANGED, "Calibration area changed");
static const istd::IChangeable::ChangeSet s_unitsChangedChangeSet(ICalibration2d::CF_UNITS_CHANGED, "Calibration units changed");


// public methods

CAffineCalibration2d::CAffineCalibration2d()
:	m_argumentAreaPtr(NULL),
	m_resultAreaPtr(NULL),
	m_argumentUnitInfoPtr(NULL),
	m_resultUnitInfoPtr(NULL)
{
	m_transformation.Reset();
}


CAffineCalibration2d::CAffineCalibration2d(const i2d::CAffine2d& transformation)
:	BaseClass(transformation),
	m_argumentAreaPtr(NULL),
	m_resultAreaPtr(NULL),
	m_argumentUnitInfoPtr(NULL),
	m_resultUnitInfoPtr(NULL)
{
}


void CAffineCalibration2d::SetArgumentArea(const i2d::CRectangle* areaPtr)
{
	if (areaPtr != m_argumentAreaPtr){
		istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);
		Q_UNUSED(notifier);

		m_argumentAreaPtr = areaPtr;
	}
}


void CAffineCalibration2d::SetResultArea(const i2d::CRectangle* areaPtr)
{
	if (areaPtr != m_resultAreaPtr){
		istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);
		Q_UNUSED(notifier);

		m_resultAreaPtr = areaPtr;
	}
}


void CAffineCalibration2d::SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	if (unitInfoPtr != m_argumentUnitInfoPtr){
		istd::CChangeNotifier notifier(this, &s_unitsChangedChangeSet);
		Q_UNUSED(notifier);

		m_argumentUnitInfoPtr = unitInfoPtr;
	}
}


void CAffineCalibration2d::SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	if (unitInfoPtr != m_resultUnitInfoPtr){
		istd::CChangeNotifier notifier(this, &s_unitsChangedChangeSet);
		Q_UNUSED(notifier);

		m_resultUnitInfoPtr = unitInfoPtr;
	}
}


// reimplemented (i2d::ICalibration2d)

const CRectangle* CAffineCalibration2d::GetArgumentArea() const
{
	return m_argumentAreaPtr;
}


const CRectangle* CAffineCalibration2d::GetResultArea() const
{
	return m_resultAreaPtr;
}


const imath::IUnitInfo* CAffineCalibration2d::GetArgumentUnitInfo() const
{
	return m_argumentUnitInfoPtr;
}


const imath::IUnitInfo* CAffineCalibration2d::GetResultUnitInfo() const
{
	return m_resultUnitInfoPtr;
}


const ICalibration2d* CAffineCalibration2d::CreateCombinedCalibration(const ITransformation2d& transformation) const
{
	const CAffineTransformation2d* affineTransformPtr = dynamic_cast<const CAffineTransformation2d*>(&transformation);
	if (affineTransformPtr != NULL){
		i2d::CAffineCalibration2d* combinedTransformPtr = new imod::TModelWrap<CAffineCalibration2d>();

		m_transformation.GetApply(affineTransformPtr->GetTransformation(), combinedTransformPtr->m_transformation);

		return combinedTransformPtr;
	}

	return NULL;
}


// reimplemented (istd::IChangeable)

bool CAffineCalibration2d::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const CAffineCalibration2d* calibrationPtr = dynamic_cast<const CAffineCalibration2d*>(&object);
	if (calibrationPtr != NULL){
		if ((mode == CM_STRICT) || (mode == CM_CONVERT)){	// we cannot convert different units, we do than strict check
			// check argument compatibility
			if (m_argumentUnitInfoPtr != calibrationPtr->m_argumentUnitInfoPtr){
				QString unitName;
				if (m_argumentUnitInfoPtr != NULL){
					unitName = m_argumentUnitInfoPtr->GetUnitName();
				}

				QString sourceUnitName;
				if (calibrationPtr->m_argumentUnitInfoPtr != NULL){
					sourceUnitName = calibrationPtr->m_argumentUnitInfoPtr->GetUnitName();
				}

				if (unitName != sourceUnitName){
					return false;
				}
			}

			// check result compatibility
			if (m_resultUnitInfoPtr != calibrationPtr->m_resultUnitInfoPtr){
				QString unitName;
				if (m_resultUnitInfoPtr != NULL){
					unitName = m_resultUnitInfoPtr->GetUnitName();
				}

				QString sourceUnitName;
				if (calibrationPtr->m_resultUnitInfoPtr != NULL){
					sourceUnitName = calibrationPtr->m_resultUnitInfoPtr->GetUnitName();
				}

				if (unitName != sourceUnitName){
					return false;
				}
			}
		}

		istd::CChangeNotifier notifier(this);
		
		SetTransformation(calibrationPtr->GetTransformation());

		if (mode == CM_WITH_REFS){
			m_argumentUnitInfoPtr = calibrationPtr->m_argumentUnitInfoPtr;
			m_resultUnitInfoPtr = calibrationPtr->m_resultUnitInfoPtr;
		}

		return true;
	}

	if ((mode == CM_STRICT) || (mode == CM_CONVERT)){	// we cannot convert different units, we do than strict check
		if ((m_argumentUnitInfoPtr != NULL) || (m_resultUnitInfoPtr != NULL)){
			return false;
		}
	}

	if (BaseClass::CopyFrom(object)){
		if (mode == CM_WITH_REFS){
			m_argumentUnitInfoPtr = NULL;
			m_resultUnitInfoPtr = NULL;
		}

		return true;
	}

	return false;
}


istd::IChangeable* CAffineCalibration2d::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CAffineCalibration2d(GetTransformation());
}


} // namespace i2d


