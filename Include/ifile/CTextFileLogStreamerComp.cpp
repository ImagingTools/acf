#include <ifile/CTextFileLogStreamerComp.h>

// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <istd/IInformationProvider.h>
#include <imod/IModel.h>
#include <ilog/IMessageContainer.h>
#include <ilog/CMessage.h>


namespace ifile
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

	// can load a file 
	if ((flags & QF_FILE) && (flags & QF_LOAD)){
		return true;
	}

	return		(dynamic_cast<const ilog::IMessageContainer*>(dataObjectPtr) != NULL) &&
				((flags & QF_SAVE) != 0) &&
				((flags & QF_FILE) != 0);
}


int CTextFileLogStreamerComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return OS_FAILED;
	}

	int retVal = OS_FAILED;

	ilog::IMessageContainer* resetContainerPtr = dynamic_cast<ilog::IMessageContainer*>(&data);
	if (resetContainerPtr){
		resetContainerPtr->ClearMessages();
	}

	ilog::IMessageConsumer* containerPtr = dynamic_cast<ilog::IMessageConsumer*>(&data);
	if (containerPtr != NULL){
		QFile textFile(filePath);
		if (textFile.open(QIODevice::ReadOnly)){
			retVal = OS_OK;

			QTextStream ts(&textFile);

			while (!ts.atEnd()){
				QString textString = ts.readLine().simplified();
				if (!textString.isEmpty()){
					int timeStartIndex = textString.indexOf('[');
					int timeEndIndex = textString.indexOf(']');
					if (timeStartIndex >= 0 && timeEndIndex >= 0){
						QString timeStampText = textString.mid(timeStartIndex+1, timeEndIndex-timeStartIndex-1);
						QDateTime timeStamp = QDateTime::fromString(timeStampText);
						
						QString text = textString.mid(timeEndIndex + 2);

						QString severityText = textString.left(timeStartIndex - 2);
						istd::IInformationProvider::InformationCategory severtity = istd::IInformationProvider::IC_NONE;
						int id = 0;
						
						if (severityText.startsWith("w")){
							severtity = istd::IInformationProvider::IC_WARNING;
							id = severityText.mid(severityText.lastIndexOf("W")+1).toInt();
						}
						else
						if (severityText.startsWith("e")){
							severtity = istd::IInformationProvider::IC_ERROR;
							id = severityText.mid(severityText.lastIndexOf("E")+1).toInt();
						}
						else
						if (severityText.startsWith("f")){
							severtity = istd::IInformationProvider::IC_CRITICAL;
							id = severityText.mid(severityText.lastIndexOf("!")+1).toInt();
						}
						else
						if (severityText.startsWith("I")){
							severtity = istd::IInformationProvider::IC_INFO;
							id = severityText.mid(1).toInt();
						}

						containerPtr->AddMessage(
							istd::TSmartPtr<const istd::IInformationProvider>
								(new ilog::CMessage(severtity, id, text, severityText, 0, &timeStamp))
							);
					}
				}
			}
		}
	}

	return retVal;
}


int CTextFileLogStreamerComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
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

bool CTextFileLogStreamerComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
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

void CTextFileLogStreamerComp::WriteText(const QString& text, istd::IInformationProvider::InformationCategory /*category*/)
{
	if (m_outputFile.isOpen()){
		m_outputFileStream << text;
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


} // namespace ifile



