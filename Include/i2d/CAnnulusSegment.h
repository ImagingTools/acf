#ifndef i2d_CAnnulusSegment_included
#define i2d_CAnnulusSegment_included


// ACF includes
#include "i2d/CAnnulus.h"


namespace i2d
{


/**
	Radial segment of the annulus object.
*/
class CAnnulusSegment: public CAnnulus
{
public:
	typedef CAnnulus BaseClass;

	CAnnulusSegment();
	CAnnulusSegment(
				const i2d::CVector2d& center,
				double innerRadius,
				double outerRadius,
				double beginAngle,
				double endAngle);

	double GetBeginAngle() const;
	void SetBeginAngle(double angle);
	double GetEndAngle() const;
	void SetEndAngle(double angle);

	/**
		Return \c true if a given point is inside of annulus segment area.
	*/
	bool Contains(const i2d::CVector2d& point) const; 

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

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CRange m_angleRange;
};


} // namespace i2d


#endif // !i2d_CAnnulusSegment_included


