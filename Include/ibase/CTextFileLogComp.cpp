#include "ibase/CTextFileLogComp.h"


// ACF includes
#include "istd/IInformationProvider.h"
#include "imod/IModel.h"


namespace ibase
{


// public methods

CTextFileLogComp:: CTextFileLogComp()
:	m_outputFile(),
	m_outputFileStream(&m_outputFile),
	m_filePathObserver(*this)
{
}


// protected methods

// reimplemented (CStreamLogCompBase)

void CTextFileLogComp::WriteText(const QString& text)
{
	if (m_outputFile.isOpen()){
		m_outputFileStream << text;
		m_outputFileStream.flush();
	}
}


void CTextFileLogComp::NewLine()
{
	if (m_outputFile.isOpen()){
		m_outputFileStream << "\n";
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

	m_outputFile.close();

	BaseClass::OnComponentDestroyed();
}


void CTextFileLogComp::OpenFileStream()
{
	if (m_outputFile.isOpen()){
		m_outputFile.flush();

		m_outputFile.close();
	}

	if (m_fileNameCompPtr.IsValid()){
		QIODevice::OpenMode openMode = QIODevice::WriteOnly | QIODevice::Truncate;

		if (*m_isAppendAttrPtr){
			openMode = QIODevice::WriteOnly | QIODevice::Append;
		}

		if (m_fileNameCompPtr->GetPath().isEmpty()){
			return;
		}

		m_outputFile.setFileName(m_fileNameCompPtr->GetPath());
		m_outputFile.open(openMode);
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


