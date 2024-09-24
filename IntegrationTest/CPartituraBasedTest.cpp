#include "CPartituraBasedTest.h"


ipackage::CComponentAccessor* CPartituraBasedTestBase::GetComponentAccessor()
{
	return m_accessorPtr.get();
}


