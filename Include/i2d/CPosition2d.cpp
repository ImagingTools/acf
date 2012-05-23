#include "i2d/CPosition2d.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CRectangle.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


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
		istd::CChangeNotifier notifier(this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

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


// reimplemented (iser::ISerializable)

bool CPosition2d::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag centerTag("Center", "Center position");

	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

	bool retVal = true;

	retVal = retVal && archive.BeginTag(centerTag);
	retVal = retVal && m_position.Serialize(archive);
	retVal = retVal && archive.EndTag(centerTag);

	return retVal;
}


} // namespace i2d


