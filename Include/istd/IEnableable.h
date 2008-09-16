#ifndef istd_IEnableable_included
#define istd_IEnableable_included


#include "istd/IChangeable.h"


namespace istd
{


/**
	Interface for objects which can be enabled/disabled.	
*/
class IEnableable: virtual public IChangeable
{
public:
	/**
		Return a \c true, if something is enabled.
	*/
	virtual bool IsEnabled() const = 0;

	/**
		Return a \c true, if something can be enabled.
	*/
	virtual bool IsEnablingAllowed() const = 0;

	/**
		Set something to \c isEnabled state.
	*/
	virtual void SetEnabled(bool isEnabled = true) = 0;

};


} // namespace istd


#endif // !istd_IEnableable_included


