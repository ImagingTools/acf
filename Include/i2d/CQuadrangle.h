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
	explicit CQuadrangle(const CRectangle& rectangle);

	bool operator==(const CQuadrangle& quadrangle) const;
	bool operator!=(const CQuadrangle& quadrangle) const;

	bool IsQuadrangleValid() const;
	bool IsQuadrangleEmpty() const;

	const CLine2d& GetFirstDiagonal() const;
	void SetFirstDiagonal(const CLine2d& firstDiagonal);

	const CLine2d& GetSecondDiagonal() const;
	void SetSecondDiagonal(const CLine2d& secondDiagonal);

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const CVector2d& position) override;
	virtual CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::TUniqueInterfacePtr<istd::IChangeable> CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	CLine2d m_firstDiagonal;
	CLine2d m_secondDiagonal;
};


} // namespace i2d


#endif // !i2d_CQuadrangle_included


