#include <i2d/CAffine2d.h>

// std includes
#include <cmath>


namespace i2d
{


CAffine2d::CAffine2d(const CMatrix2d& deform, const CVector2d& translation)
:	m_translation(translation),
	m_deformMatrix(deform)
{
}


CAffine2d::CAffine2d(const CVector2d& translation)
:	m_translation(translation),
	m_deformMatrix(CMatrix2d::GetIdentity())
{
}

CVector2d CAffine2d::GetScaleVector() const
{
	const CMatrix2d& D = GetDeformMatrix();
	const double s1 = std::sqrt(D.GetAt(0, 0) * D.GetAt(0, 0) + D.GetAt(0, 1) * D.GetAt(0, 1));
	const double s2 = std::sqrt(D.GetAt(1, 0) * D.GetAt(1, 0) + D.GetAt(1, 1) * D.GetAt(1, 1));

	return CVector2d(s1, s2);
}

CMatrix2d CAffine2d::GetRotationMatrix() const
{
	CMatrix2d result = GetDeformMatrix();
	const CVector2d& scale = GetScaleVector();

	result.GetAtRef(0, 0) = result.GetAt(0, 0) / scale.GetX();
	result.GetAtRef(0, 1) = result.GetAt(0, 1) / scale.GetX();
	result.GetAtRef(1, 0) = result.GetAt(1, 0) / scale.GetY();
	result.GetAtRef(1, 1) = result.GetAt(1, 1) / scale.GetY();

	return result;
}

// init operations

void CAffine2d::Reset()
{
	m_deformMatrix.Reset();
	m_translation.Reset();
}


void CAffine2d::Reset(const CVector2d& translation)
{
	m_deformMatrix.Reset();
	m_translation = translation;
}


void CAffine2d::Reset(const CVector2d& translation, double angle, double scale)
{
	double sinus = qSin(angle);
	double cosinus = qCos(angle);

	m_deformMatrix.SetAt(0, 0, cosinus * scale);
	m_deformMatrix.SetAt(0, 1, sinus * scale);
	m_deformMatrix.SetAt(1, 0, -sinus * scale);
	m_deformMatrix.SetAt(1, 1, cosinus * scale);

	m_translation = translation;
}


void CAffine2d::Reset(const CVector2d& translation, double angle, const CVector2d& scale)
{
	double sinus = qSin(angle);
	double cosinus = qCos(angle);

	m_deformMatrix.SetAt(0, 0, cosinus * scale[0]);
	m_deformMatrix.SetAt(0, 1, sinus * scale[0]);
	m_deformMatrix.SetAt(1, 0, -sinus * scale[1]);
	m_deformMatrix.SetAt(1, 1, cosinus * scale[1]);

	m_translation = translation;
}


CAffine2d CAffine2d::GetApply(const CAffine2d& transform) const
{
	CAffine2d result;
	CMatrix2d& resultDeform = result.GetDeformMatrixRef();
	CVector2d& resultVector = result.GetTranslationRef();

	m_deformMatrix.GetMultiplied(transform.GetDeformMatrix(), resultDeform);
	m_deformMatrix.GetMultiplied(transform.GetTranslation(), resultVector);
	resultVector += m_translation;

	return result;
}


void CAffine2d::GetApply(const CAffine2d& transform, CAffine2d& result) const
{
	CMatrix2d& resultDeform = result.GetDeformMatrixRef();
	CVector2d& resultVector = result.GetTranslationRef();

	m_deformMatrix.GetMultiplied(transform.GetDeformMatrix(), resultDeform);
	m_deformMatrix.GetMultiplied(transform.GetTranslation(), resultVector);
	resultVector += m_translation;
}


void CAffine2d::Apply(const CAffine2d& transform)
{
	CVector2d delta;

	m_deformMatrix.GetMultiplied(transform.GetTranslation(), delta);

	CMatrix2d resultDeform;
	m_deformMatrix.GetMultiplied(transform.GetDeformMatrix(), resultDeform);

	m_translation += delta;
	m_deformMatrix = resultDeform;
}


void CAffine2d::ApplyLeft(const CAffine2d& transform)
{
	const CMatrix2d& deform = transform.GetDeformMatrix();
	m_translation = deform.GetMultiplied(m_translation) + transform.GetTranslation();

	m_deformMatrix = deform.GetMultiplied(m_deformMatrix);
}


bool CAffine2d::GetInvertedApply(const i2d::CVector2d& position, i2d::CVector2d& result) const
{
	return m_deformMatrix.GetInvMultiplied(position - m_translation, result);
}


bool CAffine2d::GetInverted(CAffine2d& result) const
{
	if (!m_deformMatrix.GetInverted(result.m_deformMatrix)){
		return false;
	}

	result.m_deformMatrix.GetMultiplied(-m_translation, result.m_translation);

	return true;
}


bool CAffine2d::Serialize(iser::IArchive& archive)
{
	bool retVal = true;
	retVal = retVal && m_deformMatrix.Serialize(archive);
	retVal = retVal && m_translation.Serialize(archive);

	return retVal;
}


// operators

CAffine2d& CAffine2d::operator=(const CAffine2d& transform)
{
	m_deformMatrix = transform.GetDeformMatrix();
	m_translation = transform.GetTranslation();

	return *this;
}


// static members

CAffine2d CAffine2d::s_identity(CMatrix2d(1.0, 0.0, 0.0, 1.0), CVector2d(0, 0));


} // namespace i2d
