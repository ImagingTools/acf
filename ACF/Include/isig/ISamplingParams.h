#ifndef isig_ISamplingParams_included
#define isig_ISamplingParams_included


#include "iser/ISerializable.h"

#include "isig/isig.h"


namespace isig
{


/**
	Sampler parameters.
*/
class ISamplingParams: virtual public iser::ISerializable
{
public:
	enum SamplingMode
	{
		SM_SINGLE,
		SM_PERIODIC,
		SM_SYNCHRONIZED,
		SM_LAST = ST_SYNCHRONIZED
	};

	virtual istd::CRange GetIntervalRange() const = 0;

	/**
		Get sampling interval in seconds.
	*/
	virtual double GetInterval() const = 0;
	/**
		Set sampling interval in seconds.
	*/
	virtual void SetInterval(double value) = 0;

	/**
		Check if specified sampling mode is supported.
	*/
	virtual bool IsSamplingModeSupported(int type) const = 0;

	/**
		Get mode of sampling.
		\sa	SamplingMode.
	*/
	virtual int GetSamplingMode() const = 0;

	/**
		Set mode of sampling.
		\sa	SamplingMode.
		\return	true if this mode was supported and was set correctly.
	*/
	virtual bool SetSamplingType(int type) = 0;
};


} // namespace isig


#endif // !isig_ISamplingParams_included


