#include "i2d/CQuadrangle.h"


// ACF includes
#include "istd/CChangeNotifier.h"

#include "i2d/CVector2d.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "istd/TDelPtr.h"


namespace i2d
{


// public methods

CQuadrangle::CQuadrangle()
{
}


CQuadrangle::CQuadrangle(const CLine2d& firstDiagonal, const CLine2d& secondDiagonal)
:	m_firstDiagonal(firstDiagonal),
	m_secondDiagonal(secondDiagonal)
{
}


bool CQuadrangle::operator==(const CQuadrangle& quadrangle) const
{
	return ((m_firstDiagonal == quadrangle.m_firstDiagonal) && (m_secondDiagonal == quadrangle.m_secondDiagonal));
}


bool CQuadrangle::operator!=(const CQuadrangle& quadrangle) const
{
	return !operator==(quadrangle);
}


bool CQuadrangle::IsQuadrangleValid() const
{
	return m_firstDiagonal.IsIntersectedBy(m_secondDiagonal);
}


bool CQuadrangle::IsQuadrangleEmpty() const
{
	return m_firstDiagonal == m_secondDiagonal || m_firstDiagonal.IsNull() || m_secondDiagonal.IsNull();
}


const CLine2d& CQuadrangle::GetFirstDiagonal() const
{
	return m_firstDiagonal;
}


void CQuadrangle::SetFirstDiagonal(const CLine2d& firstDiagonal)
{
	if (m_firstDiagonal != firstDiagonal){
		istd::CChangeNotifier changePtr(this);
		
		m_firstDiagonal = firstDiagonal;
	}
}


const CLine2d& CQuadrangle::GetSecondDiagonal() const
{
	return m_secondDiagonal;
}


void CQuadrangle::SetSecondDiagonal(const CLine2d& secondDiagonal)
{
	if (m_secondDiagonal != secondDiagonal){
		istd::CChangeNotifier changePtr(this);
		
		m_secondDiagonal = secondDiagonal;
	}
}


// reimplemented (i2d::IObject2d)

CVector2d CQuadrangle::GetCenter() const
{
	CVector2d intersectionPoint;
	if (m_firstDiagonal.GetIntersection(m_secondDiagonal, intersectionPoint)){
		return intersectionPoint;
	}

	return m_firstDiagonal.GetCenter();
}


void CQuadrangle::MoveCenterTo(const CVector2d& position)
{
	CVector2d delta = position - GetCenter();
	if (delta != CVector2d(0, 0)){
		static ChangeSet changeSet(CF_OBJECT_POSITION);
		istd::CChangeNotifier notifier(this, changeSet);

		m_firstDiagonal.MoveCenterTo(delta + m_firstDiagonal.GetCenter());
		m_secondDiagonal.MoveCenterTo(delta + m_secondDiagonal.GetCenter());
	}
}


CRectangle CQuadrangle::GetBoundingBox() const
{
	return m_firstDiagonal.GetBoundingBox().GetUnion(m_secondDiagonal.GetBoundingBox());
}


bool CQuadrangle::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	static ChangeSet changeSet(CF_OBJECT_POSITION, CF_ALL_DATA);
	istd::CChangeNotifier notifier(this, changeSet);

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		bool retVal =
					m_firstDiagonal.Transform(transformation, mode, &errorFactor1) &&
					m_secondDiagonal.Transform(transformation, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;

		return retVal;
	}
	else{
		return		m_firstDiagonal.Transform(transformation, mode) &&
					m_secondDiagonal.Transform(transformation, mode);
	}
}


bool CQuadrangle::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	static ChangeSet changeSet(CF_OBJECT_POSITION, CF_ALL_DATA);
	istd::CChangeNotifier notifier(this, changeSet);

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		bool retVal =
					m_firstDiagonal.InvTransform(transformation, mode, &errorFactor1) &&
					m_secondDiagonal.InvTransform(transformation, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;

		return retVal;
	}
	else{
		return	m_firstDiagonal.InvTransform(transformation, mode) &&
				m_secondDiagonal.InvTransform(transformation, mode);
	}
}


bool CQuadrangle::GetTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CQuadrangle* resultQuadranglePtr = dynamic_cast<CQuadrangle*>(&result);
	if (resultQuadranglePtr == NULL){
		return false;
	}

	static ChangeSet changeSet(CF_OBJECT_POSITION, CF_ALL_DATA);
	istd::CChangeNotifier notifier(resultQuadranglePtr, changeSet);

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		bool retVal =
					m_firstDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode, &errorFactor1) &&
					m_secondDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;

		return retVal;
	}
	else{
		return	m_firstDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode) &&
				m_secondDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode);
	}
}


bool CQuadrangle::GetInvTransformed(
			const ITransformation2d& transformation,
			IObject2d& result,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr) const
{
	CQuadrangle* resultQuadranglePtr = dynamic_cast<CQuadrangle*>(&result);
	if (resultQuadranglePtr == NULL){
		return false;
	}

	static ChangeSet changeSet(CF_OBJECT_POSITION, CF_ALL_DATA);
	istd::CChangeNotifier notifier(resultQuadranglePtr, changeSet);

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		bool retVal =
					m_firstDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode, &errorFactor1) &&
					m_secondDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;

		return retVal;
	}
	else{
		return		m_firstDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode) &&
					m_secondDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode);
	}
}


// reimplemented (istd::IChangeable)

int CQuadrangle::GetSupportedOperations() const
{
	return SO_COPY | SO_CLONE;
}


bool CQuadrangle::CopyFrom(const IChangeable& object, CompatibilityMode mode)
{
	const CQuadrangle* quadranglesPtr = dynamic_cast<const CQuadrangle*>(&object);

	if (quadranglesPtr != NULL){
		istd::CChangeNotifier notifier(this);
		
		SetFirstDiagonal(quadranglesPtr->GetFirstDiagonal());
		SetSecondDiagonal(quadranglesPtr->GetSecondDiagonal());

		CObject2dBase::CopyFrom(object, mode);

		return true;
	}

	return false;
}


istd::IChangeable* CQuadrangle::CloneMe(CompatibilityMode mode) const 
{
	istd::TDelPtr<CQuadrangle> clonePtr(new CQuadrangle);

	if (clonePtr->CopyFrom(*this, mode)){
		return clonePtr.PopPtr();
	}

	return NULL;
}


// reimplemented (iser::ISerializable)

bool CQuadrangle::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag firstDiagonalTag("FirstDiagonal", "FirstDiagonal");
	static iser::CArchiveTag secondDiagonalTag("SecondDiagonal", "SecondDiagonal");

	static ChangeSet changeSet(CF_OBJECT_POSITION, CF_ALL_DATA);
	istd::CChangeNotifier notifier(archive.IsStoring()? NULL: this, changeSet);
	Q_UNUSED(notifier);

	bool retVal = archive.BeginTag(firstDiagonalTag);
	retVal = retVal && m_firstDiagonal.Serialize(archive);
	retVal = retVal && archive.EndTag(firstDiagonalTag);

	retVal = retVal && archive.BeginTag(secondDiagonalTag);
	retVal = retVal && m_secondDiagonal.Serialize(archive);
	retVal = retVal && archive.EndTag(secondDiagonalTag);
	
	return retVal;
}

	
} // namespace i2d


