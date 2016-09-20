#ifndef i2d_CParallelogram_included
#define i2d_CParallelogram_included


// ACF includes
#include <iser/CArchiveTag.h>


// ACF includes
#include <i2d/CObject2dBase.h>
#include <i2d/CAffine2d.h>
#include <i2d/CRectangle.h>


namespace i2d
{


/**
	This class defines parallelogram.
	Internal is this parallelogram represents as 2D linear transformation (i2d::ITransformation2d),
	but it is interpreted as a geometrical shape which is done,
	if you transform unity quadrant [0, 1]X[0, 1] using this transformation.
*/
class CParallelogram: public CObject2dBase
{
public:
	virtual const i2d::CAffine2d& GetTransform() const;
	virtual void SetTransform(const i2d::CAffine2d& transform);
	virtual const i2d::CMatrix2d& GetDeformMatrix() const;
	virtual void SetDeformMatrix(const i2d::CMatrix2d& deform);

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const;
	virtual void MoveCenterTo(const i2d::CVector2d& position);
	virtual i2d::CRectangle GetBoundingBox() const;
	virtual bool Transform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	i2d::CAffine2d m_transform;
};


// inline methods

inline const i2d::CAffine2d& CParallelogram::GetTransform() const
{
	return m_transform;
}


inline void CParallelogram::SetTransform(const i2d::CAffine2d& transform)
{
	m_transform = transform;
}



inline const i2d::CMatrix2d& CParallelogram::GetDeformMatrix() const
{
	return m_transform.GetDeformMatrix();
}


inline void CParallelogram::SetDeformMatrix(const i2d::CMatrix2d& deform)
{
	m_transform.SetDeformMatrix(deform);
}


} // namespace i2d


#endif // !i2d_CParallelogram_included


