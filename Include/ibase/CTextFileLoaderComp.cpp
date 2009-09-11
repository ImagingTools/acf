#include "ibase/CTextFileLoaderComp.h"


// STL includes
#include <fstream>


// ACF includes
#include "ibase/ITextDocument.h"


namespace ibase
{


// reimplemented (iser::IFileLoader)

bool CTextFileLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool /*beQuiet*/) const
{
	if ((filePathPtr != NULL) && filePathPtr->IsEmpty()){
		return false;
	}

	return		((dataObjectPtr == NULL) || (dynamic_cast<const ITextDocument*>(dataObjectPtr) != NULL)) &&
				((flags & QF_ANONYMOUS_ONLY) == 0);
}


int CTextFileLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		return StateFailed;
	}

	std::basic_ifstream<wchar_t> fileStream(filePath.ToString().c_str(), std::ios::in);
	if (!fileStream.is_open()){
		return StateFailed;
	}

	int retVal = StateFailed;

	ibase::ITextDocument* documentPtr = dynamic_cast<ITextDocument*>(&data);
	I_ASSERT(documentPtr != NULL);

	istd::CString documentText;
	if (documentPtr != NULL){
		while(!fileStream.eof()){
			wchar_t textCharBuffer;
			fileStream.get(textCharBuffer);

			documentText.append(1, textCharBuffer);
		}
		
		documentPtr->SetText(documentText);
		retVal = StateOk;
	}

	fileStream.close();


	return retVal;
}


int CTextFileLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
		return StateFailed;
	}

	std::basic_ofstream<wchar_t> fileStream(filePath.ToString().c_str(), std::ios::out);
	if (!fileStream.is_open()){
		return StateFailed;
	}

	int retVal = StateFailed;

	const ibase::ITextDocument* documentPtr = dynamic_cast<const ITextDocument*>(&data);
	I_ASSERT(documentPtr != NULL);

	if (documentPtr != NULL){
		fileStream << documentPtr->GetText();

		retVal = fileStream.good() ? StateOk : StateFailed;
	}

	fileStream.close();

	return retVal;
}


bool CTextFileLoaderComp::GetFileExtensions(istd::CStringList& result, int /*flags*/, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int extensionsCount = istd::Min(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
	for (int extIndex = 0; extIndex < extensionsCount; extIndex++){
		result.push_back(m_fileExtensionsAttrPtr[extIndex]);
	}

	return true;
}


istd::CString CTextFileLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if (extensionPtr != NULL){
		int extensionsCount = istd::Min(m_fileExtensionsAttrPtr.GetCount(), m_typeDescriptionsAttrPtr.GetCount());
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


