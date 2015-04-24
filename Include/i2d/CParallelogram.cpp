#include "i2d/CParallelogram.h"


// ACF includes
#include "istd/CChangeNotifier.h"
#include "istd/TDelPtr.h"


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
		istd::CChangeNotifier notifier(this, &s_objectPositionChangeSet);
		Q_UNUSED(notifier);

		m_transform.SetTranslation(position);
	}
}

i2d::CRectangle CParallelogram::GetBoundingBox() const
{
	return i2d::CRectangle();
}

bool CParallelogram::Transform(
			const i2d::ITransformation2d& transformation,
			i2d::ITransformation2d::ExactnessMode mode,
			double* /*errorFactorPtr*/)
{
	i2d::CAffine2d localTransform;
	if (transformation.GetLocalTransform(GetCenter(), localTransform, mode)){
		istd::CChangeNotifier notifier(this, &s_objectPositionAllDataChangeSet);
		Q_UNUSED(notifier);

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
		istd::CChangeNotifier notifier(this, &s_objectPositionAllDataChangeSet);
		Q_UNUSED(notifier);

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
		istd::CChangeNotifier notifier(parallelogramPtr, &s_objectPositionAllDataChangeSet);
		Q_UNUSED(notifier);

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
		istd::CChangeNotifier notifier(parallelogramPtr, &s_objectPositionAllDataChangeSet);
		Q_UNUSED(notifier);

		return parallelogramPtr->InvTransform(transformation, mode, errorFactorPtr);
	}

	return false;
}


// reimplemented (istd::IChangeable)

int CParallelogram::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CParallelogram::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CParallelogram* parallelogramPtr = dynamic_cast<const CParallelogram*>(&object);

	if (parallelogramPtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetTransform(parallelogramPtr->GetTransform());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeable* CParallelogram::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CParallelogram> clonePtr(new CParallelogram);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CParallelogram::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag transformTag("Transform", "Transformation used in parallelogram", iser::CArchiveTag::TT_GROUP);

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, &GetAllChanges());
	Q_UNUSED(notifier);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(transformTag);
	retVal = retVal && m_transform.Serialize(archive);
	retVal = retVal && archive.EndTag(transformTag);

	return retVal;
}


} // namespace i2d


