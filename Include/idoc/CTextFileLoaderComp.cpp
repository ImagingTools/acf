#include "idoc/CTextFileLoaderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include "idoc/ITextDocument.h"


namespace idoc
{


// reimplemented (ifile::IFilePersistence)

bool CTextFileLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool /*beQuiet*/) const
{
	if ((filePathPtr != NULL) && filePathPtr->isEmpty()){
		return false;
	}

	return		((dataObjectPtr == NULL) || (dynamic_cast<const idoc::ITextDocument*>(dataObjectPtr) != NULL)) &&
				((flags & (QF_LOAD | QF_SAVE)) != 0) &&
				((flags & QF_FILE) != 0);
}


int CTextFileLoaderComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return OS_FAILED;
	}

	int retVal = OS_FAILED;

	idoc::ITextDocument* documentPtr = dynamic_cast<idoc::ITextDocument*>(&data);
	if (documentPtr != NULL){
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			return OS_FAILED;
		}

		QTextStream fileStream(&file);

		QString documentText = fileStream.readAll();
		
		documentPtr->SetText(documentText);

		retVal = OS_OK;
	}


	return retVal;
}


int CTextFileLoaderComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return OS_FAILED;
	}

	int retVal = OS_FAILED;

	const idoc::ITextDocument* documentPtr = dynamic_cast<const idoc::ITextDocument*>(&data);
	if (documentPtr != NULL){
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
			return OS_FAILED;
		}

		QTextStream fileStream(&file);

		fileStream << documentPtr->GetText();

		retVal = OS_OK;
	}

	return retVal;
}


// reimplemented (ifile::IFileTypeInfo)

bool CTextFileLoaderComp::GetFileExtensions(QStringList& result, const istd::IChangeable* /*dataObjectPtr*/, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int extensionsCount = qMin(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
	for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
		result.push_back(m_fileExtensionsAttrPtr[extIndex]);
	}

	return true;
}


QString CTextFileLoaderComp::GetTypeDescription(const QString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = qMin(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
		for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
			if (m_fileExtensionsAttrPtr[extIndex] == *extensionPtr){
				return m_typeDescriptionsAttrPtr[extIndex];
			}
		}
	}

	if (m_typeDescriptionsAttrPtr.GetCount() > 0){
		return m_typeDescriptionsAttrPtr[0];
	}

	return "";
}


} // namespace idoc


