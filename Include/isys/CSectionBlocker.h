#ifndef isys_CSectionBlocker_included
#define isys_CSectionBlocker_included


#include "isys/isys.h"


namespace isys{


/**
	Simple helper class entering critical section during object construction and leaving it during destruction.
*/
class CSectionBlocker
{
public:
	CSectionBlocker(ICriticalSection* sectionPtr);
	~CSectionBlocker();

private:
	ICriticalSection& m_section;
};


inline CSectionBlocker::CSectionBlocker(ICriticalSection* sectionPtr)
:	m_section(*sectionPtr)
{
	I_ASSERT(sectionPtr != NULL);

	m_section.Enter();
}


inline CSectionBlocker::~CSectionBlocker()
{
	m_section.Leave();
}


} // namespace isys


#endif // !isys_CSectionBlocker_included


