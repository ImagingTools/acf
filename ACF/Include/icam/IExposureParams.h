#ifndef icam_IExposureParams_included
#define icam_IExposureParams_included


namespace icam
{


class ExposureParams
{
public:
	/**
		Get range of posible shutter time (exposure time) values.
		\return	range of shutter time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetShutterTimeRange() const = 0;

	/**
		Get shutter time (exposure time).
		\return	shutter time in seconds.
	*/
	virtual double GetShutterTime() const = 0;

	/**
		Set shutter time (exposure time).
		\param	time	shutter time in seconds.
		\return	true, if success.
	*/
	virtual bool SetShutterTime(double time) = 0;

	/**
		Get range of posible delay time values.
		Delay time is time between trigger and exposure begin.
		\return	range of delay time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetDelayTimeRange() const = 0;

	/**
		Get delay time.
		Delay time is time between trigger and exposure begin.
		\return	delay time in seconds.
	*/
	virtual double GetDelayTime() const = 0;

	/**
		Set delay time.
		Delay time is time between trigger and exposure begin.
		\param	time	delay time in seconds.
		\return	true, if success.
	*/
	virtual bool SetDelayTime(double time) = 0;

	/**
		Get range of posible exposure enabled (EEN) signal delay time values.
		\return	range of delay time, or empty range is this time cannot be controlled.
	*/
	virtual istd::CRange GetEenDelayRange() const = 0;

	/**
		Get exposure enabled (EEN) signal delay time.
		\return	delay time in seconds.
	*/
	virtual double GetEenDelay() const = 0;

	/**
		Set exposure enabled (EEN) signal delay time.
		\param	time	exposure enabled (EEN) signal delay time in seconds.
		\return	true, if success.
	*/
	virtual bool GetEenDelayRange(double time) = 0;
};


} // namespace icam


#endif // !icam_IExposureParams_included


