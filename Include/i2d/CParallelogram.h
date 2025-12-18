#ifndef i2d_CParallelogram_included
#define i2d_CParallelogram_included



// Qt includes
#include <QtCore/QVector>

// ACF includes
#include <iser/CArchiveTag.h>
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
	CParallelogram();

	virtual const i2d::CAffine2d& GetTransform() const;
	virtual void SetTransform(const i2d::CAffine2d& transform);
	virtual const i2d::CMatrix2d& GetDeformMatrix() const;
	virtual void SetDeformMatrix(const i2d::CMatrix2d& deform);

	double GetWidth() const;
	void SetWidth(double w);
	double GetHeight() const;
	void SetHeight(double h);
	QVector<i2d::CVector2d> GetNodes() const;
	double GetRotation() const;
	void SetRotation(double angle);
	i2d::CVector2d GetTranslation() const;
	void SetTranslation(const i2d::CVector2d& translation);

	// reimplemented (i2d::IObject2d)
	virtual i2d::CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const i2d::CVector2d& position) override;
	virtual i2d::CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const i2d::ITransformation2d& transformation,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const i2d::ITransformation2d& transformation,
				i2d::IObject2d& result,
				i2d::ITransformation2d::ExactnessMode mode = i2d::ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	virtual i2d::CVector2d GetCenter(const i2d::CMatrix2d& deformMatrix) const;

private:
	i2d::CAffine2d m_transform;
};


// inline methods

inline const i2d::CAffine2d& CParallelogram::GetTransform() const
{
	return m_transform;
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


