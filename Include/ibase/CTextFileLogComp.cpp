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

	
// reimplemented (icomp::IComponent)

void CTextFileLogComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	I_ASSERT(m_fileNameAttrPtr.IsValid());

	if (m_fileNameAttrPtr.IsValid()){
		int fileMode = std::wofstream::out | std::wofstream::app;

		if (m_isAppendAttrPtr.IsValid() && !*m_isAppendAttrPtr){
			fileMode = std::wofstream::out;
		}

		m_outputFileStream.open((*m_fileNameAttrPtr).c_str(), fileMode);
	}
}


void CTextFileLogComp::OnComponentDestroyed()
{
	m_outputFileStream.close();

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


