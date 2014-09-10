#include "i2d/CAnnulus.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "i2d/CAffine2d.h"
#include "i2d/CRectangle.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "istd/TDelPtr.h"


namespace i2d
{


CAnnulus::CAnnulus()
{
}


CAnnulus::CAnnulus(const i2d::CVector2d& center, double innerRadius, double outerRadius)
:	BaseClass(center),
	m_radiusRange(innerRadius, outerRadius)
{
	Q_ASSERT(IsAnnulusValid());
}


bool CAnnulus::IsAnnulusValid() const
{
	return m_radiusRange.IsValid();
}


double CAnnulus::GetInnerRadius() const
{
	return m_radiusRange.GetMinValue();
}


void CAnnulus::SetInnerRadius(double innerRadius)
{
	if (m_radiusRange.GetMinValue() != innerRadius && innerRadius <= m_radiusRange.GetMaxValue()){
		BeginChanges(GetAnyChange());

		m_radiusRange.SetMinValue(innerRadius);

		EndChanges(GetAnyChange());
	}
}


double CAnnulus::GetOuterRadius() const
{
	return m_radiusRange.GetMaxValue();
}


void CAnnulus::SetOuterRadius(double outerRadius)
{
	if (m_radiusRange.GetMaxValue() != outerRadius && outerRadius >= m_radiusRange.GetMinValue()){
		BeginChanges(GetAnyChange());;

		m_radiusRange.SetMaxValue(outerRadius);

		EndChanges(GetAnyChange());
	}
}


CCircle CAnnulus::GetInnerCircle() const
{
	return CCircle(m_radiusRange.GetMinValue(), m_position);
}


CCircle CAnnulus::GetOuterCircle() const
{
	return CCircle(m_radiusRange.GetMaxValue(), m_position);
}


CRectangle CAnnulus::GetBoundingBox() const
{
	double outerRadius = m_radiusRange.GetMaxValue();

	return CRectangle(m_position.GetX() - outerRadius, 
		m_position.GetY() - outerRadius,
		2 * outerRadius,
		2 * outerRadius);
}


bool CAnnulus::operator == (const CAnnulus & ref) const
{
	double deltaInner = qAbs( ref.GetInnerRadius() - GetInnerRadius() );
	double deltaOuter = qAbs( ref.GetOuterRadius() - GetOuterRadius() );

	return (deltaInner < I_BIG_EPSILON && deltaOuter < I_BIG_EPSILON) ? true : false;
}


bool CAnnulus::operator != (const CAnnulus & ref) const
{
	return !operator ==(ref);
}


// reimplemented (i2d::IObject2d)

bool CAnnulus::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos;
	if (!transformation.GetPositionAt(m_position, transPos, mode)){
		return false;
	}

	CAffine2d affine;
	if (!transformation.GetLocalTransform(m_position, affine, mode)){
		return false;
	}

	BeginChanges(GetAnyChange());

	double scale = affine.GetDeformMatrix().GetApproxScale();

	m_position = transPos;
	m_radiusRange = m_radiusRange * scale;

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	EndChanges(GetAnyChange());

	return true;
}


bool CAnnulus::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos;
	if (!transformation.GetInvPositionAt(m_position, transPos, mode)){
		return false;
	}

	CAffine2d affine;
	if (!transformation.GetLocalInvTransform(m_position, affine, mode)){
		return false;
	}

	BeginChanges(GetAnyChange());

	double scale = affine.GetDeformMatrix().GetApproxScale();

	m_position = transPos;
	m_radiusRange = m_radiusRange * scale;

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	EndChanges(GetAnyChange());

	return true;
}


bool CAnnulus::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CAnnulus* annulusPtr = dynamic_cast<CAnnulus*>(&result);
	if (annulusPtr == NULL){
		return false;
	}

	annulusPtr->BeginChanges(GetAnyChange());

	annulusPtr->m_position = m_position;
	annulusPtr->m_radiusRange = m_radiusRange;

	bool retVal = annulusPtr->Transform(transformation, mode, errorFactorPtr);

	annulusPtr->EndChanges(GetAnyChange());

	return retVal;
}


bool CAnnulus::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CAnnulus* annulusPtr = dynamic_cast<CAnnulus*>(&result);
	if (annulusPtr == NULL){
		return false;
	}

	annulusPtr->BeginChanges(GetAnyChange());

	annulusPtr->m_position = m_position;
	annulusPtr->m_radiusRange = m_radiusRange;

	bool retVal = annulusPtr->Transform(transformation, mode, errorFactorPtr);

	annulusPtr->EndChanges(GetAnyChange());

	return retVal;
}


// reimplemented (istd::IChangeable)

int CAnnulus::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CAnnulus::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CAnnulus* annulusPtr = dynamic_cast<const CAnnulus*>(&object);

	if (annulusPtr != NULL){
		BeginChanges(GetAnyChange());;

		m_position = annulusPtr->m_position;
		m_radiusRange = annulusPtr->m_radiusRange;

		CObject2dBase::CopyFrom(object, mode);

		EndChanges(GetAnyChange());

		return true;
	}

	return false;
}


istd::IChangeable* CAnnulus::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CAnnulus> clonePtr(new CAnnulus);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CAnnulus::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag innerRadiusTag("InnerRadius", "Inner radius", iser::CArchiveTag::TT_LEAF);
	static iser::CArchiveTag outerRadiusTag("OuterRadius", "Outer radius", iser::CArchiveTag::TT_LEAF);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(innerRadiusTag);
	retVal = retVal && archive.Process(m_radiusRange.GetMinValueRef());
	retVal = retVal && archive.EndTag(innerRadiusTag);

	retVal = retVal && archive.BeginTag(outerRadiusTag);
	retVal = retVal && archive.Process(m_radiusRange.GetMaxValueRef());
	retVal = retVal && archive.EndTag(outerRadiusTag);

	return retVal;
}


} // namespace i2d


