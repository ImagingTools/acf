#include <i2d/CPerspectiveCalibration2d.h>


// ACF includes
#include <istd/CChangeNotifier.h>


namespace i2d
{


// static constants
static const iser::CArchiveTag s_affinePartTag("AffinePart", "Part of calibration representing affine transformation", iser::CArchiveTag::TT_GROUP);
static const iser::CArchiveTag s_perspAxisTag("PerspAxis", "Axis of perspective deformation", iser::CArchiveTag::TT_GROUP);
static const iser::CArchiveTag s_logicalAreaTag("LogicalArea", "Area where this calibration is valid", iser::CArchiveTag::TT_GROUP);
static const istd::IChangeable::ChangeSet s_calibrationChangedChangeSet(ICalibration2d::CF_CALIBRATION_CHANGED, "Calibration changed");
static const istd::IChangeable::ChangeSet s_areaChangedChangeSet(ICalibration2d::CF_AREA_CHANGED, "Calibration area changed");
static const istd::IChangeable::ChangeSet s_unitsChangedChangeSet(ICalibration2d::CF_UNITS_CHANGED, "Calibration units changed");


CPerspectiveCalibration2d::CPerspectiveCalibration2d()
:	m_argumentAreaPtr(NULL),
	m_resultAreaPtr(NULL),
	m_argumentUnitInfoPtr(NULL),
	m_resultUnitInfoPtr(NULL)
{
	m_affinePart.Reset();
	m_perspAxis.Reset();
	m_transformationFlags = TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_PRESERVE_NULL | TF_PRESERVE_DISTANCE | TF_PRESERVE_ANGLE | TF_AFFINE | TF_CONTINUES;
}


CPerspectiveCalibration2d::CPerspectiveCalibration2d(const i2d::CAffine2d& affinePart, const i2d::CVector2d& perspectiveAxis)
:	m_affinePart(affinePart),
	m_perspAxis(perspectiveAxis),
	m_argumentAreaPtr(NULL),
	m_resultAreaPtr(NULL),
	m_argumentUnitInfoPtr(NULL),
	m_resultUnitInfoPtr(NULL)
{
	CalcTransformationFlags();
}


void CPerspectiveCalibration2d::Reset()
{
	istd::CChangeNotifier notifier(this, &GetAllChanges());
	Q_UNUSED(notifier);

	m_affinePart.Reset();
	m_perspAxis.Reset();
}


void CPerspectiveCalibration2d::SetAffinePart(const i2d::CAffine2d& value)
{
	if (value != m_affinePart){
		istd::CChangeNotifier notifier(this, &s_calibrationChangedChangeSet);
		Q_UNUSED(notifier);

		m_affinePart = value;
	}
}


void CPerspectiveCalibration2d::SetPerspAxis(const i2d::CVector2d& value)
{
	if (value != m_perspAxis){
		istd::CChangeNotifier notifier(this, &s_calibrationChangedChangeSet);
		Q_UNUSED(notifier);

		m_perspAxis = value;
	}
}


void CPerspectiveCalibration2d::SetArgumentArea(const i2d::CRectangle* areaPtr)
{
	if (areaPtr != m_argumentAreaPtr){
		istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);
		Q_UNUSED(notifier);

		m_argumentAreaPtr = areaPtr;
	}
}


void CPerspectiveCalibration2d::SetResultArea(const i2d::CRectangle* areaPtr)
{
	if (areaPtr != m_resultAreaPtr){
		istd::CChangeNotifier notifier(this, &s_areaChangedChangeSet);
		Q_UNUSED(notifier);

		m_resultAreaPtr = areaPtr;
	}
}


void CPerspectiveCalibration2d::SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	if (unitInfoPtr != m_argumentUnitInfoPtr){
		istd::CChangeNotifier notifier(this, &s_unitsChangedChangeSet);
		Q_UNUSED(notifier);

		m_argumentUnitInfoPtr = unitInfoPtr;
	}
}


void CPerspectiveCalibration2d::SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	if (unitInfoPtr != m_resultUnitInfoPtr){
		istd::CChangeNotifier notifier(this, &s_unitsChangedChangeSet);
		Q_UNUSED(notifier);

		m_resultUnitInfoPtr = unitInfoPtr;
	}
}


bool CPerspectiveCalibration2d::operator==(const CPerspectiveCalibration2d& calib) const
{
	return		(m_affinePart == calib.m_affinePart) &&
				(m_perspAxis == calib.m_perspAxis);
}


// reimplemented (i2d::ICalibration2d)

