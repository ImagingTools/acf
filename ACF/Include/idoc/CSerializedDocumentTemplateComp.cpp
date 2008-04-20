#include "idoc/CSerializedDocumentTemplateComp.h"


namespace idoc
{		


// reimplemented (idoc::IDocumentTemplate)

bool CSerializedDocumentTemplateComp::LoadDocumentFromFile(const istd::CString& filePath, imod::IModel& result) const
{
	istd::IChangeable* changeableDocPtr = dynamic_cast<istd::IChangeable*>(&result);

	if (m_fileLoaderCompPtr.IsValid() && (changeableDocPtr != NULL)){
		int state = m_fileLoaderCompPtr->LoadFromFile(*changeableDocPtr, filePath);
		return (state == istd::IFileLoader::StateOk);
	}

	return false;
}


bool CSerializedDocumentTemplateComp::SaveDocumentToFile(const imod::IModel& document, const istd::CString& filePath) const
{
	const istd::IChangeable* changeableDocPtr = dynamic_cast<const istd::IChangeable*>(&document);

	if (m_fileLoaderCompPtr.IsValid() && (changeableDocPtr != NULL)){
		int state = m_fileLoaderCompPtr->SaveToFile(*changeableDocPtr, filePath);
		return (state == istd::IFileLoader::StateOk);
	}

	return false;
}


} // namespace idoc


