#include "i2d/CAffineTransformation2d.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace i2d
{


// public methods

const i2d::CAffine2d& CAffineTransformation2d::GetTransformation() const
{
	return m_transformation;
}


void CAffineTransformation2d::SetTransformation(const i2d::CAffine2d& transformation)
{
	istd::CChangeNotifier changePtr(this);

	m_transformation = transformation;
}


void CAffineTransformation2d::Reset()
{
	istd::CChangeNotifier changePtr(this);

	m_transformation.Reset();
}


void CAffineTransformation2d::Reset(const CVector2d& translation)
{
	istd::CChangeNotifier changePtr(this);

	m_transformation.Reset(translation);
}


void CAffineTransformation2d::Reset(const CVector2d& translation, double angle, double scale)
{
	istd::CChangeNotifier changePtr(this);

	m_transformation.Reset(translation, angle, scale);
}


void CAffineTransformation2d::Reset(const CVector2d& translation, double angle, const CVector2d& scale)
{
	istd::CChangeNotifier changePtr(this);

	m_transformation.Reset(translation, angle, scale);
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


const ITransformation2d* CAffineTransformation2d::CreateCombinedTransformation(const ITransformation2d& transform) const
{
	const CAffineTransformation2d* affineTransformPtr = dynamic_cast<const CAffineTransformation2d*>(&transform);
	if (affineTransformPtr != NULL){
		CAffineTransformation2d* combinedPtr = new CAffineTransformation2d();

		m_transformation.GetApply(affineTransformPtr->m_transformation, combinedPtr->m_transformation);

		return combinedPtr;
	}

	return NULL;
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


// reimplemented (iser::ISerializable)

bool CAffineTransformation2d::Serialize(iser::IArchive& archive)
{
	return m_transformation.Serialize(archive);
}


} // namespace i2d


