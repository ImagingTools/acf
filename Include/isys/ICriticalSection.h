#ifndef isys_ICriticalSection_included
#define isys_ICriticalSection_included


#include "isys/isys.h"


#include "istd/IPolymorphic.h"


namespace isys
{


/**
	Interface for critical section implementations.
 */
class ICriticalSection: virtual public istd::IPolymorphic
{
public:
	/**
		Enter to critical section.
		You have to call Leave() if you entered the section.
	*/
	virtual void Enter() = 0;

	/**
		Enter to critical section if it is not blocked.
		\return	true, if section was entered, false otherwise.
	*/
	virtual bool EnterIfFree() = 0;

	/**	
		Leave entered section.
	*/
	virtual void Leave() = 0;
};


} // namespace isys


#endif // !isys_ICriticalSection_included


