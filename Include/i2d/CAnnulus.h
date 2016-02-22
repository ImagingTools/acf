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

	/**
		Construct an empty annulus object.
	*/
	CAnnulus();

	/**
		Construct an annulus object by the given position, inner and outer radius.
	*/
	CAnnulus(const i2d::CVector2d& center, double innerRadius, double outerRadius);

	/**
		Check, if the annulus is valid.
		An annulus is valid, if its radius range is valid.
		\sa TRange::IsValid()
	*/
	bool IsAnnulusValid() const;

	/**
		Get the inner radius of the annulus.
	*/
	double GetInnerRadius() const;

	/**
		Set the inner radius of the annulus.
	*/
	void SetInnerRadius(double innerRadius);

	/**
		Get the outer radius of the annulus.
	*/
	double GetOuterRadius() const;

	/**
		Set the outer radius of the annulus.
	*/
	void SetOuterRadius(double outerRadius);

	/**
		Set the inner and outer radius of the annulus.
	*/
	void SetInnerOuterRadius(double radius1, double radius2);

	/**
		Set the inner circle of the annulus object.
	*/
	CCircle GetInnerCircle() const;

	/**
		Set the outer circle of the annulus object.
	*/
	CCircle GetOuterCircle() const;

	/**
		Compare with another annulus.
	*/
	bool operator == (const CAnnulus & annulus) const;

	/**
		Compare with another annulus.
	*/
	bool operator != (const CAnnulus & annulus) const;

	/**
		Performs conversion of the annulus splitting it to \a segmentsCount segments to the polygon \a result.
		If segmentsCount <= 0 then number of segments will be calculated automatically, otherwise segmentsCount must be equal or greater then 3.
	*/
	bool ConvertToPolygon(i2d::CPolygon& result, int segmentsCount = 0) const;

	// reimplemented (i2d::IObject2d)
	virtual CRectangle GetBoundingBox() const;
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

protected:
	istd::CRange m_radiusRange;
};


} // namespace i2d


#endif // !i2d_CAnnulus_included


