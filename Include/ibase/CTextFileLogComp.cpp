#include "ibase/CTextFileLogComp.h"


#include "ibase/IMessage.h"


// STL includes
#include <iostream>


namespace ibase
{

		
// protected methods

// reimplemented (CStreamLogCompBase)

void CTextFileLogComp::WriteLine(const istd::CString& text)
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
		m_outputFileStream.open((*m_fileNameAttrPtr).c_str(), std::wofstream::out);
	}
}


void CTextFileLogComp::OnComponentDestroyed()
{
	m_outputFileStream.close();

	BaseClass::OnComponentDestroyed();
}


} // namespace ibase


