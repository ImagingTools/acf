#include "i2d/CQuadrangle.h"


#include "istd/TChangeNotifier.h"

#include "i2d/CVector2d.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"


namespace i2d
{


// public methods

CQuadrangle::CQuadrangle()
{
}


CQuadrangle::CQuadrangle(const CQuadrangle& quadrangle)
:	m_firstDiagonal(quadrangle.m_firstDiagonal),
	m_secondDiagonal(quadrangle.m_secondDiagonal)
{
}


CQuadrangle::CQuadrangle(const CLine2d& firstDiagonal, const CLine2d& secondDiagonal)
:	m_firstDiagonal(firstDiagonal),
	m_secondDiagonal(secondDiagonal)
{
}


CQuadrangle& CQuadrangle::operator = (const CQuadrangle& quadrangle)
{
	istd::CChangeNotifier changePtr(this);

	m_firstDiagonal = quadrangle.m_firstDiagonal;
	m_secondDiagonal = quadrangle.m_secondDiagonal;

	return *this;
}


bool CQuadrangle::operator == (const CQuadrangle& quadrangle) const
{
	return ((m_firstDiagonal == quadrangle.m_firstDiagonal) && (m_secondDiagonal == quadrangle.m_secondDiagonal));
}


bool CQuadrangle::operator != (const CQuadrangle& quadrangle) const
{
	return !operator == (quadrangle);
}


bool CQuadrangle::IsQuadrangleValid() const
{
	return m_firstDiagonal.IsIntersectedBy(m_secondDiagonal);
}


bool CQuadrangle::IsQuadrangleEmpty() const
{
	return m_firstDiagonal == m_secondDiagonal || m_firstDiagonal.IsNull() || m_secondDiagonal.IsNull();
}


CRectangle CQuadrangle::GetBoundingBox() const
{
	return m_firstDiagonal.GetBoundingBox().GetUnion(m_secondDiagonal.GetBoundingBox());
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


// reimplemented (IObject2d)

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
		istd::CChangeNotifier notifier(this, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		m_firstDiagonal.MoveCenterTo(delta + m_firstDiagonal.GetCenter());
		m_secondDiagonal.MoveCenterTo(delta + m_secondDiagonal.GetCenter());
	}
}


bool CQuadrangle::Transform(
			const ITransformation2d& transformation,
			ITransformation2d::ExactnessMode mode,
			double* errorFactorPtr)
{
	istd::CChangeNotifier notifier(this, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

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
	istd::CChangeNotifier notifier(this, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

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
		return		m_firstDiagonal.InvTransform(transformation, mode) &&
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

	istd::CChangeNotifier notifier(resultQuadranglePtr, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

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
		return		m_firstDiagonal.GetTransformed(transformation, resultQuadranglePtr->m_firstDiagonal, mode) &&
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

	istd::CChangeNotifier notifier(resultQuadranglePtr, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

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


// reimplemented (iser::ISerializable)

bool CQuadrangle::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag firstDiagonalTag("FirstDiagonal", "FirstDiagonal");
	bool retVal = archive.BeginTag(firstDiagonalTag);
	retVal = retVal && m_firstDiagonal.Serialize(archive);
	retVal = retVal && archive.EndTag(firstDiagonalTag);

	static iser::CArchiveTag secondDiagonalTag("SecondDiagonal", "SecondDiagonal");
	retVal = retVal && archive.BeginTag(secondDiagonalTag);
	retVal = retVal && m_secondDiagonal.Serialize(archive);
	retVal = retVal && archive.EndTag(secondDiagonalTag);
	
	return retVal;
}

	
} // namespace i2d


