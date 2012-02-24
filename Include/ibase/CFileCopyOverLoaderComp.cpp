#include "ibase/CFileCopyOverLoaderComp.h"


// Qt includes
#include <QStringList>


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CFileCopyOverLoaderComp::ConvertFile(
			const QString& inputFilePath,
			const QString& outputFilePath,
			const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	if (!m_inputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "Input data loader is not defined", "FileCopyOverLoader");

		return false;
	}

	if (!m_outputLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "Output data loader is not defined", "FileCopyOverLoader");

		return false;
	}

	if (!m_objectCompPtr.IsValid()){
		SendErrorMessage(0, "Data object for copy operaration is not set", "FileCopyOverLoader");

		return false;
	}

	QString usedOutputPath = outputFilePath;

	if (usedOutputPath.isEmpty()){
		QStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.empty()){
			SendErrorMessage(0, "File extension list is empty", "FileCopyOverLoader");
	
			return false;
		}

		int pointPos = inputFilePath.lastIndexOf(".");
		if (pointPos != -1){
			usedOutputPath = inputFilePath.left(pointPos + 1) + extensions.front();
		}
		else{
			usedOutputPath = inputFilePath + extensions.front();
		}
	}

	int loadState = m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputFilePath);
	if (loadState != iser::IFileLoader::StateOk){
		SendErrorMessage(0, "Data could not be loaded", "FileCopyOverLoader");

		return false;
	}

	int saveState = m_outputLoaderCompPtr->SaveToFile(*m_objectCompPtr, usedOutputPath);
	if (saveState != iser::IFileLoader::StateOk){
		SendErrorMessage(0, "Data could not be saved", "FileCopyOverLoader");

		return false;
	}

	return true;
}


} // namespace ibase


