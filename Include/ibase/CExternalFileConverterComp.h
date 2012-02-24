#ifndef ibase_CExternalFileConverterComp_included
#define ibase_CExternalFileConverterComp_included


#include "ibase/IProcessExecutor.h"
#include "ibase/IFileConvertCopy.h"
#include "ibase/TLoggerCompWrap.h"

#include "iprm/IFileNameParam.h"


namespace ibase
{


/**
	File converter, which uses an external programm to perform the convert action. 
*/
class CExternalFileConverterComp:
			public ibase::CLoggerComponentBase,
			virtual public IFileConvertCopy
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CExternalFileConverterComp);
		I_REGISTER_INTERFACE(IFileConvertCopy);
		I_ASSIGN(m_processExecuterCompPtr, "ProcessExecutor", "Process executor, that will be used for convert action", true, "ProcessExecutor");
		I_ASSIGN(m_processArgumentsAttrPtr, "ProcessArguments", "Application conversion arguments.\nUse $(Input) to specify the input and $(Output) for output file name", false, "$(Input) $(Output)");
		I_ASSIGN(m_executablePathCompPtr, "ExecutablePath", "Path to the application's binary", true, "ExecutablePath");
	I_END_COMPONENT;

	// reimplemented (ibase::IFileConvertCopy)
	virtual bool ConvertFile(
				const QString& inputFilePath,
				const QString& outputFilePath,
				const iprm::IParamsSet* paramsPtr = NULL) const;

private:
	I_REF(ibase::IProcessExecutor, m_processExecuterCompPtr);
	I_ATTR(QString, m_processArgumentsAttrPtr);
	I_REF(iprm::IFileNameParam, m_executablePathCompPtr);
};


} // namespace ibase


#endif // !ibase_CExternalFileConverterComp_included


