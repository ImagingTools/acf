#include "ibase/CTextFileLogComp.h"


#include "ibase/IMessage.h"


// STL includes
#include <iostream>


namespace ibase
{

		
// protected methods

// reimplemented (CStreamLogCompBase)

void CTextFileLogComp::WriteText(const istd::CString& text)
{
	if (m_outputFileStream.is_open()){
		m_outputFileStream << text.c_str();
		m_outputFileStream.flush();
	}
}


void CTextFileLogComp::NewLine()
{
	if (m_outputFileStream.is_open()){
		m_outputFileStream << std::endl;
		m_outputFileStream.flush();
	}
}


// reimplemented (icomp::CComponentBase)

void CTextFileLogComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_fileNameCompPtr.IsValid());

	if (m_fileNameCompPtr.IsValid()){
		std::ios::openmode fileMode = std::wofstream::out | std::wofstream::app;

		if (m_isAppendAttrPtr.IsValid() && !*m_isAppendAttrPtr){
			fileMode = std::wofstream::out;
		}

#ifdef _MSC_VER
		m_outputFileStream.open(m_fileNameCompPtr->GetPath().c_str(), fileMode);
#else
		m_outputFileStream.open(m_fileNameCompPtr->GetPath().ToString().c_str(), fileMode);
#endif
	}
}


void CTextFileLogComp::OnComponentDestroyed()
{
	m_outputFileStream.close();

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


