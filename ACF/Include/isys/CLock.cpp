#include "isys/CLock.h"


namespace isys
{


CLock::CLock(isys::ICriticalSection* sectionPtr)
	:m_sectionPtr(sectionPtr)
{
	I_ASSERT(m_sectionPtr != NULL);

	if (m_sectionPtr != NULL){
		m_sectionPtr->Enter();
	}
}


CLock::~CLock()
{
	I_ASSERT(m_sectionPtr != NULL);

	if (m_sectionPtr != NULL){
		m_sectionPtr->Leave();
	}
}


} // namespace isys

