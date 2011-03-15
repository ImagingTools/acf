#include "ibase/CAutoPersistenceComp.h"


namespace ibase
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CAutoPersistenceComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (*m_restoreOnBeginAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, *m_filePathAttrPtr);
		}
	}
}


void CAutoPersistenceComp::OnComponentDestroyed()
{
	if (*m_storeOnEndAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			m_fileLoaderCompPtr->SaveToFile(*m_objectCompPtr, *m_filePathAttrPtr);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


