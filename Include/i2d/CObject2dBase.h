#ifndef i2d_CObject2dBase_included
#define i2d_CObject2dBase_included


// ACF includes
#include "i2d/IObject2d.h"


namespace i2d
{


/**
	Base class for 2D-objects implementing interface i2d::IObject2d.
	It contains simple or empty implementations.
*/
class CObject2dBase: virtual public IObject2d
{
public:
	CObject2dBase();
	CObject2dBase(const CObject2dBase& object2d);

	/**
		Set calibration of this object.
	*/
	virtual void SetCalibration(const ITransformation2d* calibrationPtr);

	// reimplemented (i2d::ICalibrationProvider)
	virtual const ITransformation2d* GetCalibration() const;

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
				CObject2dBase& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				CObject2dBase& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

private:
	const ITransformation2d* m_calibrationPtr;
};


} // namespace i2d


#endif // !i2d_CObject2dBase_included

