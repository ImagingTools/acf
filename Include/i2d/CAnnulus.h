#ifndef i2d_CAnnulus_included
#define i2d_CAnnulus_included


#include "i2d/CPosition2d.h"
#include "i2d/CCircle.h"
#include "i2d/CRectangle.h"


namespace i2d
{	


/**
	Definition of graphical annulus object.
	Annulus is area between two concentrical circles.
	This object is stored using center position and two radii.
*/
class CAnnulus: public CPosition2d
{
public:
	typedef i2d::CPosition2d BaseClass;

	CAnnulus();
	CAnnulus(const i2d::CVector2d& center, double innerRadius, double outerRadius);

	bool IsAnnulusValid() const;

	double GetInnerRadius() const;
	void SetInnerRadius(double innerRadius);

	double GetOuterRadius() const;
	void SetOuterRadius(double outerRadius);

	CCircle GetInnerCircle() const;
	CCircle GetOuterCircle() const;

	virtual CRectangle GetBoundingBox() const;

	bool operator == (const CAnnulus & annulus) const;
	bool operator != (const CAnnulus & annulus) const;

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
	istd::CRange m_radiusRange;
};


} // namespace i2d


#endif // !i2d_CAnnulus_included


