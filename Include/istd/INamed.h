#ifndef istd_INamed_included
#define istd_INamed_included


#include "istd/IChangeable.h"
#include "istd/CString.h"


namespace istd
{


/**
	Interface for a object containing its name.
*/
class INamed: virtual public istd::IChangeable  
{
public:
	/**
		Get the object name.
	*/
	virtual const istd::CString& GetName() const = 0;

	/**
		Set the object name.
	*/
	virtual void SetName(const istd::CString& name) = 0;

	/**
		Return \c true, if the name setting is enabled.
	*/
	virtual bool IsNameFixed() const = 0;
};


} // namespace istd


#endif // !istd_INamed_included
