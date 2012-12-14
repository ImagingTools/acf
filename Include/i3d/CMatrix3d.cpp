#include "i3d/CMatrix3d.h"


namespace i3d
{


void CMatrix3d::Reset()
{
	*this = GetIdentity();
}

void CMatrix3d::Multiply(const CMatrix3d& matrix)
{
	CMatrix3d retVal;

	GetMultiplied(matrix, retVal);

	operator=(retVal);
}


void CMatrix3d::MultiplyLeft(const CMatrix3d& matrix)
{
	CMatrix3d retVal;

	matrix.GetMultiplied(*this, retVal);

	operator=(retVal);
}


CVector3d CMatrix3d::GetInvMultiplied(const CVector3d& position) const
{
	CVector3d retVal;

	GetInvMultiplied(position, retVal);

	return retVal;
}

bool CMatrix3d::GetInvMultiplied(const CVector3d& position, CVector3d& result) const
{
	double det = GetDet();

	if (qAbs(det) > I_BIG_EPSILON){
		double x =	((GetAt(1, 1) * GetAt(2, 2) - GetAt(1, 2) * GetAt(2, 1)) * position.GetX()) +
					((GetAt(0, 2) * GetAt(2, 1) - GetAt(0, 1) * GetAt(2, 2)) * position.GetY()) +
					((GetAt(0, 1) * GetAt(1, 2) - GetAt(0, 2) * GetAt(1, 1)) * position.GetZ());

		result.SetX(x / det);


		double y =	((GetAt(1, 2) * GetAt(2, 0) - GetAt(1, 0) * GetAt(2, 2)) * position.GetX()) +
					((GetAt(0, 0) * GetAt(2, 2) - GetAt(0, 2) * GetAt(2, 0)) * position.GetY()) +
					((GetAt(0, 2) * GetAt(1, 0) - GetAt(0, 0) * GetAt(1, 2)) * position.GetZ());

		result.SetY(y / det);


		double z =	((GetAt(1, 0) * GetAt(2, 1) - GetAt(1, 1) * GetAt(2, 0)) * position.GetX()) +
					((GetAt(0, 1) * GetAt(2, 0) - GetAt(0, 0) * GetAt(2, 1)) * position.GetY()) +
					((GetAt(0, 0) * GetAt(1, 1) - GetAt(0, 1) * GetAt(1, 0)) * position.GetZ());

		result.SetZ(z / det);
		
		return true;
	}
	else{
		return false;
	}
}


bool CMatrix3d::GetInverted(CMatrix3d& result) const
{
	double det = GetDet();
	if (qAbs(det) < I_BIG_EPSILON){
		return false;
	}

	result.SetAt(0, 0, (GetAt(1, 1) * GetAt(2, 2) - GetAt(1, 2) * GetAt(2, 1)) / det);
	result.SetAt(0, 1, (GetAt(0, 2) * GetAt(2, 1) - GetAt(0, 1) * GetAt(2, 2)) / det);
	result.SetAt(0, 2, (GetAt(0, 1) * GetAt(1, 2) - GetAt(0, 2) * GetAt(1, 1)) / det);

	result.SetAt(1, 0, (GetAt(1, 2) * GetAt(2, 0) - GetAt(1, 0) * GetAt(2, 2)) / det);
	result.SetAt(1, 1, (GetAt(0, 0) * GetAt(2, 2) - GetAt(0, 2) * GetAt(2, 0)) / det);
	result.SetAt(1, 2, (GetAt(0, 2) * GetAt(1, 0) - GetAt(0, 0) * GetAt(1, 2)) / det);

	result.SetAt(2, 0, (GetAt(1, 0) * GetAt(2, 1) - GetAt(1, 1) * GetAt(2, 0)) / det);
	result.SetAt(2, 1, (GetAt(0, 1) * GetAt(2, 0) - GetAt(0, 0) * GetAt(2, 1)) / det);
	result.SetAt(2, 2, (GetAt(0, 0) * GetAt(1, 1) - GetAt(0, 1) * GetAt(1, 0)) / det);
	

	return true;
}


bool CMatrix3d::GetDecompositionQDQ(CMatrix3d& matrixQ, CVector3d& diagonalD)
{
	CMatrix3d matrixR = *this;
	matrixQ.Reset();

	int i;
	for (i = 0; i < 100; ++i){
		CMatrix3d tempMatrixR;
		CMatrix3d tempMatrixQ;
		if (!matrixR.GetTriangleDecomposed(tempMatrixR, &tempMatrixQ)){
			return false;
		}
		matrixR = tempMatrixR.GetMultiplied(tempMatrixQ);

		matrixQ = tempMatrixQ.GetMultiplied(matrixQ);
	}

	for (i = 0; i < 3; ++i){
		diagonalD[i] = matrixR.GetAt(i, i);
	}

	return true;
}


// static members

CMatrix3d CMatrix3d::s_identity(1.0, 0.0, 0.0,
								0.0, 1.0, 0.0,
								0.0, 0.0, 1.0);


} // namespace i3d



