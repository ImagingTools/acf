#include "ibase/CTextFileLogComp.h"


#include "imod/IModel.h"

#include "ibase/IMessage.h"


// STL includes
#include <iostream>


namespace ibase
{


// public methods

CTextFileLogComp:: CTextFileLogComp()
	:m_filePathObserver(*this)
{
}


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

	if (m_fileNameModelCompPtr.IsValid()){
		m_fileNameModelCompPtr->AttachObserver(&m_filePathObserver);
	}
	
	OpenFileStream();
}


void CTextFileLogComp::OnComponentDestroyed()
{
	if (m_fileNameModelCompPtr.IsValid() && m_fileNameModelCompPtr->IsAttached(&m_filePathObserver)){
		m_fileNameModelCompPtr->DetachObserver(&m_filePathObserver);
	}

	m_outputFileStream.close();

	BaseClass::OnComponentDestroyed();
}


void CTextFileLogComp::OpenFileStream()
{
	if (m_outputFileStream.is_open()){
		m_outputFileStream.flush();

		m_outputFileStream.close();
	}

	if (m_fileNameCompPtr.IsValid()){
		std::ios::openmode fileMode = std::wofstream::out | std::wofstream::app;

		if (m_isAppendAttrPtr.IsValid() && !*m_isAppendAttrPtr){
			fileMode = std::wofstream::out;
		}

		if (m_fileNameCompPtr->GetPath().IsEmpty()){
			return;
		}

#ifdef _MSC_VER
		m_outputFileStream.open(m_fileNameCompPtr->GetPath().c_str(), fileMode);
#else
		m_outputFileStream.open(m_fileNameCompPtr->GetPath().ToString().c_str(), fileMode);
#endif
	}
}


// embedded class FilePathObserver

// public methods

CTextFileLogComp::FilePathObserver::FilePathObserver(CTextFileLogComp& parent)
	:m_parent(parent)
{
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)
	
void CTextFileLogComp::FilePathObserver::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	m_parent.OpenFileStream();
}


} // namespace ibase


