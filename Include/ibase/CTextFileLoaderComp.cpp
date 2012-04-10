#include "ibase/CTextFileLoaderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include "ibase/ITextDocument.h"


namespace ibase
{


// reimplemented (iser::IFileLoader)

bool CTextFileLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool /*beQuiet*/) const
{
	if ((filePathPtr != NULL) && filePathPtr->isEmpty()){
		return false;
	}

	return		((dataObjectPtr == NULL) || (dynamic_cast<const ITextDocument*>(dataObjectPtr) != NULL)) &&
				((flags & (QF_LOAD | QF_SAVE)) != 0) &&
				((flags & QF_FILE) != 0);
}


int CTextFileLoaderComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_LOAD | QF_FILE, false)){
		return StateFailed;
	}

	int retVal = StateFailed;

	ibase::ITextDocument* documentPtr = dynamic_cast<ITextDocument*>(&data);
	if (documentPtr != NULL){
		QFile file(filePath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
			return StateFailed;
		}

		QTextStream fileStream(&file);

		QString documentText = fileStream.readAll();
		
		documentPtr->SetText(documentText);

		retVal = StateOk;
	}


	return retVal;
}


int CTextFileLoaderComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_SAVE | QF_FILE, false)){
		return StateFailed;
	}

	int retVal = StateFailed;

	const ibase::ITextDocument* documentPtr = dynamic_cast<const ITextDocument*>(&data);
	if (documentPtr != NULL){
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
			return StateFailed;
		}

		QTextStream fileStream(&file);

		fileStream << documentPtr->GetText();

		retVal = StateOk;
	}

	return retVal;
}


// reimplemented (iser::IFileTypeInfo)

bool CTextFileLoaderComp::GetFileExtensions(QStringList& result, int /*flags*/, bool doAppend) const
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


} // namespace ibase


