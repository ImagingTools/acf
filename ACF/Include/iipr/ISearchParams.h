#ifndef iipr_IISearchParams_included
#define iipr_IISearchParams_included


#include "iser/ISerializable.h"

#include "i2d/CTransform.h"
#include "i2d/CRectangle.h"

#include "iimg/IBitmap.h"


namespace iipr
{


class ISearchModel;


/**	
	This interface is common for all classes,
	which implements a set of parameters for a model search.
*/
class ISearchParams: virtual public iser::ISerializable
{
public:
	virtual void SetCalibrationPtr(const i2d::CTransform* calibrationPtr) = 0;

	/**
		Gets the region that is used for model search.
	*/
	virtual i2d::CRectangle GetSearchRegion(const i2d::CTransform* calibrationPtr = NULL) const = 0;

	/*
		Gets the region for model teaching.
	*/
	virtual i2d::CRectangle GetModelRegion(const i2d::CTransform* calibrationPtr = NULL) const = 0;
		
	/**	
		Get search model definition.
	*/
	virtual const ISearchModel& GetModel() const = 0;

	/**	
		Creates an internal search model from the image \c modelImage.
	*/
	virtual bool CreateModel(const iimg::IBitmap& inputImage) = 0;

	/**	
		Returns a \c true if the search model exists.
	*/
	virtual bool IsModelCreated() const = 0;

	/**	
		Returns a \c true if the search model is preprocessed.
	*/
	virtual bool IsModelPreprocessed() const = 0;

	/** 
		Returns the model image. 
	*/
	virtual const iimg::IBitmap& GetModelImage() const = 0;

	virtual double GetMinScore() const = 0;
	virtual void SetMinScore(double minScore) = 0;
	virtual double GetStartAngle() const = 0;
	virtual void SetStartAngle(double minAngle) = 0;
	virtual double GetEndAngle() const = 0;
	virtual void SetEndAngle(double maxAngle) = 0;
	virtual double GetMinScale() const = 0;
	virtual void SetMinScale(double minScale) = 0;
	virtual double GetMaxScale() const = 0;
	virtual void SetMaxScale(double maxScale) = 0;
	virtual int GetMatchesCount() const = 0;
	virtual void SetMatchesCount(int matchesCount) = 0;
};


} // namespace iipr


#endif // !iipr_IISearchParams_included



