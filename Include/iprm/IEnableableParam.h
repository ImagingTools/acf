#ifndef iprm_IEnableableParam_included
#define iprm_IEnableableParam_included


#include <iser/ISerializable.h>


namespace iprm
{


/**
	Interface for objects which can be enabled/disabled.	
*/
class IEnableableParam: virtual public iser::ISerializable
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
		The method returns \c true if the changing of the enabling status was allowed, otherwise \c false.
	*/
	virtual bool SetEnabled(bool isEnabled = true) = 0;
};


} // namespace iprm


#endif // !iprm_IEnableableParam_included


