#ifndef isys_CSectionBlocker_included
#define isys_CSectionBlocker_included


#include "isys/ICriticalSection.h"


namespace isys
{


/**
	Simple helper class entering critical section during object construction and leaving it during destruction.
*/
class CSectionBlocker
{
public:
	CSectionBlocker(const isys::ICriticalSection* sectionPtr);
	~CSectionBlocker();

	void Reset();

private:
	const isys::ICriticalSection* m_sectionPtr;
};


// public inline methods

inline CSectionBlocker::CSectionBlocker(const isys::ICriticalSection* sectionPtr)
:	m_sectionPtr(sectionPtr)
{
	I_ASSERT(m_sectionPtr != NULL);
	if (m_sectionPtr != NULL){
		(const_cast<isys::ICriticalSection*>(m_sectionPtr))->Enter();
	}
}


inline CSectionBlocker::~CSectionBlocker()
{
	Reset();
}


inline void CSectionBlocker::Reset()
{	
	if (m_sectionPtr != NULL){
		(const_cast<isys::ICriticalSection*>(m_sectionPtr))->Leave();
	}

	m_sectionPtr = NULL;
}


} // namespace isys


#endif // !isys_CSectionBlocker_included


