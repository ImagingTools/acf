#include <i2d/CMatrix2d.h>


namespace i2d
{


void CMatrix2d::Reset()
{
	*this = GetIdentity();
}


void CMatrix2d::Reset(double angle, double scale)
{
	double scaledSinus = qSin(angle) * scale;
	double scaledCosinus = qCos(angle) * scale;

	SetAt(0, 0, scaledCosinus);
	SetAt(0, 1, scaledSinus);
	SetAt(1, 0, -scaledSinus);
	SetAt(1, 1, scaledCosinus);
}


void CMatrix2d::Reset(double angle, const CVector2d& scale)
{
	double sinus = qSin(angle);
	double cosinus = qCos(angle);

	SetAt(0, 0, cosinus * scale[0]);
	SetAt(0, 1, sinus * scale[0]);
	SetAt(1, 0, -sinus * scale[1]);
	SetAt(1, 1, cosinus * scale[1]);
}


void CMatrix2d::Multiply(const CMatrix2d& matrix)
{
	CMatrix2d retVal;

	GetMultiplied(matrix, retVal);

	operator=(retVal);
}


void CMatrix2d::MultiplyLeft(const CMatrix2d& matrix)
{
	CMatrix2d retVal;

	matrix.GetMultiplied(*this, retVal);

	operator=(retVal);
}


CVector2d CMatrix2d::GetInvMultiplied(const CVector2d& position) const
{
	CVector2d retVal;

	GetInvMultiplied(position, retVal);

	return retVal;
}


double CMatrix2d::GetApproxAngle() const
{
	return GetAxisX().GetAngle();
}


double CMatrix2d::GetApproxScale() const
{
	static const double sqrt2 = qSqrt(2.0);

	return GetFrobeniusNorm() / sqrt2;
}


bool CMatrix2d::GetInvMultiplied(const CVector2d& position, CVector2d& result) const
{
	double det = GetDet();

	if (qAbs(det) > I_BIG_EPSILON){
		result.SetX((GetAt(1, 1) * position.GetX() - GetAt(1, 0) * position.GetY()) / det);
		result.SetY((GetAt(0, 0) * position.GetY() - GetAt(0, 1) * position.GetX()) / det);

		return true;
	}
	else{
		return false;
	}
}


bool CMatrix2d::GetInverted(CMatrix2d& result) const
{
	double det = GetDet();
	if (qAbs(det) < I_BIG_EPSILON){
		return false;
	}

	result.SetAt(0, 0, GetAt(1, 1) / det);
	result.SetAt(0, 1, -GetAt(0, 1) / det);
	result.SetAt(1, 0, -GetAt(1, 0) / det);
	result.SetAt(1, 1, GetAt(0, 0) / det);

	return true;
}


bool CMatrix2d::GetEigenVectors(i2d::CVector2d& vector1, i2d::CVector2d& vector2, double& value1, double& value2) const
{
	double trace = GetTrace();
	double det = GetDet();

	double delta = trace * trace - 4 * det;
	if (delta >= 0){
		double deltaSqrt = qSqrt(delta);

		value1 = (trace + deltaSqrt) * 0.5;
		value2 = (trace - deltaSqrt) * 0.5;

		double m00 = GetAt(0, 0);
		double m10 = GetAt(1, 0);
		double m01 = GetAt(0, 1);
		double m11 = GetAt(1, 1);

		if (qFabs(m01) > I_BIG_EPSILON){
			vector1 = CVector2d(value1 - m11, m01);
			vector2 = CVector2d(value2 - m11, m01);
		}
		else if (qFabs(m10) > I_BIG_EPSILON){
			vector1 = CVector2d(m10, value1 - m00);
			vector2 = CVector2d(m10, value2 - m00);
		}
		else{
			vector1 = CVector2d(1, 0);
			vector2 = CVector2d(0, 1);
		}

		return true;
	}

	return false;
}


// static members

CMatrix2d CMatrix2d::s_identity(1.0, 0.0, 0.0, 1.0);


} // namespace i2d


