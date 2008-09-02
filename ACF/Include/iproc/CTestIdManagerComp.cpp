#include "iproc/CTestIdManagerComp.h"


namespace iproc
{


CTestIdManagerComp::CTestIdManagerComp()
	:	m_currentId(0), m_isInitialized(false)
{
}


// reimplemented (iproc::IIdManager)

bool CTestIdManagerComp::SkipToNextId(I_DWORD& result)
{
	result = ++m_currentId;

	m_isInitialized = true;

	return true;
}


bool CTestIdManagerComp::GetCurrentId(I_DWORD& result) const
{
	if (m_isInitialized){
		result = m_currentId;

		return true;
	}
	else{
		return false;
	}
}


isys::ITimer* CTestIdManagerComp::GetTimeStamp(I_DWORD id) const
{
	return NULL;
}


bool CTestIdManagerComp::GetPreviousIdFromOffset(int offset, I_DWORD& result) const
{
	if (m_isInitialized){
		result = m_currentId - I_DWORD(offset);

		return true;
	}
	else{
		return false;
	}
}


bool CTestIdManagerComp::GetOffsetFromPreviousId(I_DWORD id, int& result) const
{
	if (m_isInitialized){
		result = int(m_currentId - id);

		return true;
	}
	else{
		return false;
	}
}


} // namespace acf


