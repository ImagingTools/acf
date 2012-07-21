#include "i2d/CParallelogram.h"


// ACF includes
#include "istd/TChangeNotifier.h"


namespace i2d
{


// reimplemented (i2d::IObject2d)

i2d::CVector2d CParallelogram::GetCenter() const
{
	return m_transform.GetTranslation();
}


void CParallelogram::MoveCenterTo(const i2d::CVector2d& position)
{
	if (m_transform.GetTranslation() != position){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_transform.SetTranslation(position);
	}
}


bool CParallelogram::Transform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* /*errorFactorPtr*/)
{
	i2d::CAffine2d localTransform;
	if (transformation.GetLocalTransform(GetCenter(), localTransform, mode)){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_transform.Apply(localTransform);

		return true;
	}

	return true;
}


bool CParallelogram::InvTransform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* /*errorFactorPtr*/)
{
	i2d::CAffine2d localTransform;
	if (transformation.GetLocalInvTransform(GetCenter(), localTransform, mode)){
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_transform.Apply(localTransform);

		return true;
	}

	return true;
}


bool CParallelogram::GetTransformed(
			const i2d::ITransformation2d& transformation,
			i2d::IObject2d& result,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CParallelogram* parallelogramPtr = dynamic_cast<CParallelogram*>(&result);
	if (parallelogramPtr != NULL){
		istd::CChangeNotifier notifier(parallelogramPtr, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		return parallelogramPtr->Transform(transformation, mode, errorFactorPtr);
	}

	return false;
}


bool CParallelogram::GetInvTransformed(
			const i2d::ITransformation2d& transformation,
			i2d::IObject2d& result,
			i2d::ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CParallelogram* parallelogramPtr = dynamic_cast<CParallelogram*>(&result);
	if (parallelogramPtr != NULL){
		istd::CChangeNotifier notifier(parallelogramPtr, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		return parallelogramPtr->InvTransform(transformation, mode, errorFactorPtr);
	}

	return false;
}


// reimplemented (iser::ISerializable)

bool CParallelogram::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag transformTag("Transform", "Transformation used in parallelogram");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(transformTag);
	retVal = retVal && m_transform.Serialize(archive);
	retVal = retVal && archive.EndTag(transformTag);

	return retVal;
}


} // namespace i2d

