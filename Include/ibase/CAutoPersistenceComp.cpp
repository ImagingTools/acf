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
			QString filePath;
			if (m_filePathCompPtr.IsValid()){
				filePath = m_filePathCompPtr->GetPath();
			}

			m_fileLoaderCompPtr->LoadFromFile(*m_objectCompPtr, filePath);
		}
	}
}


void CAutoPersistenceComp::OnComponentDestroyed()
{
	if (*m_storeOnEndAttrPtr){
		if (m_fileLoaderCompPtr.IsValid() && m_objectCompPtr.IsValid()){
			QString filePath;
			if (m_filePathCompPtr.IsValid()){
				filePath = m_filePathCompPtr->GetPath();
			}

			m_fileLoaderCompPtr->SaveToFile(*m_objectCompPtr, filePath);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


