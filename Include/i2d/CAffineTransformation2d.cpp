#include "i2d/CAffineTransformation2d.h"


// ACF includes
#include "imod/TModelWrap.h"
#include "istd/CChangeNotifier.h"
#include "i2d/CRectangle.h"


namespace i2d
{


// public methods

CAffineTransformation2d::CAffineTransformation2d()
:	m_argumentUnitInfoPtr(NULL),
	m_resultUnitInfoPtr(NULL)
{
	m_transformation.Reset();
}


CAffineTransformation2d::CAffineTransformation2d(const i2d::CAffine2d& transformation)
:	m_transformation(transformation),
	m_argumentUnitInfoPtr(NULL),
	m_resultUnitInfoPtr(NULL)
{
}


void CAffineTransformation2d::SetTransformation(const i2d::CAffine2d& transformation)
{
	BeginChanges(GetAnyChange());

	m_transformation = transformation;

	EndChanges(GetAnyChange());
}


void CAffineTransformation2d::Reset()
{
	BeginChanges(GetAnyChange());

	m_transformation.Reset();

	EndChanges(GetAnyChange());
}


void CAffineTransformation2d::Reset(const CVector2d& translation)
{
	BeginChanges(GetAnyChange());

	m_transformation.Reset(translation);

	EndChanges(GetAnyChange());
}


void CAffineTransformation2d::Reset(const CVector2d& translation, double angle, double scale)
{
	BeginChanges(GetAnyChange());

	m_transformation.Reset(translation, angle, scale);

	EndChanges(GetAnyChange());
}


void CAffineTransformation2d::Reset(const CVector2d& translation, double angle, const CVector2d& scale)
{
	BeginChanges(GetAnyChange());

	m_transformation.Reset(translation, angle, scale);

	EndChanges(GetAnyChange());
}


void CAffineTransformation2d::SetArgumentUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	m_argumentUnitInfoPtr = unitInfoPtr;
}


void CAffineTransformation2d::SetResultUnitInfo(const imath::IUnitInfo* unitInfoPtr)
{
	m_resultUnitInfoPtr = unitInfoPtr;
}


// reimplemented (i2d::ICalibration2d)

const imath::IUnitInfo* CAffineTransformation2d::GetArgumentUnitInfo() const
{
	return m_argumentUnitInfoPtr;
}


const imath::IUnitInfo* CAffineTransformation2d::GetResultUnitInfo() const
{
	return m_resultUnitInfoPtr;
}


const ICalibration2d* CAffineTransformation2d::CreateCombinedCalibration(const ICalibration2d& calibration) const
{
	const CAffineTransformation2d* affineTransformPtr = dynamic_cast<const CAffineTransformation2d*>(&calibration);
	if (affineTransformPtr != NULL){
		i2d::CAffineTransformation2d* combinedTransformPtr = new imod::TModelWrap<CAffineTransformation2d>();

		m_transformation.GetApply(affineTransformPtr->m_transformation, combinedTransformPtr->m_transformation);

		return combinedTransformPtr;
	}

	return NULL;
}


// reimplemented (i2d::ITransformation2d)

int CAffineTransformation2d::GetTransformationFlags() const
{
	return TF_FORWARD | TF_INVERTED | TF_INJECTIVE | TF_SURJECTIVE | TF_AFFINE | TF_CONTINUES;
}


bool CAffineTransformation2d::GetDistance(
			const CVector2d& origPos1,
			const CVector2d& origPos2,
			double& result,
			ExactnessMode /*mode*/) const
{
	result = m_transformation.GetApply(origPos1).GetDistance(m_transformation.GetApply(origPos2));

	return true;
}


bool CAffineTransformation2d::GetPositionAt(
			const CVector2d& origPosition,
			CVector2d& result,
			ExactnessMode /*mode*/) const
{
	m_transformation.GetApply(origPosition, result);

	return true;
}


bool CAffineTransformation2d::GetInvPositionAt(
			const CVector2d& transfPosition,
			CVector2d& result,
			ExactnessMode /*mode*/) const
{
	return m_transformation.GetInvertedApply(transfPosition, result);
}


bool CAffineTransformation2d::GetLocalTransform(
			const CVector2d& /*origPosition*/,
			CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	result = m_transformation;

	return true;
}


