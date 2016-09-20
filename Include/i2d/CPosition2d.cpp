#include <i2d/CPosition2d.h>


// ACF includes
#include <istd/TDelPtr.h>
#include <istd/CChangeNotifier.h>
#include <iser/IArchive.h>
#include <iser/CArchiveTag.h>
#include <i2d/CRectangle.h>


namespace i2d
{


CPosition2d::CPosition2d()
:	m_position(0, 0)
{
}


CPosition2d::CPosition2d(const CVector2d& position)
:	m_position(position)
{
}


void CPosition2d::SetPosition(const CVector2d& position)
{
	if (position != m_position){
		istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
		Q_UNUSED(changeNotifier);

		m_position = position;
	}
}


// reimplemented (i2d::IObject2d)

CVector2d CPosition2d::GetCenter() const
{
	return m_position;
}


void CPosition2d::MoveCenterTo(const CVector2d& position)
{
	SetPosition(position);
}


CRectangle CPosition2d::GetBoundingBox() const
{
	return CRectangle(m_position.GetX() - I_BIG_EPSILON, m_position.GetY() - I_BIG_EPSILON, I_BIG_EPSILON * 2, I_BIG_EPSILON * 2);
}


bool CPosition2d::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos;
	if (!transformation.GetPositionAt(m_position, transPos, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	SetPosition(transPos);

	return true;
}


bool CPosition2d::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	CVector2d transPos;
	if (!transformation.GetInvPositionAt(m_position, transPos, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	SetPosition(transPos);

	return true;
}


bool CPosition2d::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CPosition2d* resultPositionPtr = dynamic_cast<CPosition2d*>(&result);
	if (resultPositionPtr == NULL){
		return false;
	}

	CVector2d transPos;
	if (!transformation.GetPositionAt(m_position, transPos, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	resultPositionPtr->SetPosition(transPos);

	return true;
}


bool CPosition2d::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CPosition2d* resultPositionPtr = dynamic_cast<CPosition2d*>(&result);
	if (resultPositionPtr == NULL){
		return false;
	}

	CVector2d transPos;
	if (!transformation.GetInvPositionAt(m_position, transPos, mode)){
		return false;
	}

	if (errorFactorPtr != NULL){
		*errorFactorPtr = 0;
	}

	resultPositionPtr->SetPosition(transPos);

	return true;
}


// reimplemented (istd::IChangeable)

int CPosition2d::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE | SO_COMPARE;
}


bool CPosition2d::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CPosition2d* position2dPtr = dynamic_cast<const CPosition2d*>(&object);

	if (position2dPtr != NULL){
		istd::CChangeNotifier changeNotifier(this);
		Q_UNUSED(changeNotifier);

		SetPosition(position2dPtr->GetPosition());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


bool CPosition2d::IsEqual(const istd::IChangeable& object) const
{
	const CPosition2d* position2dPtr = dynamic_cast<const CPosition2d*>(&object);
	if (position2dPtr != NULL){
		return (m_position == position2dPtr->m_position);
	}

	return false;
}


istd::IChangeable* CPosition2d::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CPosition2d> clonePtr(new CPosition2d);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CPosition2d::Serialize(iser::IArchive& archive)
{
	const iser::IVersionInfo& versionInfo = archive.GetVersionInfo();	// TODO: Remove it when backward compatibility to older versions will not be no more important
	quint32 frameworkVersion = 0;
	if (versionInfo.GetVersionNumber(iser::IVersionInfo::AcfVersionId, frameworkVersion) && (frameworkVersion < 4019)){
		static iser::CArchiveTag centerTag("Center", "Center position", iser::CArchiveTag::TT_GROUP);

		istd::CChangeNotifier changeNotifier(archive.IsStoring()? NULL: this, &GetAllChanges());
		Q_UNUSED(changeNotifier);

		bool retVal = true;

		retVal = retVal && archive.BeginTag(centerTag);
		retVal = retVal && m_position.Serialize(archive);
		retVal = retVal && archive.EndTag(centerTag);

		return retVal;
	}

	return m_position.Serialize(archive);
}


} // namespace i2d


