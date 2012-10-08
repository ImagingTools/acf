#ifndef ibase_CFileConvertOverProcessorComp_included
#define ibase_CFileConvertOverProcessorComp_included


// Qt includes
#include <QtCore/QMutex>

// ACF includes
#include "istd/TSmartPtr.h"

#include "iser/IFileLoader.h"

#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "ibase/IProgressManager.h"
#include "iproc/IProcessor.h"


namespace ibase
{


/**
	Component for a file conversion over a data processor.
*/
class CFileConvertOverProcessorComp:
			public ibase::CLoggerComponentBase,
			virtual public ibase::IFileConvertCopy
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	CFileConvertOverProcessorComp();

	I_BEGIN_COMPONENT(CFileConvertOverProcessorComp);
		I_REGISTER_INTERFACE(ibase::IFileConvertCopy);
		I_ASSIGN(m_inputFileLoaderCompPtr, "InputFileLoader", "Loader for the input file", true, "InputFileLoader");
		I_ASSIGN(m_outputFileLoaderCompPtr, "OutputFileLoader", "Persistence for the output file. If not set, the input file loader will be used", false, "OutputFileLoader");
		I_ASSIGN(m_processorCompPtr, "DataProcessor", "Processor for the file", true, "FileProcessor");
		I_ASSIGN(m_inputDataCompPtr, "InputData", "Input data definition", true, "InputData");
		I_ASSIGN(m_outputDataCompPtr, "OutputData", "Output data definition", true, "OutputData");
		I_ASSIGN(m_progressManagerCompPtr, "ProgressManager", "Processing progress observer", false, "ProgressManager");
		I_ASSIGN(m_processingParamsSetCompPtr, "ProcessingParams", "Processing parameters", false, "ProcessingParams");
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFile(
				const QString& inputFilePath,
				const QString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_REF(iser::IFileLoader, m_inputFileLoaderCompPtr);
	I_REF(iser::IFileLoader, m_outputFileLoaderCompPtr);
	I_REF(iproc::IProcessor, m_processorCompPtr);
	I_REF(istd::IChangeable, m_inputDataCompPtr);
	I_REF(istd::IChangeable, m_outputDataCompPtr);
	I_REF(ibase::IProgressManager, m_progressManagerCompPtr);
	I_REF(iprm::IParamsSet, m_processingParamsSetCompPtr);

	int m_progressSessionId;

	mutable QMutex m_mutex;
};


} // namespace ibase


#endif // !ibase_CFileConvertOverProcessorComp_included


