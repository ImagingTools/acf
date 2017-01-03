#ifndef i2d_CMatrix2d_included
#define i2d_CMatrix2d_included


// ACF includes
#include <imath/TMatrix.h>

#include <i2d/CVector2d.h>


namespace i2d
{


/**
	2D matrix.
*/
class CMatrix2d: public imath::TMatrix<2, 2>
{
public:
	typedef imath::TMatrix<2, 2> BaseClass;

	/**
		Constructor with no member initialization.
	*/
	CMatrix2d();
	CMatrix2d(const CMatrix2d& transform);
	/**
		Constructor using 2 vectors to span the transformation space.
	*/
	CMatrix2d(const CVector2d& axisX, const CVector2d& axisY);
	/**
		Constructor using matrix members.
	*/
	CMatrix2d(double m11, double m12, double m21, double m22);

	/**
		Default reset to identity.
	*/
	void Reset();
	/**
		Set the matrix using specified angle (in radians) and uniform scale.
	*/
	void Reset(double angle, double scale = 1.0);
	/**
		Set the deformation matrix using specified angle (in radians) and scale determined for both axis separately.
	*/
	void Reset(double angle, const CVector2d& scale);

	// operations
	using BaseClass::GetMultiplied;
	CVector2d GetMultiplied(const CVector2d& position) const;
	CMatrix2d GetMultiplied(const CMatrix2d& matrix) const;
	void Multiply(const CMatrix2d& matrix);
	void MultiplyLeft(const CMatrix2d& matrix);

	/**
		Get axis X vector.
	*/
	CVector2d GetAxisX() const;
	/**
		Get axis Y vector.
	*/
	CVector2d GetAxisY() const;

	/**
		Get lengths of axes vectors.
	*/
	CVector2d GetAxesLengths() const;
	/**
		Get lengths of axes vectors.
	*/
	void GetAxesLengths(CVector2d& result) const;
	/**
		Get angle of axis X in radians.
		Please note that this angle is in radians
	*/
	double GetApproxAngle() const;
	/**
		Get approximated scale.
	*/
	double GetApproxScale() const;

	/**
		Inverted operation to GetApply().
	*/
	i2d::CVector2d GetInvMultiplied(const i2d::CVector2d& position) const;
	/**
		Inverted operation to GetApply().
	*/
	bool GetInvMultiplied(const i2d::CVector2d& position, i2d::CVector2d& result) const;

	/**
		Calculate inverted matrix.
	*/
	CMatrix2d GetInverted() const;
	/**
		Calculate inverted matrix.
	*/
	bool GetInverted(CMatrix2d& result) const;

	using BaseClass::GetTransposed;
	/**
		Calculate transposed matrix.
	*/
	CMatrix2d GetTransposed() const;

	/**
		Calculate eigen vectors and eigen values.
		The first eigen value is always bigger or equal than the second one.
	*/
	bool GetEigenVectors(i2d::CVector2d& vector1, i2d::CVector2d& vector2, double& value1, double& value2) const;

	/**
		Calculate determinant of deformation matrix.
	*/
	double GetDet() const;

	// operators
	/**
		Copy operator.
	*/
	CMatrix2d& operator=(const CMatrix2d& matrix);

	CMatrix2d operator+(const BaseClass& matrix) const;
	CMatrix2d operator-(const BaseClass& matrix) const;
	CMatrix2d operator-();
	/**
		Multiplication by scalar number.
	*/
	CMatrix2d operator*(double scale) const;
	/**
		Division by scalar number.
	*/
	CMatrix2d operator/(double scale) const;

	// static methods
	static const CMatrix2d& GetIdentity();

private:
	// static members
	static CMatrix2d s_identity;
};


// inline methods

inline CMatrix2d::CMatrix2d()
{
}


inline CMatrix2d::CMatrix2d(const CMatrix2d& matrix)
:	BaseClass(matrix)
{
}


inline CMatrix2d::CMatrix2d(const CVector2d& axisX, const CVector2d& axisY)
{
	SetAt(0, 0, axisX[0]);
	SetAt(0, 1, axisX[1]);
	SetAt(1, 0, axisY[0]);
	SetAt(1, 1, axisY[1]);
}


inline CMatrix2d::CMatrix2d(double m11, double m12, double m21, double m22)
{
	SetAt(0, 0, m11);
	SetAt(0, 1, m12);
	SetAt(1, 0, m21);
	SetAt(1, 1, m22);
}


inline CVector2d CMatrix2d::GetMultiplied(const CVector2d& position) const
{
	CVector2d retVal;

	GetMultiplied(position, retVal);

	return retVal;
}


inline CMatrix2d CMatrix2d::GetMultiplied(const CMatrix2d& matrix) const
{
	CMatrix2d retVal;

	GetMultiplied(matrix, retVal);

	return retVal;
}


inline CVector2d CMatrix2d::GetAxisX() const
{
	return CVector2d(GetAt(0, 0), GetAt(0, 1));
}


inline CVector2d CMatrix2d::GetAxisY() const
{
	return CVector2d(GetAt(1, 0), GetAt(1, 1));
}


inline CVector2d CMatrix2d::GetAxesLengths() const
{
	CVector2d result;

	GetAxesLengths(result);
	
	return result;
}


inline void CMatrix2d::GetAxesLengths(CVector2d& result) const
{
	result.SetX(GetAxisX().GetLength());
	result.SetY(GetAxisY().GetLength());
}


inline CMatrix2d CMatrix2d::GetInverted() const
{
	CMatrix2d result;

	GetInverted(result);

	return result;
}


inline CMatrix2d CMatrix2d::GetTransposed() const
{
	return CMatrix2d(GetAt(0, 0), GetAt(0, 1), GetAt(1, 0), GetAt(1, 1));
}


inline double CMatrix2d::GetDet() const
{
	return GetAt(0, 0) * GetAt(1, 1) - GetAt(0, 1) * GetAt(1, 0);
}


// operators

inline CMatrix2d CMatrix2d::operator+(const BaseClass& matrix) const
{
	CMatrix2d retVal;

	BaseClass::GetAdded(matrix, retVal);

	return retVal;
}


inline CMatrix2d CMatrix2d::operator-(const BaseClass& matrix) const
{
	CMatrix2d retVal;

	BaseClass::GetSubstracted(matrix, retVal);

	return retVal;
}


inline CMatrix2d CMatrix2d::operator-()
{
	CMatrix2d retVal;

	BaseClass::GetNegated(retVal);

	return retVal;
}


inline CMatrix2d& CMatrix2d::operator=(const CMatrix2d& matrix)
{
	BaseClass::operator=(matrix);

	return *this;
}


inline CMatrix2d CMatrix2d::operator*(double scale) const
{
	CMatrix2d retVal;

	GetScaled(scale, retVal);

	return retVal;
}


inline CMatrix2d CMatrix2d::operator/(double scale) const
{
	return CMatrix2d(GetAt(0, 0) / scale, GetAt(0, 1) / scale, GetAt(1, 0) / scale, GetAt(1, 1) / scale);
}


// static methods

inline const CMatrix2d& CMatrix2d::GetIdentity()
{
	return s_identity;
}


} // namespace i2d


#endif // !i2d_CMatrix2d_included


