#ifndef i2d_CQuadrangle_included
#define i2d_CQuadrangle_included


// ACF includes
#include <i2d/CObject2dBase.h>
#include <i2d/CLine2d.h>
#include <i2d/CRectangle.h>


namespace i2d
{


/**
	Definition of convexes quadrangle object.
*/
class CQuadrangle: public CObject2dBase
{
public:
	CQuadrangle();
	CQuadrangle(const CLine2d& firstDiagonal, const CLine2d& secondDiagonal);

	bool operator==(const CQuadrangle& quadrangle) const;
	bool operator!=(const CQuadrangle& quadrangle) const;

	bool IsQuadrangleValid() const;
	bool IsQuadrangleEmpty() const;

	const CLine2d& GetFirstDiagonal() const;
	void SetFirstDiagonal(const CLine2d& firstDiagonal);

	const CLine2d& GetSecondDiagonal() const;
	void SetSecondDiagonal(const CLine2d& secondDiagonal);

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
	CRectangle GetBoundingBox() const;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	CLine2d m_firstDiagonal;
	CLine2d m_secondDiagonal;
};


} // namespace i2d


#endif // !i2d_CQuadrangle_included


