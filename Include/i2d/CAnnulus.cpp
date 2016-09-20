#include <i2d/CAnnulus.h>

// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <i2d/CAffine2d.h>
#include <i2d/CRectangle.h>
#include <i2d/CAnnulusSegment.h>
#include <i2d/CPolygon.h>
#include <i2d/CPolyline.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>


namespace i2d
{

	
// static constants
static const iser::CArchiveTag s_innerRadiusTag("InnerRadius", "Inner radius", iser::CArchiveTag::TT_LEAF);
static const iser::CArchiveTag s_outerRadiusTag("OuterRadius", "Outer radius", iser::CArchiveTag::TT_LEAF);



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
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_radiusRange.SetMinValue(innerRadius);
	}
}


double CAnnulus::GetOuterRadius() const
{
	return m_radiusRange.GetMaxValue();
}


void CAnnulus::SetOuterRadius(double outerRadius)
{
	if (m_radiusRange.GetMaxValue() != outerRadius && outerRadius >= m_radiusRange.GetMinValue()){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_radiusRange.SetMaxValue(outerRadius);
	}
}


void CAnnulus::SetInnerOuterRadius(double radius1, double radius2)
{
	istd::CChangeNotifier changeNotifier(this);
	Q_UNUSED(changeNotifier);

	m_radiusRange.SetMinValue(qMin(radius1, radius2));
	m_radiusRange.SetMaxValue(qMax(radius1, radius2));
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


bool CAnnulus::ConvertToPolygon(i2d::CPolygon& result, int segmentsCount) const
{
	double beginAngle = 0;
	double endAngle = 2 * I_PI;
	const i2d::CAnnulusSegment* segmentPtr = dynamic_cast<const i2d::CAnnulusSegment*>(this);
	if (segmentPtr != NULL){
		beginAngle = segmentPtr->GetBeginAngle();
		endAngle = segmentPtr->GetEndAngle();
	}

	double minRadius = m_radiusRange.GetMinValue();
	double maxRadius = m_radiusRange.GetMaxValue();

	if (segmentsCount < 3){
		segmentsCount = int(minRadius + maxRadius) * (endAngle - beginAngle) * 0.5 + 1;
	}

	i2d::CVector2d directionVector;

	std::vector<i2d::CVector2d> innerPoints(segmentsCount + 1);
	std::vector<i2d::CVector2d> outerPoints(segmentsCount + 1);

	for (int i = 0; i < segmentsCount; ++i){
		double alpha = (double)i / segmentsCount;
		double angle = alpha * (endAngle - beginAngle) + beginAngle;

		directionVector.Init(angle);

		innerPoints[i] = m_position + directionVector * minRadius;
		outerPoints[i] = m_position + directionVector * maxRadius;
	}

	// last point
	directionVector.Init(endAngle);
	innerPoints[segmentsCount] = m_position + directionVector * minRadius;
	outerPoints[segmentsCount] = m_position + directionVector * maxRadius;

	istd::CChangeNotifier notifier(&result, &GetAllChanges());
	Q_UNUSED(notifier);

	result.SetNodesCount(segmentsCount * 2 + 2);
	result.SetCalibration(GetCalibration());

	int index = 0;
	for (int i = 0; i <= segmentsCount; ++i){
		result.SetNodePos(index++, outerPoints.at(i));
	}				
	for (int i = segmentsCount; i >= 0 ; --i){
		result.SetNodePos(index++, innerPoints.at(i));
	}				

	// close if polyline
	i2d::CPolyline* polylinePtr = dynamic_cast<i2d::CPolyline*>(&result);
	if (polylinePtr){
		polylinePtr->SetClosed(true);
	}

	return true;
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

	istd::CChangeNotifier changeNotifier(this);
	Q_UNUSED(changeNotifier);

	double scale = affine.GetDeformMatrix().GetApproxScale();

	m_position = transPos;
	m_radiusRange = m_radiusRange * scale;

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

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

	istd::CChangeNotifier changeNotifier(this);
	Q_UNUSED(changeNotifier);

	double scale = affine.GetDeformMatrix().GetApproxScale();

	m_position = transPos;
	m_radiusRange = m_radiusRange * scale;

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

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

	istd::CChangeNotifier changeNotifier(annulusPtr);
	Q_UNUSED(changeNotifier);

	annulusPtr->m_position = m_position;
	annulusPtr->m_radiusRange = m_radiusRange;

	return annulusPtr->Transform(transformation, mode, errorFactorPtr);
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
	
	istd::CChangeNotifier changeNotifier(annulusPtr);
	Q_UNUSED(changeNotifier);

	annulusPtr->m_position = m_position;
	annulusPtr->m_radiusRange = m_radiusRange;

	return annulusPtr->Transform(transformation, mode, errorFactorPtr);
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
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		m_position = annulusPtr->m_position;
		m_radiusRange = annulusPtr->m_radiusRange;

		CObject2dBase::CopyFrom(object, mode);

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
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = BaseClass::Serialize(archive);

	retVal = retVal && archive.BeginTag(s_innerRadiusTag);
	retVal = retVal && archive.Process(m_radiusRange.GetMinValueRef());
	retVal = retVal && archive.EndTag(s_innerRadiusTag);

	retVal = retVal && archive.BeginTag(s_outerRadiusTag);
	retVal = retVal && archive.Process(m_radiusRange.GetMaxValueRef());
	retVal = retVal && archive.EndTag(s_outerRadiusTag);

	return retVal;
}


} // namespace i2d


