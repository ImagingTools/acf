#ifndef i2d_CCircle_included
#define i2d_CCircle_included


#include "i2d/CPosition2d.h"


namespace i2d
{	


class CRectangle;


/**
	Definition of graphical circle object.
	This object is stored using center position and radius.
*/
class CCircle: public CPosition2d
{
public:
	typedef CPosition2d BaseClass;

	CCircle();
	CCircle(double radius, const CVector2d& center);

	double GetRadius() const;
	void SetRadius(double radius);

	virtual CRectangle GetBoundingBox() const;

	bool operator ==(const CCircle& circle) const;
	bool operator !=(const CCircle& circle) const;

	// reimplemented (i2d::IObject2d)
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

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	double m_radius;
};


// inline methods

inline double CCircle::GetRadius() const
{
	return m_radius;
}


} // namespace i2d


#endif // !i2d_CCircle_included


