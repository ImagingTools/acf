#ifndef i2d_CAffine2d_included
#define i2d_CAffine2d_included


// ACF includes
#include "i2d/CVector2d.h"
#include "i2d/CMatrix2d.h"


namespace i2d
{


/**
	This class defines an affinite 2D transformation.
*/
class CAffine2d
{
public:
	/**
		Constructor, parameters will be not initialized.
	*/
	CAffine2d();

	/**
		Copy constructor, transformation data will be copied from the input object.
	*/
	CAffine2d(const CAffine2d& transform);

	/**
		Constructs a transform using specified deformation matrix and translation.
	*/
	explicit CAffine2d(const CMatrix2d& deform, const CVector2d& translation = CVector2d(0.0, 0.0));

	/**
		Constructs a transform with translation.
	*/
	explicit CAffine2d(const CVector2d& translation);

	/**
		Get translation part of this affine transformation.
	*/
	const CVector2d& GetTranslation() const;

	/**
		Set translation part of this affine transformation.
	*/
	void SetTranslation(const CVector2d& translation);

	/**
		Get rotation, skew and scale part of this affine transformation.
	*/
	const CMatrix2d& GetDeformMatrix() const;

	/**
		Set rotation, skew and scale part of this affine transformation.
	*/
	void SetDeformMatrix(const CMatrix2d& deform);

	// internal access operations

	/**
		Get internal reference to the translation vector.
	*/
	CVector2d& GetTranslationRef();

	/**
		Get internal reference to the transformation matrix.
	*/
	CMatrix2d& GetDeformMatrixRef();

	// init operations

	/**
		Set to identity transformation.
	*/
	void Reset();

	/**
		Set this transformation to translation.
	*/
	void Reset(const CVector2d& translation);

	/**
		Set this transformation to a given translation, rotation and scaling.
	*/
	void Reset(const CVector2d& translation, double angle, double scale = 1.0);

	/**
		Set this transformation to a given translation, rotation and both axis scaling.
	*/
	void Reset(const CVector2d& translation, double angle, const CVector2d& scale);

	/**
		Calculate transformed position.
	*/
	CVector2d GetApply(const CVector2d& position) const;

	/**
		Get position after this transformation.
	*/
	void GetApply(const CVector2d& position, CVector2d& result) const;

	/**
		Get difference vector after this transformation.
		Please note, that the translation will be not concidered for this transformation.
	*/
	CVector2d GetApplyToDelta(const CVector2d& delta) const;

	/**
		Get vector after this transformation.
		Please note, that the translation will be not concidered for this transformation.
	*/
	void GetApplyToDelta(const CVector2d& delta, CVector2d& result) const;

	/**
		Get combined transformation.
		\param	transform	local transformation used on right side of transformation multiplication.
		\return	combined transform.
	*/
	CAffine2d GetApply(const CAffine2d& transform) const;

	/**
		Get combined transformation.
		\param	transform	local transformation used on right side of transformation multiplication.
		\return	combined transformation, it is equal to thisTranform * parameterTransform.
	*/
	void GetApply(const CAffine2d& transform, CAffine2d& result) const;

	/**
		Combine this transformation with another transformation.
		\param	transform	local transformation used on right side of transformation multiplication.
	*/
	void Apply(const CAffine2d& transform);

	/**
		Combine this transformation with another transformation on the left side.
		\param	transform	local transformation used on left side of transformation multiplication.
	*/
	void ApplyLeft(const CAffine2d& transform);

	/**
		Inverted operation to GetApply().
	*/
	CVector2d GetInvertedApply(const CVector2d& position) const;

	/**
		Inverted operation to GetApply().
	*/
	bool GetInvertedApply(const CVector2d& position, CVector2d& result) const;

	/**
		Get some transformation combined with translation.
	*/
	CAffine2d GetTranslated(const CVector2d& delta) const;

