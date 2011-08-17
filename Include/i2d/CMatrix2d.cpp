#include "i2d/CMatrix2d.h"


namespace i2d
{


void CMatrix2d::Reset(double angle, double scale)
{
	double scaledSinus = std::sin(angle) * scale;
	double scaledCosinus = std::cos(angle) * scale;

	SetAt(0, 0, scaledCosinus);
	SetAt(0, 1, scaledSinus);
	SetAt(1, 0, -scaledSinus);
	SetAt(1, 1, scaledCosinus);
}


void CMatrix2d::Reset(double angle, const CVector2d& scale)
{
	double sinus = std::sin(angle);
	double cosinus = std::cos(angle);

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
	static const double sqrt2 = std::sqrt(2.0);

	return GetFrobeniusNorm() / sqrt2;
}


bool CMatrix2d::GetInvMultiplied(const CVector2d& position, CVector2d& result) const
{
	double det = GetDet();

	if (std::fabs(det) > I_BIG_EPSILON){
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
	if (std::fabs(det) < I_BIG_EPSILON){
		return false;
	}

	result.SetAt(0, 0, GetAt(1, 1) / det);
	result.SetAt(0, 1, GetAt(0, 1) / det);
	result.SetAt(1, 0, GetAt(1, 0) / det);
	result.SetAt(1, 1, GetAt(0, 0) / det);

	return true;
}


bool CMatrix2d::GetDecompositionQDQ(CMatrix2d& matrixQ, CVector2d& diagonalD)
{
	CMatrix2d matrixR = *this;
	matrixQ.Reset();

	int i;
	for (i = 0; i < 100; ++i){
		CMatrix2d tempMatrixR;
		CMatrix2d tempMatrixQ;
		if (!matrixR.GetTriangleDecomposed(tempMatrixR, &tempMatrixQ)){
			return false;
		}
		matrixR = tempMatrixR.GetMultiplied(tempMatrixQ);

		matrixQ = tempMatrixQ.GetMultiplied(matrixQ);
	}

	for (i = 0; i < 2; ++i){
		diagonalD[i] = matrixR.GetAt(i, i);
	}

	return true;
}


// static members

CMatrix2d CMatrix2d::s_identity(1.0, 0.0, 0.0, 1.0);


} // namespace i2d



