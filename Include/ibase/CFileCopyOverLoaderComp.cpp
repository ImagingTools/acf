#include "ibase/CFileCopyOverLoaderComp.h"


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CFileCopyOverLoaderComp::CopyFile(
			const istd::CString& inputFilePath,
			const istd::CString& outputFilePath,
			const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!m_inputLoaderCompPtr.IsValid() || !m_outputLoaderCompPtr.IsValid() || !m_objectCompPtr.IsValid()){
		return false;
	}

	istd::CString usedOutputPath = outputFilePath;

	if (usedOutputPath.IsEmpty()){
		istd::CStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.empty()){
			return false;
		}

		istd::CString::size_type pointPos = inputFilePath.rfind('.');

		if (pointPos != istd::CString::npos){
			usedOutputPath = inputFilePath.substr(0, pointPos + 1) + extensions.front();
		}
		else{
			usedOutputPath = inputFilePath + extensions.front();
		}
	}

	return		(m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputFilePath) == iser::IFileLoader::StateOk) &&
				(m_outputLoaderCompPtr->SaveToFile(*m_objectCompPtr, usedOutputPath) == iser::IFileLoader::StateOk);
}


} // namespace ibase


