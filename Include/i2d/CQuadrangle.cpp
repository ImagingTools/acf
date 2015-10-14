#include "i2d/CQuadrangle.h"


// ACF includes
#include "istd/TDelPtr.h"
#include "istd/CChangeNotifier.h"
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"
#include "i2d/CVector2d.h"


namespace i2d
{


static const istd::IChangeable::ChangeSet s_setDiagonalChange(CQuadrangle::CF_OBJECT_POSITION, QObject::tr("Set diagnal"));


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
		istd::CChangeNotifier changeNotifier(this, &s_setDiagonalChange);
		Q_UNUSED(changeNotifier);
		
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
		istd::CChangeNotifier changeNotifier(this, &s_setDiagonalChange);
		Q_UNUSED(changeNotifier);
		
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
		istd::CChangeNotifier changeNotifier(this, &s_objectMovedChange);
		Q_UNUSED(changeNotifier);

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
	istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
	Q_UNUSED(changeNotifier);

	bool retVal = false;

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		retVal = m_firstDiagonal.Transform(transformation, mode, &errorFactor1) && m_secondDiagonal.Transform(transformation, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;	   
	}
	else{	   
		retVal = m_firstDiagonal.Transform(transformation, mode) && m_secondDiagonal.Transform(transformation, mode);
	}

	return retVal;
}


bool CQuadrangle::InvTransform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
	Q_UNUSED(changeNotifier);

	bool retVal = false;

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		retVal = m_firstDiagonal.InvTransform(transformation, mode, &errorFactor1) && m_secondDiagonal.InvTransform(transformation, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;

	}
	else{
		retVal = m_firstDiagonal.InvTransform(transformation, mode) && m_secondDiagonal.InvTransform(transformation, mode);
	}

	return retVal;
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

	istd::CChangeNotifier changeNotifier(resultQuadranglePtr, &s_objectModifiedChange);
	Q_UNUSED(changeNotifier);

	bool retVal = false;

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		retVal =
					m_firstDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode, &errorFactor1) &&
					m_secondDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;
	}
	else{
		retVal =	m_firstDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode) &&
				m_secondDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode);
	}

	 return retVal;
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

	istd::CChangeNotifier changeNotifier(resultQuadranglePtr, &s_objectModifiedChange);
	Q_UNUSED(changeNotifier);

	bool retVal = false;

	if (errorFactorPtr != NULL){
		double errorFactor1 = 0;
		double errorFactor2 = 0;
		retVal =
					m_firstDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode, &errorFactor1) &&
					m_secondDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode, &errorFactor2);

		*errorFactorPtr = errorFactor1 + errorFactor2;
	}
	else{
		retVal =		m_firstDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode) &&
					m_secondDiagonal.GetInvTransformed(transformation, resultQuadranglePtr->m_secondDiagonal, mode);
	}

	return retVal;
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
		istd::CChangeNotifier changeNotifier(this, &s_objectModifiedChange);
		Q_UNUSED(changeNotifier);
		
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
	static iser::CArchiveTag firstDiagonalTag("FirstDiagonal", "FirstDiagonal", iser::CArchiveTag::TT_GROUP);
	static iser::CArchiveTag secondDiagonalTag("SecondDiagonal", "SecondDiagonal", iser::CArchiveTag::TT_GROUP);

	istd::CChangeNotifier changeNotifier(this, &GetAllChanges());
	Q_UNUSED(changeNotifier);

	bool retVal = archive.BeginTag(firstDiagonalTag);
	retVal = retVal && m_firstDiagonal.Serialize(archive);
	retVal = retVal && archive.EndTag(firstDiagonalTag);

	retVal = retVal && archive.BeginTag(secondDiagonalTag);
	retVal = retVal && m_secondDiagonal.Serialize(archive);
	retVal = retVal && archive.EndTag(secondDiagonalTag);
	
	return retVal;
}

	
} // namespace i2d