const i2d::CRectangle* CPerspectiveCalibration2d::GetArgumentArea() const
{
	return m_argumentAreaPtr;
}


const i2d::CRectangle* CPerspectiveCalibration2d::GetResultArea() const
{
	return m_resultAreaPtr;
}


const imath::IUnitInfo* CPerspectiveCalibration2d::GetArgumentUnitInfo() const
{
	return m_argumentUnitInfoPtr;
}


const imath::IUnitInfo* CPerspectiveCalibration2d::GetResultUnitInfo() const
{
	return m_resultUnitInfoPtr;
}


const ICalibration2d* CPerspectiveCalibration2d::CreateCombinedCalibration(const ITransformation2d& /*transformation*/) const
{
	return NULL;
}


// reimplemented (i2d::ITransformation2d)

int CPerspectiveCalibration2d::GetTransformationFlags() const
{
	return m_transformationFlags;
}


bool CPerspectiveCalibration2d::GetDistance(
			const i2d::CVector2d& origPos1,
			const i2d::CVector2d& origPos2,
			double& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d log1;
	i2d::CVector2d log2;

	if (		GetPositionAt(origPos1, log1) &&
				GetPositionAt(origPos2, log2)){
		result = log1.GetDistance(log2);

		return true;
	}

	return false;
}


bool CPerspectiveCalibration2d::GetPositionAt(
			const i2d::CVector2d& origPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	double persDivider = m_perspAxis.GetDotProduct(origPosition) + 1;
	if (persDivider > I_BIG_EPSILON){
		result = m_affinePart.GetApply(origPosition) / persDivider;

		return true;
	}

	return false;
}


bool CPerspectiveCalibration2d::GetInvPositionAt(
			const i2d::CVector2d& transfPosition,
			i2d::CVector2d& result,
			ExactnessMode /*mode*/) const
{
	i2d::CVector2d delta = transfPosition - m_affinePart.GetTranslation();
	i2d::CMatrix2d perspMatrix = m_affinePart.GetDeformMatrix() - i2d::CMatrix2d(transfPosition * m_perspAxis.GetX(), transfPosition * m_perspAxis.GetY());

	double det = perspMatrix.GetDet();

	if (qAbs(det) > I_BIG_EPSILON){
		result.SetX((perspMatrix.GetAt(1, 1) * delta.GetX() - perspMatrix.GetAt(1, 0) * delta.GetY()) / det);
		result.SetY((perspMatrix.GetAt(0, 0) * delta.GetY() - perspMatrix.GetAt(0, 1) * delta.GetX()) / det);

		return true;
	}
	else{
		return false;
	}
}


bool CPerspectiveCalibration2d::GetLocalTransform(
			const i2d::CVector2d& origPosition,
			i2d::CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	double persDivider = m_perspAxis.GetDotProduct(origPosition) + 1;
	if (persDivider > I_BIG_EPSILON){
		result = i2d::CAffine2d(m_affinePart.GetDeformMatrix() / persDivider, m_affinePart.GetTranslation() / persDivider);

		return true;
	}

	return false;
}


bool CPerspectiveCalibration2d::GetLocalInvTransform(
			const i2d::CVector2d& transfPosition,
			i2d::CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	i2d::CMatrix2d perspMatrix = m_affinePart.GetDeformMatrix() - i2d::CMatrix2d(transfPosition * m_perspAxis.GetX(), transfPosition * m_perspAxis.GetY());

	double det = perspMatrix.GetDet();

	if (qAbs(det) > I_BIG_EPSILON){
		i2d::CMatrix2d deformMatrx = perspMatrix.GetInverted();
		result.SetDeformMatrix(deformMatrx);
		result.SetTranslation(deformMatrx.GetMultiplied(transfPosition) - m_affinePart.GetTranslation());	// TODO: not sure if it is correct (CPerspectiveCalibration2d::GetLocalTransform)

		return true;
	}
	else{
		return false;
	}
}


// reimplemented (imath::TISurjectFunction)

bool CPerspectiveCalibration2d::GetInvValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetInvPositionAt(argument, result);
}


i2d::CVector2d CPerspectiveCalibration2d::GetInvValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetInvValueAt(argument, retVal);

	return retVal;
}


// reimplemented (imath::TIMathFunction)

bool CPerspectiveCalibration2d::GetValueAt(const i2d::CVector2d& argument, i2d::CVector2d& result) const
{
	return GetPositionAt(argument, result);
}


