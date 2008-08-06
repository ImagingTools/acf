#ifndef iavt_IAvtTriggerParams_included
#define iavt_IAvtTriggerParams_included


// ACF modules
#include "iser/ISerializable.h"

#include "i2d/CRectangle.h"

#include "iavt/iavt.h"


namespace iavt
{


/**
	Interface for AVT camera parameters.
*/
class IAvtTriggerParams: virtual public iser::ISerializable
{
public:
	enum TriggerMode
	{
		EdgeMode,
		LevelMode,
		BulkMode,
		UnknownTriggerMode = 0xff
	};

	enum TriggerPolarity
	{
		FallingPolarity,
		RisingPolarity,
		UnknownPolarity = 0xff
	};

	virtual bool IsTriggerEnabled() const = 0;
	virtual void SetTriggerEnabled(bool isEnabled) = 0;

	virtual int GetTriggerMode() const = 0;
	virtual void SetTriggerMode(int triggerMode) = 0;

	virtual int GetTriggerPolarity() const = 0;
	virtual void SetTriggerPolarity(int triggerPolarity) = 0;
};


} // namespace iavt


#endif // !iavt_IAvtTriggerParams_included


