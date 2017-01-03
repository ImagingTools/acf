#ifndef i3d_CMatrix3d_included
#define i3d_CMatrix3d_included


// ACF includes
#include <imath/TMatrix.h>
#include <i3d/CVector3d.h>


namespace i3d
{


/**
	3D-matrix definition.
*/
class CMatrix3d: public imath::TMatrix<3, 3>
{
public:
	typedef imath::TMatrix<3, 3> BaseClass;

	/**
		Constructor with no member initialization.
	*/
	CMatrix3d();
	CMatrix3d(const CMatrix3d& transform);
	/**
		Constructor using 3 vectors to span the transformation space.
	*/
	CMatrix3d(	const CVector3d& axisX,
				const CVector3d& axisY,
				const CVector3d& axisZ);
	/**
		Constructor using matrix members.
	*/
	CMatrix3d(	double m11, double m12, double m13,
				double m21, double m22, double m23,
				double m31, double m32, double m33);

	/**
		Default reset to identity.
	*/
	void Reset();

	// operations
	using BaseClass::GetMultiplied;
	CVector3d GetMultiplied(const CVector3d& position) const;
	CMatrix3d GetMultiplied(const CMatrix3d& matrix) const;
	void Multiply(const CMatrix3d& matrix);
	void MultiplyLeft(const CMatrix3d& matrix);

	/**
		Get axis X vector.
	*/
	CVector3d GetAxisX() const;

	/**
		Get axis Y vector.
	*/
	CVector3d GetAxisY() const;

	/**
		Get axis Z vector.
	*/
	CVector3d GetAxisZ() const;

	/**
		Get lengths of axes vectors.
	*/
	CVector3d GetAxesLengths() const;

	/**
		Get lengths of axes vectors.
	*/
	void GetAxesLengths(CVector3d& result) const;

	/**
		Inverted operation to GetApply().
	*/
	i3d::CVector3d GetInvMultiplied(const i3d::CVector3d& position) const;

	/**
		Inverted operation to GetApply().
	*/
	bool GetInvMultiplied(const i3d::CVector3d& position, i3d::CVector3d& result) const;

	/**
		Calculate inverted matrix.
	*/
	CMatrix3d GetInverted() const;

	/**
		Calculate inverted matrix.
	*/
	bool GetInverted(CMatrix3d& result) const;

	using BaseClass::GetTransposed;

	/**
		Calculate transposed matrix.
	*/
	CMatrix3d GetTransposed() const;

	/**
		Calculate determinant of deformation matrix.
	*/
	double GetDet() const;

	/**
		Copy operator.
	*/
	CMatrix3d& operator=(const CMatrix3d& matrix);

	/**
		Multiplication by scalar number.
	*/
	CMatrix3d operator*(double scale) const;
	/**
		Division by scalar number.
	*/
	CMatrix3d operator/(double scale) const;

	// static methods
	static const CMatrix3d& GetIdentity();

private:
	// static members
	static CMatrix3d s_identity;
};


// inline methods

inline CMatrix3d::CMatrix3d()
{
}


inline CMatrix3d::CMatrix3d(const CMatrix3d& matrix)
:	BaseClass(matrix)
{
}


inline CMatrix3d::CMatrix3d(const CVector3d& axisX,
							const CVector3d& axisY,
							const CVector3d& axisZ)
{
	SetAt(0, 0, axisX[0]);
	SetAt(0, 1, axisX[1]);
	SetAt(0, 2, axisX[2]);
	SetAt(1, 0, axisY[0]);
	SetAt(1, 1, axisY[1]);
	SetAt(1, 2, axisY[2]);
	SetAt(2, 0, axisZ[0]);
	SetAt(2, 1, axisZ[1]);
	SetAt(2, 2, axisZ[2]);
}


inline CMatrix3d::CMatrix3d(double m11, double m12, double m13,
							double m21, double m22, double m23,
							double m31, double m32, double m33)
{
	SetAt(0, 0, m11);
	SetAt(0, 1, m12);
	SetAt(0, 2, m13);
	SetAt(1, 0, m21);
	SetAt(1, 1, m22);
	SetAt(1, 2, m23);
	SetAt(2, 0, m31);
	SetAt(2, 1, m32);
	SetAt(2, 2, m33);
}


inline CVector3d CMatrix3d::GetMultiplied(const CVector3d& position) const
{
	CVector3d retVal;

	GetMultiplied(position, retVal);

	return retVal;
}


inline CMatrix3d CMatrix3d::GetMultiplied(const CMatrix3d& matrix) const
{
	CMatrix3d retVal;

	GetMultiplied(matrix, retVal);

	return retVal;
}


inline CVector3d CMatrix3d::GetAxisX() const
{
	return CVector3d(GetAt(0, 0), GetAt(0, 1), GetAt(0, 2));
}


inline CVector3d CMatrix3d::GetAxisY() const
{
	return CVector3d(GetAt(1, 0), GetAt(1, 1), GetAt(1, 2));
}

inline CVector3d CMatrix3d::GetAxisZ() const
{
	return CVector3d(GetAt(2, 0), GetAt(2, 1), GetAt(2, 2));
}


inline CVector3d CMatrix3d::GetAxesLengths() const
{
	CVector3d result;

	GetAxesLengths(result);
	
	return result;
}


inline void CMatrix3d::GetAxesLengths(CVector3d& result) const
{
	result.SetX(GetAxisX().GetLength());
	result.SetY(GetAxisY().GetLength());
	result.SetZ(GetAxisZ().GetLength());
}


inline CMatrix3d CMatrix3d::GetInverted() const
{
	CMatrix3d result;

	GetInverted(result);

	return result;
}


inline CMatrix3d CMatrix3d::GetTransposed() const
{
	return	CMatrix3d(	GetAt(0, 0), GetAt(0, 1), GetAt(0, 2),
						GetAt(1, 0), GetAt(1, 1), GetAt(1, 2),
						GetAt(2, 0), GetAt(2, 1), GetAt(2, 2));
}


inline double CMatrix3d::GetDet() const
{
	return	(GetAt(0, 0) * GetAt(1, 1) * GetAt(2, 2)) + 
			(GetAt(0, 1) * GetAt(1, 2) * GetAt(2, 0)) +
			(GetAt(0, 2) * GetAt(1, 0) * GetAt(2, 1)) -
			(GetAt(0, 0) * GetAt(1, 2) * GetAt(2, 1)) -
			(GetAt(0, 1) * GetAt(1, 0) * GetAt(2, 2)) -
			(GetAt(0, 2) * GetAt(1, 1) * GetAt(2, 0));
}


// operators

inline CMatrix3d& CMatrix3d::operator=(const CMatrix3d& matrix)
{
	BaseClass::operator=(matrix);

	return *this;
}


inline CMatrix3d CMatrix3d::operator*(double scale) const
{
	CMatrix3d retVal;

	GetScaled(scale, retVal);

	return retVal;
}


inline CMatrix3d CMatrix3d::operator/(double scale) const
{
	return CMatrix3d(	GetAt(0, 0) / scale,
						GetAt(0, 1) / scale,
						GetAt(0, 2) / scale,
						GetAt(1, 0) / scale,
						GetAt(1, 1) / scale,
						GetAt(1, 2) / scale,
						GetAt(2, 0) / scale,
						GetAt(2, 1) / scale,
						GetAt(2, 2) / scale);
}


// static methods

inline const CMatrix3d& CMatrix3d::GetIdentity()
{
	return s_identity;
}


} // namespace i3d


#endif // !i3d_CMatrix3d_included