i2d::CVector2d CPerspectiveCalibration2d::GetValueAt(const i2d::CVector2d& argument) const
{
	i2d::CVector2d retVal;
	
	GetValueAt(argument, retVal);

	return retVal;
}
	
	
// reimplemented (iser::ISerializable)

bool CPerspectiveCalibration2d::Serialize(iser::IArchive& archive)
{
	bool isStoring = archive.IsStoring();

	istd::CChangeNotifier notifier(isStoring? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(s_affinePartTag);
	retVal = retVal && m_affinePart.Serialize(archive);
	retVal = retVal && archive.EndTag(s_affinePartTag);

	retVal = retVal && archive.BeginTag(s_perspAxisTag);
	retVal = retVal && m_perspAxis.Serialize(archive);
	retVal = retVal && archive.EndTag(s_perspAxisTag);

	return retVal;
}


// reimplemented (istd::IChangeable)

int CPerspectiveCalibration2d::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE;
}


bool CPerspectiveCalibration2d::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const CPerspectiveCalibration2d* calibrationImplPtr = dynamic_cast<const CPerspectiveCalibration2d*>(&object);
	if (calibrationImplPtr != NULL){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		*this = *calibrationImplPtr;

		return true;
	}
	else{
		const i2d::ICalibration2d* calibrationPtr = dynamic_cast<const i2d::ICalibration2d*>(&object);
		if (calibrationPtr != NULL){
			int flags = calibrationPtr->GetTransformationFlags();

			if ((mode == CM_CONVERT) || ((flags & TF_AFFINE) != 0)){
				i2d::CAffine2d affinePart;
				if (calibrationPtr->GetLocalTransform(i2d::CVector2d::GetZero(), affinePart, EM_EXACT)){
					istd::CChangeNotifier notifier(this);
					Q_UNUSED(notifier);

					m_affinePart = affinePart;
					m_perspAxis.Reset();
					m_transformationFlags = TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_CONTINUES | TF_AFFINE;

					if (m_affinePart.GetTranslation().IsNull()){
						m_transformationFlags |= TF_PRESERVE_NULL;
					}

					return true;
				}
			}
		}
	}

	return false;
}


bool CPerspectiveCalibration2d::IsEqual(const istd::IChangeable& object) const
{
	const CPerspectiveCalibration2d* calibrationImplPtr = dynamic_cast<const CPerspectiveCalibration2d*>(&object);
	if (calibrationImplPtr != NULL){
		return *this == *calibrationImplPtr;
	}
	else if ((m_transformationFlags & TF_AFFINE) != 0){
		const i2d::ICalibration2d* calibrationPtr = dynamic_cast<const i2d::ICalibration2d*>(&object);
		if (calibrationPtr != NULL){
			int flags = calibrationPtr->GetTransformationFlags();

			if ((flags & TF_AFFINE) != 0){
				i2d::CAffine2d affinePart;
				if (calibrationPtr->GetLocalTransform(i2d::CVector2d::GetZero(), affinePart, EM_EXACT)){
					return m_affinePart == affinePart;
				}
			}
		}
	}

	return false;
}


istd::IChangeable* CPerspectiveCalibration2d::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CPerspectiveCalibration2d(*this);
}


bool CPerspectiveCalibration2d::ResetData(CompatibilityMode /*mode*/)
{
	Reset();

	return true;
}


// protected methods

void CPerspectiveCalibration2d::CalcTransformationFlags()
{
	m_transformationFlags = TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_CONTINUES;

	if (m_perspAxis.IsNull()){
		m_transformationFlags |= TF_AFFINE;

		i2d::CVector2d axisX = m_affinePart.GetDeformMatrix().GetAxisX();
		i2d::CVector2d axisY = m_affinePart.GetDeformMatrix().GetAxisY();
		if ((qAbs(axisX.GetDotProduct(axisY)) < I_BIG_EPSILON) && (qAbs(axisX.GetLength2() - axisY.GetLength2()) < I_BIG_EPSILON)){
			m_transformationFlags |= TF_PRESERVE_ANGLE;

			if (qAbs(axisX.GetLength2() - 1) < I_BIG_EPSILON){
				m_transformationFlags |= TF_PRESERVE_DISTANCE;
			}
		}
	}

	if (m_affinePart.GetTranslation().IsNull()){
		m_transformationFlags |= TF_PRESERVE_NULL;
	}
}


// reimplemented (istd::IChangeable)

void CPerspectiveCalibration2d::OnEndChanges(const ChangeSet& changeSet)
{
	if (changeSet.Contains(CF_CALIBRATION_CHANGED)){
		CalcTransformationFlags();
	}
}


} // namespace i2d