	/**
		Get some transformation combined with translation.
	*/
	void GetTranslated(const CVector2d& delta, CAffine2d& result) const;

	/**
		Combine this transformation with translation.
	*/
	void Translate(const CVector2d& delta);

	/**
		Get inverted transformation.
	*/
	CAffine2d GetInverted() const;

	/**
		Get inverted transformation.
	*/
	bool GetInverted(CAffine2d& result) const;

	/**
		Serialize transformation parameters into/from archive.
	*/
	bool Serialize(iser::IArchive& archive);

	// operators
	bool operator==(const CAffine2d& transform) const;
	bool operator!=(const CAffine2d& transform) const;
	CAffine2d& operator=(const CAffine2d& transform);

	// static methods
	static const CAffine2d& GetIdentity();

private:
	CVector2d m_translation;
	CMatrix2d m_deformMatrix;

	// static members
	static CAffine2d s_identity;
};


// inline methods

inline CAffine2d::CAffine2d()
{
}


inline CAffine2d::CAffine2d(const CAffine2d& transform)
:	m_translation(transform.m_translation),
	m_deformMatrix(transform.m_deformMatrix)
{
}


// access to members

inline const CVector2d& CAffine2d::GetTranslation() const
{
	return m_translation;
}


inline void CAffine2d::SetTranslation(const CVector2d& translation)
{
	m_translation = translation;
}


inline const CMatrix2d& CAffine2d::GetDeformMatrix() const
{
	return m_deformMatrix;
}


inline void CAffine2d::SetDeformMatrix(const CMatrix2d& deform)
{
	m_deformMatrix = deform;
}


// internal access operations

inline CVector2d& CAffine2d::GetTranslationRef()
{
	return m_translation;
}


inline CMatrix2d& CAffine2d::GetDeformMatrixRef()
{
	return m_deformMatrix;
}


// operations

inline CVector2d CAffine2d::GetApply(const CVector2d& position) const
{
	CVector2d result;
	m_deformMatrix.GetMultiplied(position, result);

	return result + m_translation;
}


inline void CAffine2d::GetApply(const CVector2d& position, CVector2d& result) const
{
	m_deformMatrix.GetMultiplied(position, result);
	result += m_translation;
}


inline CVector2d CAffine2d::GetApplyToDelta(const CVector2d& delta) const
{
	CVector2d retVal;

	m_deformMatrix.GetMultiplied(delta, retVal);

	return retVal;
}


inline void CAffine2d::GetApplyToDelta(const CVector2d& delta, CVector2d& result) const
{
	m_deformMatrix.GetMultiplied(delta, result);
}


inline CAffine2d CAffine2d::GetTranslated(const CVector2d& vector) const
{
	return CAffine2d(m_deformMatrix, m_translation + vector);
}


inline void CAffine2d::GetTranslated(const CVector2d& vector, CAffine2d& result) const
{
	result.GetDeformMatrixRef() = m_deformMatrix;
	result.GetTranslationRef() = m_translation + vector;
}


inline void CAffine2d::Translate(const CVector2d& vector)
{
	m_translation += vector;
}


inline CVector2d CAffine2d::GetInvertedApply(const CVector2d& position) const
{
	CVector2d result;

	GetInvertedApply(position, result);

	return result;
}


inline CAffine2d CAffine2d::GetInverted() const
{
	CAffine2d result;

	GetInverted(result);

	return result;
}


// operators

inline bool CAffine2d::operator==(const CAffine2d& transform) const
{
	return (m_translation == transform.m_translation) && (m_deformMatrix == transform.m_deformMatrix);
}


inline bool CAffine2d::operator!=(const CAffine2d& transform) const
{
	return (m_translation != transform.m_translation) || (m_deformMatrix != transform.m_deformMatrix);
}


// static methods

inline const CAffine2d& CAffine2d::GetIdentity()
{
	return s_identity;
}


} // namespace i2d


#endif // !i2d_CAffine2d_included


