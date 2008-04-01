#ifndef isys_CLock_included
#define isys_CLock_included


#include "isys/ICriticalSection.h"


namespace isys
{

/**
	Help class for automatic locking/unlocking of critical sections.
*/
class CLock
{
public:
	/**
		Constructs the lock object and lock the section \c sectionPtr.
	*/
	CLock(isys::ICriticalSection* sectionPtr);

	/**
		Destroy the lock object and unlock the previously locked section.
	*/
	virtual ~CLock();

private:
	isys::ICriticalSection* m_sectionPtr;
};


} // namespace isys


#endif // !isys_CLock_included