bool CAffineTransformation2d::GetLocalInvTransform(
			const CVector2d& /*transfPosition*/,
			CAffine2d& result,
			ExactnessMode /*mode*/) const
{
	return m_transformation.GetInverted(result);
}


// reimplemented (imath::TISurjectFunction)

bool CAffineTransformation2d::GetInvValueAt(const CVector2d& argument, CVector2d& result) const
{
	return m_transformation.GetInvertedApply(argument, result);
}


CVector2d CAffineTransformation2d::GetInvValueAt(const CVector2d& argument) const
{
	return m_transformation.GetInvertedApply(argument);
}


// reimplemented (imath::TIMathFunction)

bool CAffineTransformation2d::GetValueAt(const CVector2d& argument, CVector2d& result) const
{
	m_transformation.GetApply(argument, result);

	return true;
}


CVector2d CAffineTransformation2d::GetValueAt(const CVector2d& argument) const
{
	return m_transformation.GetApply(argument);
}


// reimplemented (i2d::IObject2d)

CVector2d CAffineTransformation2d::GetCenter() const
{
	return m_transformation.GetTranslation();
}


void CAffineTransformation2d::MoveCenterTo(const CVector2d& position)
{
	return m_transformation.SetTranslation(position);
}


CRectangle CAffineTransformation2d::GetBoundingBox() const
{
	CRectangle retVal(m_transformation.GetTranslation(), m_transformation.GetTranslation());

	retVal.Unite(m_transformation.GetTranslation() + m_transformation.GetDeformMatrix().GetAxisX());
	retVal.Unite(m_transformation.GetTranslation() + m_transformation.GetDeformMatrix().GetAxisY());
	retVal.Unite(m_transformation.GetTranslation() + m_transformation.GetDeformMatrix().GetAxisX() + m_transformation.GetDeformMatrix().GetAxisY());

	return retVal;
}


// reimplemented (iser::ISerializable)

bool CAffineTransformation2d::Serialize(iser::IArchive& archive)
{
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this);

	return m_transformation.Serialize(archive);
}


// reimplemented (istd::IChangeable)

bool CAffineTransformation2d::CopyFrom(const istd::IChangeable& object, CompatibilityMode mode)
{
	const CAffineTransformation2d* sourcePtr = dynamic_cast<const CAffineTransformation2d*>(&object);
	if (sourcePtr != NULL){
		if ((mode == CM_STRICT) || (mode == CM_CONVERT)){	// we cannot convert different units, we do than strict check
			// check argument compatibility
			if (m_argumentUnitInfoPtr != sourcePtr->m_argumentUnitInfoPtr){
				QString unitName;
				if (m_argumentUnitInfoPtr != NULL){
					unitName = m_argumentUnitInfoPtr->GetUnitName();
				}

				QString sourceUnitName;
				if (sourcePtr->m_argumentUnitInfoPtr != NULL){
					sourceUnitName = sourcePtr->m_argumentUnitInfoPtr->GetUnitName();
				}

				if (unitName != sourceUnitName){
					return false;
				}
			}

			// check result compatibility
			if (m_resultUnitInfoPtr != sourcePtr->m_resultUnitInfoPtr){
				QString unitName;
				if (m_resultUnitInfoPtr != NULL){
					unitName = m_resultUnitInfoPtr->GetUnitName();
				}

				QString sourceUnitName;
				if (sourcePtr->m_resultUnitInfoPtr != NULL){
					sourceUnitName = sourcePtr->m_resultUnitInfoPtr->GetUnitName();
				}

				if (unitName != sourceUnitName){
					return false;
				}
			}
		}

		BeginChanges(GetAnyChange());
		
		m_transformation = sourcePtr->m_transformation;

		if (mode == CM_WITH_REFS){
			m_argumentUnitInfoPtr = sourcePtr->m_argumentUnitInfoPtr;
			m_resultUnitInfoPtr = sourcePtr->m_resultUnitInfoPtr;
		}

		EndChanges(GetAnyChange());

		return true;
	}

	return false;
}


istd::IChangeable* CAffineTransformation2d::CloneMe(CompatibilityMode /*mode*/) const
{
	return new CAffineTransformation2d(m_transformation);
}


} // namespace i2d


