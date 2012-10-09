#include "ibase/CFileCopyOverLoaderComp.h"


// Qt includes
#include <QtCore/QStringList>
#include <QtCore/QFileInfo>


namespace ibase
{


// reimplemented (ibase::IFileConvertCopy)

bool CFileCopyOverLoaderComp::ConvertFiles(
			const QString& inputPath,
			const QString& outputPath,
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

	QString usedOutputPath = outputPath;

	if (usedOutputPath.isEmpty()){
		QStringList extensions;
		m_outputLoaderCompPtr->GetFileExtensions(extensions);

		if (extensions.isEmpty()){
			SendErrorMessage(0, "File extension list is empty", "FileCopyOverLoader");
	
			return false;
		}

		int pointPos = inputPath.lastIndexOf(".");
		if (pointPos != -1){
			usedOutputPath = inputPath.left(pointPos + 1) + extensions.front();
		}
		else{
			usedOutputPath = inputPath + extensions.front();
		}
	}

	QFileInfo inputFileInfo(inputPath);
	if (!inputFileInfo.exists()){
		SendWarningMessage(0, QString("Input file %1 doesn't exist").arg(inputPath));
	}

	int loadState = m_inputLoaderCompPtr->LoadFromFile(*m_objectCompPtr, inputPath);
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


