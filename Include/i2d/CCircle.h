#ifndef i2d_CCircle_included
#define i2d_CCircle_included


#include "i2d/CPosition2d.h"
#include "i2d/CLine2d.h"


namespace i2d
{


class CRectangle;
class CPolygon;


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

	bool operator==(const CCircle& circle) const;
	bool operator!=(const CCircle& circle) const;

	/**
		Performs conversion of the \a circle splitting it to \a segmentsCount segments to the polygon \a result.
		If segmentsCount <= 0 then number of segments will be calculated automatically, otherwise segmentsCount must be equal or greater then 3.
	*/
	bool ConvertToPolygon(i2d::CPolygon& result, int segmentsCount = 0) const;

	/**
		Check if this circle is intersected by another circle (or disk).
		\param	line		line for check.
		\param	isFilled	flag if this circle should be interpreted as filled disk or boundary only.
	*/
	bool IsIntersectedBy(const CCircle& circle, bool isFilled = false) const;

	/**
		Check if this circle is intersected by line.
		\param	line		line for check.
		\param	isFilled	flag if this circle should be interpreted as filled disk or boundary only.
	*/
	bool IsIntersectedBy(const CLine2d& line, bool isFilled = false) const;

	/**
		Get points of intersection circle by line.
		\param	line	line for check.
		\param	result	alpha values of intersection points.
						If returned value is bigger or equals 1 the first value will be set.
						If returned value is equals 2 the both values will be set.
		\return	number of intersection points. It can be 0, 1 or 2.
	*/
	int GetIntersectionAlphas(const CLine2d& line, double result[2]) const;

	/**
		Get points of intersection circle by line.
		\param	line	line for check.
		\param	result1	intersection point 1. Will be set only if returned 1 or 2.
		\param	result2	intersection point 2. Will be set only if returned 2.
		\return	number of intersection points. It can be 0, 1 or 2.
	*/
	int GetIntersectionPoints(const CLine2d& line, i2d::CVector2d& result1, i2d::CVector2d& result2) const;

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

	// reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

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


