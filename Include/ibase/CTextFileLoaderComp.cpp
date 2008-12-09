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
		bool forLoading,
		bool/* forSaving*/,
		bool /*beQuiet*/) const
{
	if (dynamic_cast<const ITextDocument*>(dataObjectPtr) == NULL){
		return false;
	}

	return true;
}


int CTextFileLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, true, false, false)){
		return StateFailed;
	}

	std::wifstream fileStream(filePath.ToString().c_str(), std::ios::in);
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
	if (!IsOperationSupported(&data, &filePath, false, true, false)){
		return StateFailed;
	}

	std::wofstream fileStream(filePath.ToString().c_str(), std::ios::out);
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


bool CTextFileLoaderComp::GetFileExtensions(istd::CStringList& result, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	if (m_fileExtensionsAttrPtr.IsValid()){
		for (int extIndex = 0; extIndex < m_fileExtensionsAttrPtr.GetCount(); extIndex++){
			result.push_back(m_fileExtensionsAttrPtr[extIndex]);
		}
	}

	return true;
}


} // namespace ibase


