#ifndef i2d_IObject2d_included
#define i2d_IObject2d_included


// ACF includes
#include "iser/ISerializable.h"
#include "i2d/ITransformation2d.h"
#include "i2d/ICalibrationProvider.h"


namespace i2d
{


class CVector2d;


/**
	Common interface for describing the 2D-objects.
*/
class IObject2d:
			virtual public iser::ISerializable,
			virtual public ICalibrationProvider
{
public:
	enum ChangeFlags
	{
		CF_OBJECT_POSITION = 1 << 20
	};

	/**
		Returns center of this 2D-object.
	*/
	virtual CVector2d GetCenter() const = 0;

	/**
		Move object to position \c position.
	*/
	virtual void MoveCenterTo(const CVector2d& position) = 0;

	/**
		Transform this object using some transformation.
		\param	mode			controls needed transformation exactness.
		\param	errorFactorPtr	optional output where approximated length of transformation error vector will be stored.
								Value 0 means that transformation is exactly done.
		\return	true, if  transformation was done correctly.
	*/
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) = 0;

	/**
		Do inverse transformation of this object.
		\param	mode			controls needed transformation exactness.
		\param	errorFactorPtr	optional output where approximated length of transformation error vector will be stored.
								Value 0 means that transformation is exactly done.
		\return	true, if  transformation was done correctly.
	*/
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) = 0;

	/**
		Calulate transformation of the object into second one.
		\param	mode			controls needed transformation exactness.
		\param	errorFactorPtr	optional output where approximated length of transformation error vector will be stored.
								Value 0 means that transformation is exactly done.
		\param	result			result object.
		\return	true, if  transformation was done correctly.
	*/
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const = 0;

	/**
		Calulate inverse transformation of the object into second one.
		\param	mode			controls needed transformation exactness.
		\param	errorFactorPtr	optional output where approximated length of transformation error vector will be stored.
								Value 0 means that transformation is exactly done.
		\param	result			result object.
		\return	true, if  transformation was done correctly.
	*/
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const = 0;
};


} // namespace i2d


#endif // !i2d_IObject2d_included

