#include "ibase/CFileConvertOverProcessorComp.h"


// Qt includes
#include <QtCore/QMutexLocker>

// ACF includes
#include "istd/CStaticServicesProvider.h"


namespace ibase
{


// public methods

CFileConvertOverProcessorComp::CFileConvertOverProcessorComp()
:	m_progressSessionId(0)
{
}


// reimplemented (ibase::IFileConvertCopy)

bool CFileConvertOverProcessorComp::ConvertFile(
			const QString& inputFilePath,
			const QString& outputFilePath,
			const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	QMutexLocker blocker(&m_mutex);

	if (!m_inputFileLoaderCompPtr.IsValid()){
		SendErrorMessage(0, "No file loader defined", "File processing component");

		return false;
	}

	if (!m_inputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Input data definition was not set", "File processing component");

		return false;
	}

	if (!m_outputDataCompPtr.IsValid()){
		SendErrorMessage(0, "Output data definition was not set", "File processing component");

		return false;
	}

	if (!m_inputFileLoaderCompPtr->IsOperationSupported(m_inputDataCompPtr.GetPtr(), &inputFilePath, iser::IFileLoader::QF_LOAD | iser::IFileLoader::QF_FILE)){
		SendErrorMessage(0, "File could not be loaded", "File processing component");
		
		return false;
	}

	if (m_inputFileLoaderCompPtr->LoadFromFile(*m_inputDataCompPtr.GetPtr(), inputFilePath) != iser::IFileLoader::StateOk){
		SendErrorMessage(0, "File could not be loaded", "File processing component");
		
		return false;
	}

	if (!m_processorCompPtr.IsValid()){
		SendErrorMessage(0, "File processor not set", "File processing component");

		return false;
	}

	int processingResult = m_processorCompPtr->DoProcessing(
				m_processingParamsSetCompPtr.GetPtr(),
				m_inputDataCompPtr.GetPtr(),
				m_outputDataCompPtr.GetPtr(),
				m_progressManagerCompPtr.GetPtr());
	if (processingResult != iproc::IProcessor::TS_OK){
		return false;
	}

	iser::IFileLoader* outputFileLoader = m_inputFileLoaderCompPtr.GetPtr();
	if (m_outputFileLoaderCompPtr.IsValid()){
		outputFileLoader = m_outputFileLoaderCompPtr.GetPtr(); 
	}

	I_ASSERT(outputFileLoader != NULL);

	return (outputFileLoader->SaveToFile(*m_outputDataCompPtr.GetPtr(), outputFilePath) == iser::IFileLoader::StateOk);
}


} // namespace ibase


