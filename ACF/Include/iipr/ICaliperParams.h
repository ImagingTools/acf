#ifndef iipr_ICaliperParams_included
#define iipr_ICaliperParams_included


#include "iser/ISerializable.h"

#include "iipr/iipr.h"


namespace iipr
{


class ICaliperParams: virtual public iser::ISerializable
{
public:
	/**
		Describe mode of polarity acceptance.
	*/
	enum PolarityMode
	{
		/**
			Only riseing edges (dark to bright) will be accepted.
		*/
		PM_RISED,
		/**
			Only droping edges (bright to dark) will be accepted.
		*/
		PM_DROPPED,
		/**
			Any polarity will be accepted.
		*/
		PM_ALL
	};

	/**
		Describe mode of search direction.
	*/
	enum DirectionMode
	{
		DM_FORWARD,
		DM_BACKWARD,
		DM_OUTSIDE,
		DM_INSIDE,
		DM_DONT_CARE
	};

	/**
		Get feature weight threshold will be used to cut weak fetures off.
	*/
	virtual double GetWeightThreshold() const = 0;

	/**
		Set feature weight threshold will be used to cut weak fetures off.
	*/
	virtual void SetWeightThreshold(double threshold) = 0;

	/**
		Get polarity mode.
		\sa PolarityMode.
	*/
	virtual int GetPolarityMode() const = 0;

	/**
		Set polarity mode.
		\sa PolarityMode.
	*/
	virtual void SetPolarityMode(int mode) = 0;

	/**
		Get direction mode.
		\sa DirectionMode.
	*/
	virtual int GetDirectionMode() const = 0;

	/**
		Set direction mode.
		\sa DirectionMode.
	*/
	virtual void SetDirectionMode(int mode) = 0;
};


} // namespace iipr


#endif // !iipr_ICaliperParams_included


