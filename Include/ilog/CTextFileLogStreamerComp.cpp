#include "ilog/CTextFileLogStreamerComp.h"


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include "istd/IInformationProvider.h"
#include "imod/IModel.h"
#include "ilog/IMessageContainer.h"


namespace ilog
{


// public methods

CTextFileLogStreamerComp::CTextFileLogStreamerComp()
:	m_outputFile(),
	m_outputFileStream(&m_outputFile)
{
}


// reimplemented (ifile::IFilePersistence)

bool CTextFileLogStreamerComp::IsOperationSupported(
	const istd::IChangeable* dataObjectPtr,
	const QString* filePathPtr,
	int flags,
	bool /*beQuiet*/) const
{
	if ((filePathPtr != NULL) && filePathPtr->isEmpty()){
		return false;
	}

	return (dynamic_cast<const ilog::IMessageContainer*>(dataObjectPtr) != NULL) &&
		((flags & QF_SAVE) != 0) &&
		((flags & QF_FILE) != 0);
}


int CTextFileLogStreamerComp::LoadFromFile(istd::IChangeable& /*data*/, const QString& /*filePath*/) const
{
	return OS_FAILED;
}


int CTextFileLogStreamerComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return OS_FAILED;
	}

	int retVal = OS_FAILED;

	const ilog::IMessageContainer* containerPtr = dynamic_cast<const ilog::IMessageContainer*>(&data);
	if (containerPtr != NULL){
		if (!const_cast<CTextFileLogStreamerComp*>(this)->OpenFileStream(filePath)){
			return OS_FAILED;
		}

		const ilog::IMessageContainer::Messages messages = containerPtr->GetMessages();
		for (int i = 0; i < messages.count(); i++){
			const_cast<CTextFileLogStreamerComp*>(this)->AddMessage(messages.at(i));
		}

		const_cast<CTextFileLogStreamerComp*>(this)->CloseFileStream();

		retVal = OS_OK;
	}

	return retVal;
}


// reimplemented (ifile::IFileTypeInfo)

bool CTextFileLogStreamerComp::GetFileExtensions(QStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	result.append("txt");

	return true;
}


QString CTextFileLogStreamerComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "Text Log";
}


// protected methods

// reimplemented (CStreamLogCompBase)

void CTextFileLogStreamerComp::WriteText(const QString& text)
{
	if (m_outputFile.isOpen()){
		m_outputFileStream << text;
		m_outputFileStream.flush();
	}
}


void CTextFileLogStreamerComp::NewLine()
{
	if (m_outputFile.isOpen()){
		m_outputFileStream << "\n";
		m_outputFileStream.flush();
	}
}


// reimplemented (icomp::CComponentBase)

void CTextFileLogStreamerComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();
}


void CTextFileLogStreamerComp::OnComponentDestroyed()
{
	CloseFileStream();

	BaseClass::OnComponentDestroyed();
}


bool CTextFileLogStreamerComp::OpenFileStream(const QString& filePath)
{
	if (m_outputFile.isOpen()){
		CloseFileStream();
	}

	m_outputFile.setFileName(filePath);

	if (!m_outputFile.open(*m_isAppendAttrPtr ? 
			QIODevice::WriteOnly | QIODevice::Text : 
			QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
		return false;
	}

	return true;
}


void CTextFileLogStreamerComp::CloseFileStream()
{
	if (m_outputFile.isOpen()){
		m_outputFile.flush();

		m_outputFile.close();
	}
}


} // namespace

